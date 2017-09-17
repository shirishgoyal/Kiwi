/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Dsp/KiwiModel_AudioIOObjects.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                 AUDIO IO OBJECTS                                 //
    // ================================================================================ //
    
    void AudioIOObjects::declare()
    {
        if(DataModel::has<AudioIOObjectBase>()) return;
        
        DataModel::declare<AudioIOObjectBase>()
        .name("cicm.kiwi.object.AudioIOObjectBase")
        .inherit<model::Object>();
        
        Factory::add<DacTilde, AudioIOObjectBase>("dac~");
        Factory::add<AdcTilde, AudioIOObjectBase>("adc~");
    }
    
    // ================================================================================ //
    //                               AUDIO IO OBJECT BASE                               //
    // ================================================================================ //
    
    AudioIOObjectBase::AudioIOObjectBase(flip::Default& d)
    : model::Object(d)
    {
        ;
    }
    
    AudioIOObjectBase::AudioIOObjectBase(std::vector<Atom> const& args)
    : m_routes(parseArgsAsChannelRoutes(args))
    , m_routes_parsed(true)
    {
        ;
    }
    
    std::vector<size_t> const& AudioIOObjectBase::useRoutes() const
    {
        if(!m_routes_parsed)
        {
            auto text_atoms = AtomHelper::parse(getText());
            text_atoms.erase(text_atoms.cbegin());
            m_routes = parseArgsAsChannelRoutes(text_atoms);
            m_routes_parsed = true;
        }
        
        return m_routes;
    }
    
    std::vector<size_t> AudioIOObjectBase::parseArgsAsChannelRoutes(std::vector<Atom> const& args)
    {
        std::vector<size_t> routes;
        
        for(Atom const& arg : args)
        {
            if (arg.isNumber())
            {
                if (arg.getInt() <= 0)
                {
                    throw std::runtime_error("null or negative channel");
                }
                
                routes.push_back(arg.getInt() - 1);
            }
            else if(arg.isString())
            {
                std::string inputs(arg.getString());
                
                size_t sep_pos = inputs.find(":");
                
                if (sep_pos == std::string::npos)
                {
                    throw std::runtime_error("wrong symbol syntax");
                }
                
                int left_input = std::stoi(inputs.substr(0, sep_pos)) - 1;
                int right_input = std::stoi(inputs.substr(inputs.find(":") + 1)) - 1;
                
                if (left_input < 0 || right_input < 0)
                {
                    throw std::runtime_error("null or negative channel");
                }
                
                const bool rev = left_input > right_input;
                
                for (int channel = left_input;
                     rev ? channel >= right_input : channel <= right_input;
                     rev ? --channel : ++channel)
                {
                    routes.push_back(channel);
                }
            }
        }
        
        if (routes.empty())
        {
            routes = {0, 1};
        }
        
        return routes;
    }
    
    // ================================================================================ //
    //                                     OBJECT DAC~                                  //
    // ================================================================================ //
    
    DacTilde::DacTilde(flip::Default& d)
    : AudioIOObjectBase(d)
    {
        
    }
    
    DacTilde::DacTilde(std::string const& name, std::vector<Atom> const& args)
    : AudioIOObjectBase(args)
    {
        size_t channels = useRoutes().size();
        
        pushInlet({PinType::IType::Signal, PinType::IType::Control});
        
        for (int i = 1; i < channels; ++i)
        {
            pushInlet({PinType::IType::Signal});
        }
    }
    
    std::string DacTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            if(index < getNumberOfInlets())
            {
                auto const& routes = useRoutes();
                
                if(index == 0)
                {
                    return "Start/Stop dsp, (signal) Audio Out Channel " + std::to_string(routes[0] + 1);
                }
                else
                {
                    return "(signal) Audio Out Channel " + std::to_string(routes[index] + 1);
                }
            }
        }
        
        return {};
    }
    
    // ================================================================================ //
    //                                     OBJECT ADC~                                  //
    // ================================================================================ //
    
    AdcTilde::AdcTilde(flip::Default& d)
    : AudioIOObjectBase(d)
    {
        
    }
    
    AdcTilde::AdcTilde(std::string const& name, std::vector<Atom> const& args)
    : AudioIOObjectBase(args)
    {
        size_t channels = useRoutes().size();
        
        pushInlet({PinType::IType::Control});
        
        for (int i = 0; i < channels; ++i)
        {
            pushOutlet({PinType::IType::Signal});
        }
    }
    
    std::string AdcTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            return "Start/Stop dsp";
        }
        else
        {
            if(index < getNumberOfOutlets())
            {
                auto const& routes = useRoutes();
                return "(signal) Audio In Channel " + std::to_string(routes[index] + 1);
            }
        }
        
        return {};
    }
    
}}

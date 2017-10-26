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

#include <KiwiModel/KiwiModel_Factory.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Controller/KiwiModel_Slider.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT SLIDER                                   //
    // ================================================================================ //
    
    void Slider::declare()
    {
        std::unique_ptr<ObjectClass> slider_class(new ObjectClass("slider",
                                                                  &Slider::create));
        
        slider_class->setFlag(ObjectClass::Flag::ResizeWidth);
        slider_class->setFlag(ObjectClass::Flag::ResizeHeight);
        slider_class->setFlag(ObjectClass::Flag::DefinedSize);
        
        flip::Class<Slider> & slider_model = DataModel::declare<Slider>().inherit<Object>()
        .member<flip::Bool, &Slider::m_horizontal>("horizontal");
        
        Factory::add<Slider>(std::move(slider_class), slider_model);
    }
    
    std::unique_ptr<Object> create(std::vector<tool::Atom> const& args)
    {
        if (!args.empty())
        {
            throw std::runtime_error("wrong arguments for object slider");
        }
        
        return std::make_unique<Slider>(false);
    }
    
    Slider::Slider(bool horizontal):
    Object(),
    m_horizontal(false)
    {
        setMinWidth(20.);
        setMinHeight(20.);
        setWidth(20);
        setHeight(50.);
        pushInlet({PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    Slider::Slider(flip::Default& d):
    Object(d),
    m_horizontal(false)
    {
    }
    
    std::string Slider::getIODescription(bool is_inlet, size_t index) const
    {
        if (is_inlet && index == 0)
        {
            return "Sets current slider value. Value is between 0 and 1";
        }
        else if(!is_inlet && index == 0)
        {
            return "Outputs the slider's value";
        }
        else
        {
            return "";
        }
    }
}}

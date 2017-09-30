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

#include <KiwiModel/Kiwi_Objects/Kiwi_Dsp/Kiwi_TimesTilde.h>

#include <KiwiModel/Kiwi_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT TIMES~                                   //
    // ================================================================================ //
    
    TimesTilde::TimesTilde(std::string const& name, std::vector<Atom> const& args)
    : model::Object(name, args)
    {
        pushInlet({PinType::IType::Signal});
        
        if (args.empty() || !args[0].isNumber())
        {
            pushInlet({PinType::IType::Signal, PinType::IType::Control});
        }
        
        pushOutlet(PinType::IType::Signal);
    }
    
    void TimesTilde::declare()
    {
        Factory::add<TimesTilde>("times~").addAlias("*~");
    }
    
    std::string TimesTilde::getIODescription(bool is_inlet, size_t index) const
    {
        return is_inlet ? (index == 0) ? "(signal) Left operand" : "(signal) Right operand" : "(signal) Output";
    }
    
}}
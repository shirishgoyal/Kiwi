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

#include <KiwiModel/Kiwi_ObjectBox.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  PIN TYPE MODEL                                  //
    // ================================================================================ //
    
    PinType::PinType(IType type):
    m_type(type)
    {
        
    }
    
    PinType::IType PinType::getType() const
    {
        return !removed() ? m_type.value() : m_type.before();
    }
    
    bool PinType::operator<(PinType const& other) const
    {
        return getType() < other.getType();
    }
    
    bool PinType::operator==(PinType const& other) const
    {
        return !(*this < other) && !(other < *this);
    }
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    Inlet::Inlet(std::set<PinType> types):
    m_types()
    {
        m_types.insert(m_types.begin(), types.begin(), types.end());
    }
    
    bool Inlet::hasType(PinType type) const
    {
        return (m_types.find_if([type](PinType const& inlet_type){
            return inlet_type == type;
        }) != m_types.end());
    }
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    Outlet::Outlet(PinType type):
    m_type(type)
    {
    }
    
    PinType const& Outlet::getType() const
    {
        return m_type;
    }
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    ObjectBox::ObjectBox(flip::Default& d)
    {
        ;
    }
}}

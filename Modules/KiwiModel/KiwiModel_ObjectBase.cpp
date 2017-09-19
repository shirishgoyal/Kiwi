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

#include "KiwiModel_DataModel.h"
#include "KiwiModel_ObjectBase.h"

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OBJECT BASE                                   //
    // ================================================================================ //
    
    void ObjectBase::declare()
    {
        if(DataModel::has<model::ObjectBase>()) return;
        
        DataModel::declare<model::ObjectBase>()
        .name("cicm.kiwi.ObjectBase");
    }
    
    ObjectBase::ObjectBase()
    : m_class(nullptr)
    , m_signals()
    {
        ;
    }
    
    Factory::ObjectClassBase const* ObjectBase::getClass() const
    {
        if(m_class == nullptr)
        {
            assert(is_bound() && "The object has not been created by the model::Factory neither by flip...");
            
            m_class = Factory::getClassByModelName(get_class().name());
        }
        
        return m_class;
    }
    
    std::string ObjectBase::getClassName() const
    {
        if(const auto* class_ptr = getClass())
        {
            return class_ptr->getClassName();
        }
        
        return {};
    }
}}

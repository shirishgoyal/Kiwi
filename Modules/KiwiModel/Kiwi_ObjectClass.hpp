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

#pragma once

#include "flip/Mold.h"

#include <KiwiModel/Kiwi_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT CLASS BASE                               //
    // ================================================================================ //
    
    template<class TModel>
    auto ObjectClassBase::getCtor() -> ctor_fn_t
    {
        return [](std::string const& name, std::vector<Atom> const& args)
        {
            return std::make_unique<TModel>(name, args);
        };
    }
    
    template<class TModel>
    auto ObjectClassBase::getMoldMaker() -> mold_maker_fn_t
    {
        return [](model::Object const& object, flip::Mold& mold)
        {
            // make a mold with container_flag active
            mold.make(static_cast<TModel const&>(object), false);
        };
    }
    
    template<class TModel>
    auto ObjectClassBase::getMoldCaster() -> mold_caster_fn_t
    {
        return [](flip::Mold const& mold)
        {
            flip::Default d;
            auto object_uptr = std::make_unique<TModel>(d);
            mold.cast<TModel>(static_cast<TModel&>(*(object_uptr.get())));
            return object_uptr;
        };
    }
    
    template<class TObject, class ...Args>
    std::unique_ptr<TObject> ObjectClassBase::create(Args&& ...args) const
    {
        auto object = std::make_unique<TObject>(std::forward<Args>(args)...);
        object->m_class = this;
        return object;
    }
    
    // ================================================================================ //
    //                                  OBJECT CLASS                                    //
    // ================================================================================ //
    
    template<class TObjectClass, class TInheritedObject>
    ObjectClass<TObjectClass, TInheritedObject>::ObjectClass(std::string const& name)
    : ObjectClassBase(name,
                      getCtor<class_t>(),
                      getMoldMaker<class_t>(),
                      getMoldCaster<class_t>())
    , m_flip_class(DataModel::declare<class_t>())
    {
        m_flip_class.name(getDataModelName().c_str())
        .template inherit<TInheritedObject>();
    }
    
    template<class TObjectClass, class TInheritedObject>
    template<class U, U TObjectClass::*ptr_to_member>
    void ObjectClass<TObjectClass, TInheritedObject>::addMember(char const* name)
    {
        m_flip_class.template member(name);
    }
    
}}

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

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   OBJECT FACTORY                                 //
    // ================================================================================ //
    
    template<class TObject>
    struct Factory::isValidObject
    {
        enum
        {
            value = std::is_base_of<model::Object, TObject>::value
            && !std::is_abstract<TObject>::value
            && std::is_constructible<TObject, std::string, std::vector<Atom>>::value
            && std::is_constructible<TObject, flip::Default &>::value
        };
    };
    
    template<class TObjectBase>
    struct Factory::isValidObjectBase
    {
        enum
        {
            value = (std::is_same<model::Object, TObjectBase>::value
                     || std::is_base_of<model::Object, TObjectBase>::value)
        };
    };
    
    template<class TObject, class TObjectBase>
    std::unique_ptr<Factory::ObjectClass<TObject, TObjectBase>>
    Factory::createClass(std::string const& name)
    {
        static_assert(isValidObjectBase<TObjectBase>::value, "Not a valid inheritable Object");
        static_assert(isValidObject<TObject>::value, "Not a valid Object");
        
        assert(DataModel::has<TObjectBase>() && "Inherited object class must be added first");
        
        assert(!name.empty() && "Object name empty");
        
        assert(!DataModel::has<TObject>() && "Object has already been declared");
        
        return std::make_unique<ObjectClass<TObject, TObjectBase>>(name);
    }
    
    template<class TObject, class TObjectBase>
    Factory::ObjectClass<TObject, TObjectBase>& Factory::add(std::string const& name)
    {
        auto object_class_ptr = createClass<TObject, TObjectBase>(name);
        auto& object_class = *object_class_ptr;
        
        Factory::add(std::move(object_class_ptr));
        
        return object_class;
    }
    
    template<class TObject>
    auto Factory::getCtor() -> ctor_fn_t
    {
        return [](std::string const& name, std::vector<Atom> const& args)
        {
            return std::make_unique<TObject>(name, args);
        };
    }
    
    template<class TObject>
    auto Factory::getMoldMaker() -> mold_maker_fn_t
    {
        return [](model::Object const& object, flip::Mold& mold)
        {
            // make a mold with container_flag active
            mold.make(static_cast<TObject const&>(object), false);
        };
    }
    
    template<class TObject>
    auto Factory::getMoldCaster() -> mold_caster_fn_t
    {
        return [](flip::Mold const& mold)
        {
            flip::Default d;
            auto object_uptr = std::make_unique<TObject>(d);
            mold.cast<TObject>(static_cast<TObject&>(*(object_uptr.get())));
            return object_uptr;
        };
    }
    
    // ================================================================================ //
    //                             FACTORY OBJECT CLASS BASE                            //
    // ================================================================================ //
    
    template<class TObject, class ...Args>
    std::unique_ptr<TObject> Factory::ObjectClassBase::create(Args&& ...args) const
    {
        auto object = std::make_unique<TObject>(std::forward<Args>(args)...);
        object->m_class = this;
        return object;
    }
    
    // ================================================================================ //
    //                               FACTORY OBJECT CLASS                               //
    // ================================================================================ //
    
    template<class TObjectClass, class TObjectBase>
    Factory::ObjectClass<TObjectClass, TObjectBase>::ObjectClass(std::string const& name)
    : Factory::ObjectClassBase(name,
                               Factory::sanitizeName(name),
                               getCtor<object_class_t>(),
                               getMoldMaker<object_class_t>(),
                               getMoldCaster<object_class_t>())
    , m_flip_class(DataModel::declare<object_class_t>())
    {
        m_flip_class.name(getDataModelName().c_str())
        .template inherit<TObjectBase>();
    }
    
    template<class TObjectClass, class TObjectBase>
    template<class U, U TObjectClass::*ptr_to_member>
    auto Factory::ObjectClass<TObjectClass, TObjectBase>::addMember(char const* name) -> class_t&
    {
        m_flip_class.template member(name);
        return *this;
    }
}}

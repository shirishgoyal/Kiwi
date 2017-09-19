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
    
    template<class TModel>
    struct Factory::isValidObject
    {
        enum
        {
            value = std::is_base_of<model::Object, TModel>::value
            && !std::is_abstract<TModel>::value
            && std::is_constructible<TModel, std::string, std::vector<Atom>>::value
            && std::is_constructible<TModel, flip::Default &>::value
        };
    };
    
    template<class TInheritedObject>
    struct Factory::isValidInheritableObject
    {
        enum
        {
            value = (std::is_same<model::Object, TInheritedObject>::value
                     || std::is_base_of<model::Object, TInheritedObject>::value)
        };
    };
    
    template<class TModel, class TInheritedObject>
    Factory::ObjectClass<TModel, TInheritedObject>& Factory::add(std::string const& name)
    {
        static_assert(isValidInheritableObject<TInheritedObject>::value, "Not a valid inheritable Object");
        static_assert(isValidObject<TModel>::value, "Not a valid Object");
        
        assert(DataModel::has<TInheritedObject>() && "Inherited object must be declared first");
        
        assert((!name.empty() && !DataModel::has<TModel>())
               && "Object name empty or object class already added");
        
        // check if the name match the name of another object in the factory.
        if(has(name))
        {
            throw std::runtime_error("The \"" + name + "\" object is already in the factory");
        }
        
        auto& object_classes = getClasses();
        const auto it = object_classes.emplace(object_classes.end(),
                                               std::make_unique<ObjectClass<TModel, TInheritedObject>>(name));
        
        return dynamic_cast<ObjectClass<TModel, TInheritedObject>&>(*(it->get()));
    }
    
    template<class TModel>
    auto Factory::getCtor() -> ctor_fn_t
    {
        return [](std::string const& name, std::vector<Atom> const& args)
        {
            return std::make_unique<TModel>(name, args);
        };
    }
    
    template<class TModel>
    auto Factory::getMoldMaker() -> mold_maker_fn_t
    {
        return [](model::Object const& object, flip::Mold& mold)
        {
            // make a mold with container_flag active
            mold.make(static_cast<TModel const&>(object), false);
        };
    }
    
    template<class TModel>
    auto Factory::getMoldCaster() -> mold_caster_fn_t
    {
        return [](flip::Mold const& mold)
        {
            flip::Default d;
            auto object_uptr = std::make_unique<TModel>(d);
            mold.cast<TModel>(static_cast<TModel&>(*(object_uptr.get())));
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
    
    template<class TObjectClass, class TInheritedObject>
    Factory::ObjectClass<TObjectClass, TInheritedObject>::ObjectClass(std::string const& name)
    : Factory::ObjectClassBase(name,
                               Factory::sanitizeName(name),
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
    void Factory::ObjectClass<TObjectClass, TInheritedObject>::addMember(char const* name)
    {
        m_flip_class.template member(name);
    }
}}

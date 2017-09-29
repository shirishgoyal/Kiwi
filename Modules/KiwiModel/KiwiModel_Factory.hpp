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

#include <KiwiModel/KiwiModel_DataModel.h>

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
    ObjectClass<TModel, TInheritedObject>& Factory::add(std::string const& name)
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
    
}}

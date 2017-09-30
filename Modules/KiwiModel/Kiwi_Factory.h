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

#include <KiwiModel/Kiwi_Atom.h>
#include <KiwiModel/Kiwi_ObjectClass.h>

#include <functional>
#include <set>
#include <vector>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   OBJECT FACTORY                                 //
    // ================================================================================ //
    
    //! @brief The model Object's factory
    class Factory
    {   
    public: // methods
        
        //! @brief isValidObject type traits
        //! @details To be considered as valid an object must:
        //! - not be abstract.
        //! - inherit from model::Object.
        //! - be constructible with flip::Default.
        //! - be constructible with a string and a vector of atoms.
        template<class TModel>
        struct isValidObject;
        
        //! @brief isValidInheritableObject type traits
        //! @details A valid inheritable object must inherit from or be a model::Object
        template<class TInheritedObject>
        struct isValidInheritableObject;
        
        //! @brief Adds an object model into the Factory.
        //! @details The function throw if the object has already been added.
        //! The name you pass in parameter will be used and stored in the DataModel,
        //! thus if you pass a different name later, this will imply a breaking change in the DataModel.
        //! @param name The name of the object (must not be empty and not already used by another object or alias name in the Factory).
        //! @see isValidObject, isValidInheritableObject
        template<class TModel, class TInheritedObject = model::Object>
        static ObjectClass<TModel, TInheritedObject>& add(std::string const& name);
        
        //! @brief Creates a new Object with a name and arguments.
        //! @details This function will throw if the object name does not exist.
        //! @param name The name of the Object.
        //! @param args A list of arguments as a vector of Atom.
        //! @return A ptr to a model::Object.
        static std::unique_ptr<model::Object> create(std::vector<Atom> const& args);
        
        //! @brief Creates a new Object from a flip::Mold.
        //! @details This function will throw if the object name does not exist.
        //! @param name The name of the Object to create.
        //! @param mold The flip::Mold from which to create the object.
        //! @return A ptr to a model::Object.
        static std::unique_ptr<model::Object> create(std::string const& name,
                                                     flip::Mold const& mold);
        
        //! @brief Make a mold of this object.
        //! @details This function will throw if the object does not exist.
        //! @param object The Object to copy.
        //! @param mold The flip::Mold from which to create the object.
        //! @return A ptr to a model::Object.
        static void copy(model::Object const& object, flip::Mold& mold);
        
        //! @brief Returns a ptr to an object class thas has this name or alias name.
        //! @details This method returns nullptr if the name is not found.
        //! @param name The name or an alias name of the class.
        //! @param ignore_aliases Default to false, pass true to ignore aliases.
        //! @return A ptr to an ObjectClassBase.
        static ObjectClassBase* getClassByName(std::string const& name,
                                               const bool ignore_aliases = false);
        
        static ObjectClassBase* getClassByModelName(std::string const& name,
                                                    const bool ignore_aliases = false);
        
        
        //! @brief Returns true if a given string match a registered object class name.
        //! @param name The name of the object class to find.
        //! @return true if the object class has been added, otherwise false.
        static bool has(std::string const& name);
        
        //! @brief Gets the names of the objects that has been added to the Factory.
        //! @param ignore_aliases Default to false, you may pass true to exclude them.
        //! @param ignore_internals Default to true, you may pass false to include them.
        //! @return A vector of object class names.
        static std::vector<std::string> getNames(const bool ignore_aliases = false,
                                                 const bool ignore_internals = true);
        
    private: // methods
        
        using object_classes_t = std::vector<std::unique_ptr<ObjectClassBase>>;
        
        //! @internal Returns the object classes.
        static object_classes_t& getClasses();
        
    private: // deleted methods
        
        Factory() = delete;
        ~Factory() = delete;
    };
    
}}

#include "Kiwi_Factory.hpp"

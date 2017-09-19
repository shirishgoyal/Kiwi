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

#include "KiwiModel_DataModel.h"
#include "KiwiModel_Atom.h"

#include <functional>
#include <set>

namespace kiwi { namespace model {
    
    class Object;
    
    // ================================================================================ //
    //                                   OBJECT FACTORY                                 //
    // ================================================================================ //
    
    //! @brief The model Object's factory
    class Factory
    {
    public: // classes
        
        class ObjectClassBase;
        template<class TObject, class TObjectBase> class ObjectClass;
        
    public: // methods
        
        //! @brief isValidObject type traits
        //! @details To be considered as valid an object must:
        //! - not be abstract.
        //! - inherit from model::Object.
        //! - be constructible with flip::Default.
        //! - be constructible with a string and a vector of atoms.
        template<class TObject> struct isValidObject;
        
        //! @brief isValidObjectBase type traits
        //! @details A valid inheritable object must inherit from or be a model::Object
        template<class TObjectBase> struct isValidObjectBase;
        
        //! @brief Create a new ObjectClass.
        //! @see Factory::add
        template<class TObject, class TObjectBase = model::Object>
        static std::unique_ptr<ObjectClass<TObject, TObjectBase>>
        createClass(std::string const& name);
        
        //! @brief Add a new ObjectClass to the Factory.
        //! @details Use the createClass method to get an ObjectClass.
        //! @see Factory::createClass
        static void add(std::unique_ptr<ObjectClassBase> object_class);
        
        //! @brief Adds an object model into the Factory.
        //! @details The function throw if the object has already been added.
        //! The name you pass in parameter will be used and stored in the DataModel,
        //! thus if you pass a different name later, this will imply a breaking change in the DataModel.
        //! @param name The name of the object (must not be empty and not already used by another object or alias name in the Factory).
        //! @see isValidObject, isValidObjectBase
        template<class TObject, class TObjectBase = model::Object>
        static ObjectClass<TObject, TObjectBase>& add(std::string const& name);
        
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
        using ctor_fn_t = std::function<std::unique_ptr<model::Object>(std::string const&, std::vector<Atom>)>;
        using mold_maker_fn_t = std::function<void(model::Object const&, flip::Mold&)>;
        using mold_caster_fn_t = std::function<std::unique_ptr<model::Object>(flip::Mold const&)>;
        
        //! @internal Returns a constructor function.
        template<class TObject>
        static ctor_fn_t getCtor();
        
        //! @internal Returns a mold maker function.
        template<class TObject>
        static mold_maker_fn_t getMoldMaker();
        
        //! @internal Returns a mold caster function.
        template<class TObject>
        static mold_caster_fn_t getMoldCaster();
        
        //! @internal Returns a sanitized version of the string.
        static std::string sanitizeName(std::string const& name);
        
        //! @internal Returns the object classes.
        static object_classes_t& getClasses();
        
    private: // deleted methods
        
        Factory() = delete;
        ~Factory() = delete;
    };
    
    // ================================================================================ //
    //                             FACTORY OBJECT CLASS BASE                            //
    // ================================================================================ //
    
    //! @brief ObjectClass base class
    class Factory::ObjectClassBase
    {
    public: // methods
        
        //! @brief Constructor.
        ObjectClassBase(std::string const& name,
                        std::string const& model_name,
                        const ctor_fn_t ctor,
                        const mold_maker_fn_t mold_maker,
                        const mold_caster_fn_t  mold_caster);
        
        //! @brief Destructor.
        virtual ~ObjectClassBase() = default;
        
        //! @brief Returns the name of the object.
        std::string const& getClassName() const;
        
        //! @brief Returns the name used into the data model of kiwi.
        std::string const& getDataModelName() const;
        
        //! @brief Returns true if it's an internal object.
        bool isInternal() const noexcept;
        
        //! @brief Pass true if this is an internal object.
        void setInternal(const bool is_internal) noexcept;
        
        //! @brief Returns true if this object class has aliases.
        bool hasAlias() const noexcept;
        
        //! @brief Pass true if this is an internal object.
        std::set<std::string> const& getAliases() const noexcept;
        
        //! @brief Returns true if this class has this alias name.
        bool hasAlias(std::string const& alias) const noexcept;
        
        //! @brief Adds a creator name alias to the class.
        void addAlias(std::string alias);
        
        //! @brief Creates and returns a new Object with a vector of Atom as parameter.
        //! @param args A vector of Atom.
        //! @return A new model::Object.
        std::unique_ptr<model::Object> create(std::vector<Atom> const& args) const;
        
        //! @brief Creates and returns a new Object with a vector of Atom as parameter.
        //! @param typed_name the classname or alias name of the object.
        //! @param args A vector of Atom.
        //! @return A new model::Object.
        std::unique_ptr<model::Object> create(std::string const& typed_name,
                                              std::vector<Atom> const& args) const;
        
        //! @brief Creates and returns a new TObject with custom arguments.
        //! @param args Variadic arguments forwarded to the object constructor.
        //! @return A new model::Object.
        template<class TObject, class ...Args>
        std::unique_ptr<TObject> create(Args&& ...args) const;
        
        //! @brief Copy the content an object instance into a flip::Mold.
        //! @param object The object instance.
        //! @param mold The The flip::Mold where to copy object instance data.
        void moldMake(model::Object const& object, flip::Mold& mold) const;
        
        //! @brief Creates and returns a new Object from a flip::Mold.
        //! @param mold The The flip::Mold from which to retrieve object instance data.
        //! @return A new model::Object.
        std::unique_ptr<model::Object> moldCast(flip::Mold const& mold) const;
        
    private: // members
        
        const std::string       m_name {};
        const std::string       m_model_name {};
        std::set<std::string>   m_aliases {};
        const ctor_fn_t         m_ctor {};
        const mold_maker_fn_t   m_mold_maker {};
        const mold_caster_fn_t  m_mold_caster {};
        bool                    m_internal = false;
    };
    
    // ================================================================================ //
    //                               FACTORY OBJECT CLASS                               //
    // ================================================================================ //
    
    //! @brief ObjectClass
    template<class TObject, class TObjectBase = model::Object>
    class Factory::ObjectClass : public ObjectClassBase
    {
    public: // methods
        
        using object_class_t = TObject;
        using class_t = ObjectClass<object_class_t>;
        
        //! @brief Constructor
        ObjectClass(std::string const& name);
        
        //! @brief Destructor
        ~ObjectClass() = default;
        
        //! @brief Adds a member to the class
        //! @details This function returns a reference to the ObjectClass to allow call chaining.
        template<class U, U object_class_t::*ptr_to_member>
        class_t& addMember(char const* name);

    private: // members
        
        flip::Class<object_class_t>& m_flip_class;
    };
}}

#include "KiwiModel_Factory.hpp"

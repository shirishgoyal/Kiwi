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

#include <KiwiModel/Kiwi_DataModel.h>
#include <KiwiModel/Kiwi_Atom.h>

#include <functional>
#include <set>

namespace kiwi { namespace model {
    
    class Object;
    
    // ================================================================================ //
    //                                  OBJECT CLASS BASE                               //
    // ================================================================================ //
    
    //! @brief ObjectClass base class
    class ObjectClassBase
    {
    private: // typedefs
        
        using ctor_fn_t = std::function<std::unique_ptr<model::Object>(std::string const&, std::vector<Atom>)>;
        using mold_maker_fn_t = std::function<void(model::Object const&, flip::Mold&)>;
        using mold_caster_fn_t = std::function<std::unique_ptr<model::Object>(flip::Mold const&)>;
        
    public: // methods
        
        //! @brief Constructor.
        ObjectClassBase(std::string const& name,
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
        
    private: // methods
        
        //! @internal Returns a sanitized version of the string.
        static std::string sanitizeName(std::string const& name);
        
        //! @internal Returns a constructor function.
        template<class TModel>
        static ctor_fn_t getCtor();
        
        //! @internal Returns a mold maker function.
        template<class TModel>
        static mold_maker_fn_t getMoldMaker();
        
        //! @internal Returns a mold caster function.
        template<class TModel>
        static mold_caster_fn_t getMoldCaster();
        
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
    //                                  OBJECT CLASS                                    //
    // ================================================================================ //
    
    //! @brief ObjectClass
    template<class TObjectClass, class TInheritedObject>
    class ObjectClass : public ObjectClassBase
    {
    public: // methods
        
        using class_t = TObjectClass;
        using parent_class_t = TInheritedObject;
        
        //! @brief Constructor
        ObjectClass(std::string const& name);
        
        //! @brief Destructor
        ~ObjectClass() = default;
        
        //! @brief Adds a member to the class
        template<class U, U class_t::*ptr_to_member>
        void addMember(char const* name);
        
    private: // members
        
        flip::Class<class_t>& m_flip_class;
    };
}}

#include <KiwiModel/Kiwi_ObjectClass.hpp>

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

#include <KiwiModel/Kiwi_ObjectBase.h>

#include "flip/Bool.h"
#include "flip/Int.h"
#include "flip/Float.h"
#include "flip/String.h"
#include "flip/Array.h"
#include "flip/Collection.h"
#include "flip/Object.h"
#include "flip/ObjectRef.h"
#include "flip/Enum.h"
#include "flip/Signal.h"

namespace kiwi { namespace model {
    
    class Inlet;
    class Outlet;
    
    // ================================================================================ //
    //                                   OBJECT BOX                                     //
    // ================================================================================ //
    
    //! @brief The ObjectBox is a base class for kiwi object boxes.
    //! @details Object boxes can be instantiated in a Patcher.
    class ObjectBox : public ObjectBase
    {
    public: // methods
        
        //! @brief Default constructor.
        ObjectBox() = default;
        
        //! @brief flip Default constructor.
        ObjectBox(flip::Default& d);
        
        //! @brief Destructor.
        virtual ~ObjectBox() = default;
        
    public: // properties
        
        flip::String        m_typed_name;
        flip::String        m_additional_text;
        flip::Array<Inlet>  m_inlets;
        flip::Array<Outlet> m_outlets;
        
        flip::Float         m_position_x;
        flip::Float         m_position_y;
        flip::Float         m_width;
        flip::Float         m_height;
        
    public: // declaration
        
        //! @brief flip class declaration.
        template<class TDataModel>
        static void declare();
        
    private: // deleted methods
        
        ObjectBox(ObjectBox const&) = delete;
        ObjectBox(ObjectBox&&) = delete;
        ObjectBox& operator=(ObjectBox const&) = delete;
        ObjectBox& operator=(ObjectBox&&) = delete;
    };
    
    // ================================================================================ //
    //                                  PIN TYPE MODEL                                  //
    // ================================================================================ //
    
    //! @brief Class that represent a type of pin.
    //! @details This class is a flip object wrapper on an enum.
    //! It's needed for putting the type into a flip::Array.
    class PinType : public flip::Object
    {
    public: // enum
        
        enum class IType
        {
            Control,
            Signal
        };
        
    public: // methods
        
        //! @brief Default constructor.
        PinType() = default;
        
        // @brief Constructor of type.
        PinType(IType type);
        
        //! @brief Comparison operator.
        bool operator<(PinType const& other) const;
        
        //! @brief Equality comparator. Consistent with comparison operator.
        bool operator==(PinType const& other) const;
        
        //! @brief Returns the type or the previous type if the Type is deleted.
        //! @details During document changed phase the type can be tagged as removed.
        IType getType() const;
        
    public: // properties
        
        flip::Enum<IType> m_type;
        
    private: // declaration
        
        //! @internal class declarator (called by ObjectBox::declare).
        template<class TDataModel>
        static void declare();
        
        template<class TDataModel>
        friend void ObjectBox::declare();
    };
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    //! @brief Class that represent an inlet able to have multiple types.
    class Inlet : public flip::Object
    {
    public: // methods
        
        //! @brief Default constructor.
        Inlet() = default;
        
        //! @brief Initializes the Inlet with multiple types.
        Inlet(std::set<PinType> types);
        
        //! @brief The destructor.
        ~Inlet() = default;
        
        //! @brief Checks if the inlet is compatible  with type.
        bool hasType(PinType type) const;
        
    public: // properties
        
        flip::Array<PinType> m_types;
        
    private: // declaration
        
        //! @internal class declarator (called by ObjectBox::declare).
        template<class TDataModel>
        static void declare();
        
        template<class TDataModel>
        friend void ObjectBox::declare();
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    //! @brief Class that represent a certain outlet having only one type.
    class Outlet : public flip::Object
    {
    public: // methods
        
        //! @brief Default constructor.
        Outlet() = default;
        
        //! @brief Initializes the Outlet with a type.
        Outlet(PinType type);
        
        //! @brief The destructor.
        ~Outlet() = default;
        
        // @brief Returns the type of the outlet.
        PinType const& getType() const;
        
    public: // properties
        
        PinType m_type;
    
    private: // declaration
        
        //! @internal class declarator (called by ObjectBox::declare).
        template<class TDataModel>
        static void declare();
        
        template<class TDataModel>
        friend void ObjectBox::declare();
    };
    
}}

#include <KiwiModel/Kiwi_ObjectBox.hpp>

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

#include "flip/ObjectRef.h"
#include "flip/Int.h"

namespace kiwi { namespace model {
    
    class ObjectBox;
    
    // ================================================================================ //
    //                                   LINK MODEL                                     //
    // ================================================================================ //
    
    //! @brief The Link is used to create a connection between two objects.
    //! @details The Link holds a reference to the sender Object, to the receiver Object
    //! and inlet and outlet indexes.
    
    class Link : public ObjectBase
    {
    public: // methods
        
        Link(flip::Default&);
        
        //! @brief Constructs a Link.
        //! @details Constructs a Link with given origin and destination Object pointers
        //! and IO indexes.
        //! @param from     The sender Object.
        //! @param outlet   The sender outlet index.
        //! @param to       The receiver Object.
        //! @param inlet    The receiver inlet index.
        Link(ObjectBox const& from, const size_t outlet,
             ObjectBox const& to, const size_t inlet);
        
        //! @brief Destructor.
        ~Link() = default;
        
    public: // properties
        
        flip::ObjectRef<ObjectBox>    sender;
        flip::ObjectRef<ObjectBox>    receiver;
        flip::Int                     outlet_index;
        flip::Int                     inlet_index;
        
    public: // declaration
        
        //! @brief flip class declaration.
        template<class TDataModel>
        static void declare();
    };
    
}}

#include <KiwiModel/Kiwi_Link.hpp>

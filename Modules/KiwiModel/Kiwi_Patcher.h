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
#include <KiwiModel/Kiwi_PatcherUser.h>

#include "flip/Collection.h"
#include "flip/Array.h"
#include "flip/String.h"

namespace kiwi { namespace model {
    
    class ObjectBox;
    class Link;
    class PatcherView;
    
    // ================================================================================ //
    //                                  PATCHER MODEL                                   //
    // ================================================================================ //
    
    //! @brief The Patcher manages a set of Object and Link
    class Patcher : public ObjectBase
    {
    public: // methods
        
        using ObjectBoxes = flip::Array<ObjectBox>;
        using Links = flip::Array<Link>;
        using Users = flip::Collection<PatcherUser>;
        
        //! @brief Default constructor.
        Patcher();
        
        //! @brief Destructor.
        ~Patcher() = default;
        
        //! @brief Adds an object to the patcher model.
        //! @param The object to add.
        //! @return A reference to the object added.
        ObjectBox& addObjectBox(std::unique_ptr<ObjectBox> && object);
        
        //! @brief Returns true if an Object has been added, removed or changed.
        bool objectsChanged() const noexcept;
        
        //! @brief Returns true if a Link has been added, removed or changed.
        bool linksChanged() const noexcept;
        
        //! @brief Returns true if a User has been added, removed or changed.
        bool usersChanged() const noexcept;
        
        //! @brief Returns true if the Patcher name changed.
        bool nameChanged() const noexcept;
        
        //! @brief Returns the Patcher name.
        std::string getName() const;
        
        //! @brief Sets the Patcher name.
        void setName(std::string const& new_name);
        
        //! @brief Gets the objects.
        ObjectBoxes const& getObjectBoxes() const noexcept;
        
        //! @brief Gets the objects.
        ObjectBoxes& getObjectBoxes() noexcept;
        
        //! @brief Gets the links.
        Links const& getLinks() const noexcept;
        
        //! @brief Gets the links.
        Links& getLinks() noexcept;
        
        //! @brief Gets the users.
        Users const& getUsers() const noexcept;
        
        //! @brief Gets the users.
        Users& getUsers() noexcept;
        
        //! @brief Returns the current User.
        //! @details The function will look for a User that match
        //! the current user id of the document, if it's not found, the User will be created,
        //! therefore, do not use this method while observing the model.
        //! @return The current User.
        PatcherUser& useSelfUser();
        
    public: // signals
        
        enum
        {
            Signal_USER_CONNECT     = 0,
            Signal_USER_DISCONNECT,
            Signal_GET_CONNECTED_USERS,
            Signal_RECEIVE_CONNECTED_USERS,
        };
        
        // from server to client
        flip::Signal<uint64_t>              signal_user_connect;
        
        // from server to client
        flip::Signal<uint64_t>              signal_user_disconnect;
        
        // from client to server
        flip::Signal<>                      signal_get_connected_users;
        
        // from server to client
        flip::Signal<std::vector<uint64_t>> signal_receive_connected_users;
        
    public: // internal methods
        
        //! @brief flip class declaration
        template<class TDataModel>
        static void declare();
        
    public: // methods
        
        ObjectBoxes::const_iterator  findObject(ObjectBox const& object) const;
        ObjectBoxes::iterator        findObject(ObjectBox const& object);
        
        Links::const_iterator    findLink(Link const& link) const;
        Links::iterator          findLink(Link const& link);
        
    private: // members
        
        flip::String    m_patcher_name;
        
        ObjectBoxes     m_objects;
        Links           m_links;
        
        Users           m_users;
        
    private: // deleted methods
        
        Patcher(Patcher const&) = delete;
        Patcher(Patcher&&) = delete;
        Patcher& operator=(Patcher const&) = delete;
        Patcher& operator=(Patcher&&) = delete;
    };
}}

#include <KiwiModel/Kiwi_Patcher.hpp>

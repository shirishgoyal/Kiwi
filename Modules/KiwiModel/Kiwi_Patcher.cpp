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

#include <KiwiModel/Kiwi_Patcher.h>
#include <KiwiModel/Kiwi_Link.h>
#include <KiwiModel/Kiwi_ObjectBox.h>
#include <KiwiModel/Kiwi_PatcherUser.h>
#include <KiwiModel/Kiwi_PatcherView.h>

#include "flip/Signal.h"

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  PATCHER MODEL                                   //
    // ================================================================================ //
    
    Patcher::Patcher()
    {
        addSignal<uint64_t>(Signal_USER_CONNECT, *this);
        addSignal<uint64_t>(Signal_USER_DISCONNECT, *this);
        addSignal<>(Signal_GET_CONNECTED_USERS, *this);
        addSignal<std::vector<uint64_t>>(Signal_RECEIVE_CONNECTED_USERS, *this);
        
        // user changes doesn't need to be stored in an history.
        m_users.disable_in_undo();
    }
    
    ObjectBox& Patcher::addObjectBox(std::unique_ptr<ObjectBox>&& object)
    {
        assert(object != nullptr);
        return *m_objects.insert(m_objects.end(), std::move(object));
    }
    
    Link& Patcher::addLink(std::unique_ptr<Link>&& link)
    {
        assert(link != nullptr);
        return *m_links.insert(m_links.end(), std::move(link));
    }
    
    bool Patcher::objectsChanged() const noexcept
    {
        return m_objects.changed();
    }
    
    bool Patcher::linksChanged() const noexcept
    {
        return m_links.changed();
    }
    
    bool Patcher::usersChanged() const noexcept
    {
        return m_users.changed();
    }
    
    bool Patcher::nameChanged() const noexcept
    {
        return m_patcher_name.changed();
    }
    
    std::string Patcher::getName() const
    {
        return !m_patcher_name.removed() ? static_cast<std::string>(m_patcher_name) : "Untitled";
    }
    
    void Patcher::setName(std::string const& new_name)
    {
        m_patcher_name = new_name;
    }
    
    Patcher::ObjectBoxes const& Patcher::getObjectBoxes() const noexcept
    {
        return m_objects;
    }
    
    auto Patcher::getObjectBoxes() noexcept -> ObjectBoxes&
    {
        return m_objects;
    }
    
    Patcher::Links const& Patcher::getLinks() const noexcept
    {
        return m_links;
    }
    
    Patcher::Links& Patcher::getLinks() noexcept
    {
        return m_links;
    }
    
    Patcher::Users const& Patcher::getUsers() const noexcept
    {
        return m_users;
    }
    
    Patcher::Users& Patcher::getUsers() noexcept
    {
        return m_users;
    }
    
    PatcherUser& Patcher::useSelfUser()
    {
        auto it = std::find_if
        (m_users.begin(), m_users.end(),
         [self_id = document().user()](auto const& user) {
            return (user.getId() == self_id);
        });
        
        // creates and returns a new user if it didn't exist.
        return (it != m_users.end()) ? *it : *m_users.emplace();
    }
    
    auto Patcher::findObject(ObjectBox const& object) const -> ObjectBoxes::const_iterator
    {
        const auto find_it = [&object](ObjectBox const& object_model)
        {
            return (object.ref() == object_model.ref());
        };
        
        return std::find_if(m_objects.begin(), m_objects.end(), find_it);
    }
    
    auto Patcher::findLink(Link const& link) const -> Links::const_iterator
    {
        const auto find_it = [&link](Link const& link_model)
        {
            return (link.ref() == link_model.ref());
        };
        
        return std::find_if(m_links.begin(), m_links.end(), find_it);
    }
    
    auto Patcher::findObject(ObjectBox const& object) -> ObjectBoxes::iterator
    {
        const auto find_it = [&object](ObjectBox const& object_model)
        {
            return (object.ref() == object_model.ref());
        };
        
        return std::find_if(m_objects.begin(), m_objects.end(), find_it);
    }
    
    auto Patcher::findLink(Link const& link) -> Links::iterator
    {
        const auto find_it = [&link](Link const& link_model)
        {
            return (link.ref() == link_model.ref());
        };
        
        return std::find_if(m_links.begin(), m_links.end(), find_it);
    }
    
}}

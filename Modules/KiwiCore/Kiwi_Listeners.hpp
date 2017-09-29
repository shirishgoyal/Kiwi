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

#include <memory>
#include <set>
#include <vector>
#include <mutex>

namespace kiwi { namespace core {
    
    // ================================================================================ //
    //                                      LISTENERS                                   //
    // ================================================================================ //
    
    template <class TListener>
    Listeners<TListener>::Listeners()
    {
        static_assert(is_valid_listener::value,
                      "Template parameter must not be a pointer or a reference");
    }
    
    template <class TListener>
    Listeners<TListener>::~Listeners() noexcept
    {
        m_listeners.clear();
    }
    
    template <class TListener>
    bool Listeners<TListener>::add(listener_ref_t listener) noexcept
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        bool insert_success = m_listeners.insert(&listener).second;
        return insert_success;
    }
    
    template <class TListener>
    bool Listeners<TListener>::remove(listener_ref_t listener) noexcept
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        bool erase_success = m_listeners.erase(&listener);
        return erase_success;
    }
    
    template <class TListener>
    size_t Listeners<TListener>::size() const noexcept
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_listeners.size();
    }
    
    template <class TListener>
    bool Listeners<TListener>::empty() const noexcept
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_listeners.empty();
    }
    
    template <class TListener>
    void Listeners<TListener>::clear() noexcept
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_listeners.clear();
    }
    
    template <class TListener>
    bool Listeners<TListener>::contains(listener_ref_t listener) const noexcept
    {
        return (m_listeners.find(&listener) != m_listeners.end());
    }
    
    template <class TListener>
    auto Listeners<TListener>::getListeners() -> std::vector<listener_ptr_t>
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return {m_listeners.begin(), m_listeners.end()};
    }
    
    template <class TListener>
    auto Listeners<TListener>::getListeners() const -> std::vector<listener_ptr_t>
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return {m_listeners.begin(), m_listeners.end()};
    }
    
    template <class TListener>
    template <class T, class ...Args>
    void Listeners<TListener>::call(T fun, Args&& ...arguments) const
    {
        for(auto* listener : getListeners())
        {
            (listener->*(fun))(arguments...);
        }
    }
    
}}

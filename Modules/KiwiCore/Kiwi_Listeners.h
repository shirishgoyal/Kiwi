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
    
    //! @brief The listener set is a class that manages a list of listeners.
    //! @details Manages a list of listeners and allows to retrieve them easily and thread-safely.
    template <class TListener>
    class Listeners
    {
    public:
        
        struct is_valid_listener : std::integral_constant<bool,
        !std::is_pointer<TListener>::value &&
        !std::is_reference<TListener>::value
        > {};
        
        using listener_t = typename std::remove_pointer<typename std::remove_reference<TListener>::type>::type;
        using listener_ref_t = listener_t&;
        using listener_ptr_t = listener_t*;
        
        //! @brief Creates an empty listener set.
        Listeners();
        
        //! @brief Destructor.
        ~Listeners() noexcept;
        
        //! @brief Add a listener.
        //! @details If the listener was allready present in the set, the function does nothing.
        //! @param listener The new listener to be added.
        //! @return True if success, otherwise false.
        bool add(listener_ref_t listener) noexcept;
        
        //! @brief Remove a listener.
        //! @details If the listener wasn't in the set, the function does nothing.
        //! @param listener The listener to be removed.
        //! @return True if success, false otherwise.
        bool remove(listener_ref_t listener) noexcept;
        
        //! @brief Returns the number of listeners.
        size_t size() const noexcept;
        
        //! @brief Returns true if there is no listener.
        bool empty() const noexcept;
        
        //! @brief Remove all listeners.
        void clear() noexcept;
        
        //! @brief Returns true if the set contains a given listener.
        bool contains(listener_ref_t listener) const noexcept;
        
        //! @brief Get the listeners.
        std::vector<listener_ptr_t> getListeners();
        
        //! @brief Retrieve the listeners.
        std::vector<listener_ptr_t> getListeners() const;
        
        //! @brief Calls a given method for each listener of the set.
        //! @param fun The listener's method to call.
        //! @param arguments optional arguments.
        template<class T, class ...Args>
        void call(T fun, Args&& ...arguments) const;
        
    private:
        
        std::set<listener_ptr_t> m_listeners;
        mutable std::mutex       m_mutex;
    };
}}

#include <KiwiCore/Kiwi_Listeners.hpp>

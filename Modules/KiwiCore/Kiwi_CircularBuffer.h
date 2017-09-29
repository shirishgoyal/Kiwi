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

#include <algorithm>
#include <cstdlib>
#include <cassert>

namespace kiwi { namespace core {
    
    // ================================================================================ //
    //                                  CIRCULAR BUFFER                                 //
    // ================================================================================ //
    
    //! @todo documentation inspired from boost circular buffer
    //! @see http://www.boost.org/doc/libs/1_59_0/doc/html/boost/circular_buffer.html#idp23975984-bb
    template<class T>
    class CircularBuffer
    {
    public: // methods
        
        CircularBuffer(size_t capacity);
        
        CircularBuffer(size_t capacity, size_t size, T const& value);
        
        ~CircularBuffer();
        
        void assign(size_t nb_elements, const T& value);
        
        void clear();
        
        size_t size() const;
        
        T const& operator[](size_t index) const noexcept;
        
        T& operator[](size_t index) noexcept;
        
        void push_back(T const& value);
        
        void pop_front();
        
    private: // methods
        
        void increment_head();
        
        void increment_tail();
        
    private: // members
        
        T*             m_buffer;
        size_t         m_head;
        size_t         m_tail;
        size_t         m_size;
        size_t         m_capacity;
        
    private: // deleted methods
        
        CircularBuffer() = delete;
        CircularBuffer(CircularBuffer const& other) = delete;
        CircularBuffer(CircularBuffer && other) = delete;
        CircularBuffer& operator=(CircularBuffer const& other) = delete;
        CircularBuffer& operator=(CircularBuffer && other) = delete;  
    };
    
}}

#include <KiwiCore/Kiwi_CircularBuffer.hpp>

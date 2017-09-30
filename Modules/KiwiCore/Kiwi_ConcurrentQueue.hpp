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

namespace kiwi { namespace core {
    
    // ==================================================================================== //
    //                                  CONCURRENT QUEUE                                    //
    // ==================================================================================== //
    
    template <class TElem>
    ConcurrentQueue<TElem>::ConcurrentQueue(size_t capacity):
    m_queue(capacity),
    m_size(0)
    {
        ;
    }
    
    template <class TElem>
    ConcurrentQueue<TElem>::~ConcurrentQueue()
    {
        ;
    }
    
    template <class TElem>
    void ConcurrentQueue<TElem>::push(TElem const& value)
    {
        if(m_queue.enqueue(value))
        {
            m_size++;
        }
    }
    
    template <class TElem>
    void ConcurrentQueue<TElem>::push(TElem&& value)
    {
        if(m_queue.enqueue(std::forward<TElem>(value)))
        {
            m_size++;
        }
    }
    
    template <class TElem>
    bool ConcurrentQueue<TElem>::pop(TElem & value)
    {
        if(m_queue.try_dequeue(value))
        {
            m_size--;
            return true;
        }
        
        return false;
    }
    
    template <class TElem>
    size_t ConcurrentQueue<TElem>::load_size() const
    {
        return m_size.load();
    }
    
}}

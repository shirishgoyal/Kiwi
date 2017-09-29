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

#include <cstddef>
#include <atomic>

#include <concurrentqueue.h>

namespace kiwi { namespace core {
    
    // ==================================================================================== //
    //                                  CONCURRENT QUEUE                                    //
    // ==================================================================================== //
    
    //! @brief A multi producer, multi consumer FIFO lock free queue.
    //! @details Wrapper around a thirdparty concurrent queue.
    //! @see https://github.com/cameron314/concurrentqueue
    template <class TElem>
    class ConcurrentQueue final
    {
    public: // methods
        
        using elem_t = TElem;
        
        //! @brief Constructor.
        //! @details Reserves memory space for at least capacity elements.
        ConcurrentQueue(size_t capacity);
        
        //! @brief Destructor.
        ~ConcurrentQueue();
        
        //! @brief Pushes element at end of queue (by copy).
        //! @details If number of elements exceeds capacity allocation will occur.
        //! Increments element counter.
        void push(elem_t const& value);
        
        //! @brief Pushes element at end of queue (by move).
        //! @details If number of elements exceeds capacity allocation will occur.
        //! Increments element counter.
        void push(elem_t&& value);
        
        //! @brief Pops first element in the queue.
        //! @details Returns false if the queue was empty and pop failed, true otherwise.
        bool pop(elem_t & value);
        
        //! @brief Returns an approximative size for the queue.
        //! @details Since size is increased and decreased after elements are effectively pushed or poped,
        //! for the consumer the returned size is guaranteed to be lower or equal
        //! to the effective size of the queue, for the producer the returned size is guaranteed to be
        //! greater or equal than the effective size of the queue.
        size_t load_size() const;
        
    private: // classes
        
        struct Trait : public moodycamel::ConcurrentQueueDefaultTraits
        {
            static const size_t BLOCK_SIZE = 1024;
        };
        
    private: // members
        
        moodycamel::ConcurrentQueue<elem_t, Trait>  m_queue;
        std::atomic<int>                            m_size;
        
    private: // deleted methods
        
        ConcurrentQueue() = delete;
        ConcurrentQueue(ConcurrentQueue const& other) = delete;
        ConcurrentQueue(ConcurrentQueue && other) = delete;
        ConcurrentQueue& operator=(ConcurrentQueue const& other) = delete;
        ConcurrentQueue& operator=(ConcurrentQueue && other) = delete;
    };
    
}}

#include <KiwiCore/Kiwi_ConcurrentQueue.hpp>

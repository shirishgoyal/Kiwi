/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "KiwiDspSignal.hpp"

namespace kiwi
{
namespace dsp
{
    Signal::Signal() noexcept :
    m_samples(nullptr), m_size(0ul), m_owner(true)
    {
        
    }
    
    Signal::Signal(const size_t size, const sample val) :
    m_samples(nullptr), m_size(0ul), m_owner(true)
    {
        std::cout << "Signal base constructor " << reinterpret_cast<size_t>(this) << "\n";
        m_samples = Samples<sample>::allocate(size);
        if(m_samples)
        {
            m_size  = size;
            m_owner = true;
            Samples<sample>::fill(size, val, m_samples);
        }
        else
        {
            class ErrorAlloc : public Error
            {
                const char* what() const noexcept final {return "Kiwi::Dsp::Signal : can't allocate samples !";}
            };
            throw Error();
        }
    }
    
    Signal::Signal(Signal const& other) :
    m_samples(nullptr), m_size(0ul), m_owner(true)
    {
        std::cout << "Signal copy constructor " << reinterpret_cast<size_t>(this) << "\n";
        if(other.m_samples && other.m_size)
        {
            m_samples = Samples<sample>::allocate(other.m_size);
            if(m_samples)
            {
                m_size  = other.m_size;
                m_owner = true;
                Samples<sample>::copy(other.m_size, other.m_samples, m_samples);
            }
            else
            {
                class ErrorAlloc : public Error
                {
                    const char* what() const noexcept final {return "Kiwi::Dsp::Signal : can't allocate samples !";}
                };
                throw Error();
            }
        }
    }
    
    Signal::Signal(Signal&& other) noexcept :
    m_samples(nullptr), m_size(0ul), m_owner(other.m_owner)
    {
        std::cout << "Signal move constructor " << reinterpret_cast<size_t>(this) << "\n";
        other.m_owner = true;
        std::swap(m_samples, other.m_samples);
        std::swap(m_size, other.m_size);
    }
    
    Signal::~Signal()
    {
        std::cout << "Signal destructor " << reinterpret_cast<size_t>(this);
        if(m_owner && m_samples)
        {
            std::cout << " : deletion";
            Samples<sample>::release(m_samples);
        }
        m_samples = nullptr;
        m_size    = 0ul;
        std::cout << "\n";
    }
}
}
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

#include "flip/Object.h"
#include "flip/Signal.h"
#include "flip/Class.h"

#include <map>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   OBJECT BASE                                    //
    // ================================================================================ //
    
    //! @brief The ObjectBase is the base class of all kiwi objects.
    class ObjectBase : public flip::Object
    {
    public: // definitions
        
        using SignalKey = uint32_t;
        
    public: // methods
        
        //! @brief Constructor
        ObjectBase();
        
        //! @brief Destructor.
        virtual ~ObjectBase() = default;
        
        //! @brief Returns the object's signal referenced by this key.
        //! @details Throws an exception if no signal is referenced for key.
        template <class... Args>
        auto& getSignal(SignalKey key) const;
        
    public: // internal methods
        
        //! @brief flip class declaration
        template<class TDataModel>
        static void declare();
        
    protected: // methods
        
        //! @brief Adds a signal having signal key.
        template <class... Args>
        void addSignal(SignalKey key, model::ObjectBase& object);
        
    private: // members
        
        std::map<SignalKey, std::unique_ptr<flip::SignalBase>> m_signals;
        
    private: // deleted methods
        
        ObjectBase(ObjectBase const&) = delete;
        ObjectBase(model::ObjectBase&&) = delete;
        model::ObjectBase& operator=(model::ObjectBase const&) = delete;
        model::ObjectBase& operator=(model::ObjectBase&&) = delete;
    };
    
}}

#include <KiwiModel/Kiwi_ObjectBase.hpp>

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

//! @todo Clean flip headers below, use only needed one in this file

// ---- Flip headers ---- //
#include "flip/Bool.h"
#include "flip/Int.h"
#include "flip/Float.h"
#include "flip/String.h"
#include "flip/Array.h"
#include "flip/Collection.h"
#include "flip/Object.h"
#include "flip/ObjectRef.h"
#include "flip/Enum.h"
#include "flip/Signal.h"

#include "KiwiModel_Atom.h"

#include <mutex>
#include <algorithm>
#include <exception>
#include <set>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OBJECT BASE                                   //
    // ================================================================================ //
    
    //! @brief The ObjectBase is the base class of all kiwi objects.
    class ObjectBase : public flip::Object
    {
    public: // methods
        
        //! @brief Constructor
        ObjectBase();
        
        //! @brief Destructor.
        virtual ~ObjectBase() = default;
        
        //! @brief Returns the model::Factory::ObjectClassBase.
        //! @details This method will assert if this object was not created by flip or via the object's Factory.
        Factory::ObjectClassBase const* getClass() const;
        
        //! @brief Returns the class name of the object.
        //! @brief the classname is the name of the object as declared in the object Factory for this object class.
        std::string getClassName() const;
        
        using SignalKey = uint32_t;
        
        //! @brief Returns the object's signal referenced by this key.
        //! @details Throws an exception if no signal is referenced for key.
        template <class... Args>
        auto& getSignal(SignalKey key) const;
        
    protected: // methods
        
        //! @brief Adds a signal having singal key.
        template <class... Args>
        void addSignal(SignalKey key, model::ObjectBase& object);
        
    public: // internal methods
        
        //! @internal flip static declare method
        static void declare();
        
    private: // members
        
        mutable Factory::ObjectClassBase const* m_class = nullptr;
        
        std::map<SignalKey, std::unique_ptr<flip::SignalBase>> m_signals;
        
        friend class Factory;
        
    private: // deleted methods
        
        ObjectBase(ObjectBase const&) = delete;
        ObjectBase(model::ObjectBase&&) = delete;
        model::ObjectBase& operator=(model::ObjectBase const&) = delete;
        model::ObjectBase& operator=(model::ObjectBase&&) = delete;
    };
}}

#include "KiwiModel_ObjectBase.hpp"

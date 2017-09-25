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

#include "flip/DocumentValidator.h"

#include <KiwiModel/Kiwi_PatcherRoot.h>

namespace kiwi { namespace model {
    
    class ObjectBox;
    
    // ================================================================================ //
    //                             KIWI DOCUMENT VALIDATOR                              //
    // ================================================================================ //
    
    class PatcherValidator : public flip::DocumentValidator<PatcherRoot>
    {
    public: // methods
        
        PatcherValidator() = default;
        ~PatcherValidator() = default;
        
        //! @brief Validate the model.
        virtual void validate(PatcherRoot& root) override;
        
    private: // methods
        
        //! @brief Validate the Patcher.
        void validatePatcher(Patcher& patcher);
        
        //! @brief Carry out checks once a object is removed.
        void objectRemoved(ObjectBox const& object, Patcher const& patcher) const;
        
        //! @brief Carry out checks once a link is created.
        void linkAdded(Link const& link) const;
        
    private: // deleted methods
        
        PatcherValidator(PatcherValidator const& other) = delete;
        PatcherValidator(PatcherValidator && other) = delete;
        PatcherValidator& operator=(PatcherValidator const& other) = delete;
        PatcherValidator& operator=(PatcherValidator && other) = delete;
    };
    
}}

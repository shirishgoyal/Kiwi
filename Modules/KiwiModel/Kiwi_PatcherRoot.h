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

#include <KiwiModel/Kiwi_Patcher.h>

namespace kiwi { namespace model {
    
    class Patcher;
    
    // ================================================================================ //
    //                                   PATCHER ROOT                                   //
    // ================================================================================ //
    
    //! @brief The Patcher root is the document root.
    //! @details The PatcherRoot contains a single "top-patcher" that can have multiple "sub-patchers".
    class PatcherRoot : public flip::Object
    {
    public: // methods
        
        //! @brief Default constructor.
        PatcherRoot();
        
        //! @brief Destructor.
        ~PatcherRoot() = default;
        
        //! @brief Returns the Patcher.
        Patcher& usePatcher();
        
        //! @brief Returns the Patcher (const version).
        Patcher const& usePatcher() const;
        
    public: // internal methods
        
        //! @brief flip class declaration
        static void declare();
        
    private: // members
        
        Patcher m_patcher;
        
    private: // deleted methods
        
        PatcherRoot(PatcherRoot const&) = delete;
        PatcherRoot(PatcherRoot&&) = delete;
        PatcherRoot& operator=(PatcherRoot const&) = delete;
        PatcherRoot& operator=(PatcherRoot&&) = delete;
    };
}}

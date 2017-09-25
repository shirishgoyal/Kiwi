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

#include "flip/DataModel.h"

#define KIWI_MODEL_VERSION_STRING "v0.2.0"

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  KIWI DATA MODEL                                 //
    // ================================================================================ //
    
    struct PatcherDataModel : public flip::DataModel<PatcherDataModel>
    {
        //! @brief Declare the DataModel.
        //! @details This method must be called only once.
        //! After the DataModel declaration,
        //! you can use its singleton instance using the static use() method.
        static void declare();
        
        //! @brief Returns true if this DataModel has been declared
        static bool declared();
        
    private: // methods
        
        //! @internal Declare Patcher classes.
        static void declarePatcher();
        
        //! @internal Declare object boxes.
        static void declareObjectBoxes();
        
    private: // variables
        
        static bool declared_flag;
    };
    
}}

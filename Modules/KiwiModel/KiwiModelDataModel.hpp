/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_MODEL_DATA_MODEL_HPP_INCLUDED
#define KIWI_MODEL_DATA_MODEL_HPP_INCLUDED

#include "flip/DataModel.h"

namespace kiwi
{
    namespace model
    {
        //! @brief The Patcher Model class
        class DataModel : public flip::DataModel<DataModel>
        {
        public:
            
            //! @brief Initializes the model.
            //! @details Declares all flip classes.
            static void init();
            
        private:
            
            //! @brief Declare all kiwi objects here
            static void declareObjects();
        };
    }
}

#endif // KIWI_MODEL_DATA_MODEL_HPP_INCLUDED

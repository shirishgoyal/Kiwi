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

#ifndef KIWI_ENGINE_INSTANCE_HPP_INCLUDED
#define KIWI_ENGINE_INSTANCE_HPP_INCLUDED

#include "flip/Document.h"

#include "KiwiGuiDevice.hpp"
#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        class Instance
        {
        public:
            
            //! @brief Constructor.
            //! @param user_id The user ID.
            Instance(uint64_t user_id, std::unique_ptr<GuiDevice> gui_device);
            
            //! @brief Destructor.
            ~Instance();
            
            //! @brief Get the user ID of the Instance.
            uint64_t getUserId() const noexcept;
            
            //! @brief Create and returns a Timer.
            //! @param callback The timer callback method. 
            std::unique_ptr<Timer> createTimer(std::function<void()> callback) const;
            
        private:
            
            class PatcherModelDeclarator;
            
            const uint64_t              m_user_id;
            
            std::unique_ptr<GuiDevice>  m_gui_device;
        };
    }
}


#endif // KIWI_ENGINE_INSTANCE_HPP_INCLUDED

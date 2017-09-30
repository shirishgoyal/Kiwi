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

#include <KiwiModel/Kiwi_Factory.h>
#include <KiwiModel/Kiwi_Object.h>

#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::unique_ptr<Object> Factory::create(Patcher& patcher, model::Object const& object)
        {
            auto& creators = getCreators();
            auto const* const flip_class = &object.get_class();
            
            assert(creators.count(flip_class) != 0 && "The object has not been registered.");
            
            const auto args = AtomHelper::parse(object.getAdditionalText());
            return std::unique_ptr<Object>(creators[flip_class](object, patcher, args));
            
            return nullptr;
        }
        
        auto Factory::getCreators() -> creator_map_t&
        {
            static creator_map_t static_creators;
            return static_creators;
        }
    }
}

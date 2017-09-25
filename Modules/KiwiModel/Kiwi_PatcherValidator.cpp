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

#include <KiwiModel/Kiwi_PatcherValidator.h>
#include <KiwiModel/Kiwi_Link.h>
#include <KiwiModel/Kiwi_ObjectBox.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                             KIWI DOCUMENT VALIDATOR                              //
    // ================================================================================ //
    
    void PatcherValidator::validate(PatcherRoot & root)
    {
        if(root.changed())
        {
            validatePatcher(root.usePatcher());
        }
    }
    
    void PatcherValidator::validatePatcher(Patcher& patcher)
    {
        if (patcher.changed())
        {
            if (patcher.objectsChanged())
            {
                for(ObjectBox const& object : patcher.getObjectBoxes())
                {
                    if(object.removed())
                    {
                        objectRemoved(object, patcher);
                    }
                }
            }
            
            if (patcher.linksChanged())
            {
                for(Link const& link : patcher.getLinks())
                {
                    if(link.added())
                    {
                        linkAdded(link);
                    }
                }
            }
        }
    }
    
    void PatcherValidator::objectRemoved(ObjectBox const& object, Patcher const& patcher) const
    {
        for(Link const& link : patcher.getLinks())
        {
            if(!link.removed() && (link.sender.value() == &object || link.receiver.value() == &object))
            {
                flip_VALIDATION_FAILED("Removing object without removing its links");
            }
        }    
    }
    
    void PatcherValidator::linkAdded(Link const& link) const
    {
        if(link.sender.value() == nullptr || link.receiver.value() == nullptr)
        {
            flip_VALIDATION_FAILED("Creating link to non existing object");
        }
    }
    
}}

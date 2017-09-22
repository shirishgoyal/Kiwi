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

#include <KiwiModel/Kiwi_ObjectBox.h>

// ================================================================================ //
//                                    OBJECT BOXES                                  //
// ================================================================================ //

#define KIWI_STRINGIFY_MACRO_HELPER(a) #a

//! @internal A handy C macro for stringifying any symbol, rather than just a macro parameter.
#define KIWI_STRINGIFY(item) KIWI_STRINGIFY_MACRO_HELPER(item)

#define OBJECTBOX_MODEL_NAME_PREFIX cicm.kiwi.patch.boxes
#define OBJECTBOX_MODEL_NAME(NAME) KIWI_STRINGIFY(OBJECTBOX_MODEL_NAME_PREFIX.NAME)

//! @brief The following macro provides boilerplate code for simple ObjectBox classes declaration
//! @details It creates a class that inherits from ObjectBox.
//! with a default ctor, a flip default ctor and declare method.
//! @todo Allow to inherit from a subclass of ObjectBox.
//! @todo Add Member declaration macro.

#define OBJECT_BOX(ClassName)                               \
struct ClassName : public ObjectBox                         \
{                                                           \
    ClassName() {}                                          \
    ClassName(flip::Default& d) : ObjectBox(d) {}           \
                                                            \
    template<class TDataModel>                              \
    static void declare()                                   \
    {                                                       \
        flip::Class<ClassName>::declare()                   \
        .name(OBJECTBOX_MODEL_NAME(ClassName))              \
        .inherit<ObjectBox>();                              \
                                                            \
        TDataModel:: template add<ClassName>();             \
    }                                                       \
}

//! @brief Call the declare method of the given class.
#define OBJECT_BOX_DECLARE(ClassName) box::ClassName::declare<TDataModel>()

namespace kiwi { namespace model {
    
    namespace box
    {
        OBJECT_BOX(NewBox);
        OBJECT_BOX(ErrorBox);
    }
    
}}

namespace kiwi { namespace model { namespace boxes {
    
    // declare all objects defined
    template<class TDataModel>
    void declare()
    {
        OBJECT_BOX_DECLARE(NewBox);
        OBJECT_BOX_DECLARE(ErrorBox);
    }
    
}}}

#undef OBJECT_BOX_DECLARE
#undef OBJECT_BOX
#undef OBJECTBOX_MODEL_NAME
#undef OBJECTBOX_MODEL_NAME_PREFIX
#undef KIWI_STRINGIFY
#undef KIWI_STRINGIFY_MACRO_HELPER

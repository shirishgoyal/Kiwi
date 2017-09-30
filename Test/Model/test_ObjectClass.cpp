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

#include "../catch.hpp"

#include "flip/Document.h"

#include <KiwiModel/Kiwi_DataModel.h>
#include <KiwiModel/Kiwi_Patcher.h>
#include <KiwiModel/Kiwi_PatcherRoot.h>
#include <KiwiModel/Kiwi_Atom.h>
#include <KiwiModel/Kiwi_Factory.h>

#include <KiwiModel/Kiwi_Objects/Kiwi_Objects.h>

using namespace kiwi;

// ==================================================================================== //
//                                      OBJECT CLASS                                    //
// ==================================================================================== //

TEST_CASE("Model ObjectClass", "[Factory]")
{
    SECTION("ObjectClass test")
    {
        flip::Document document (model::DataModel::use(), 123456789ULL, 'cicm', 'test');
        
        auto& patcher = document.root<model::PatcherRoot>().usePatcher();
        
        auto* plus_class = model::Factory::getClassByName("+");
        
        REQUIRE(plus_class);
        
        CHECK(plus_class->getClassName() == "plus");
        
        auto& box_1 = patcher.addObject(model::Factory::create({"+"}));
        
        CHECK(box_1.getClass() == plus_class);
        CHECK(box_1.getClassName() == "plus");
        
        auto box_uptr = model::Factory::create({"+"});
        
        CHECK(box_uptr->getClass() == plus_class);
        CHECK(box_uptr->getClassName() == "plus");
        
        auto& box_2 = patcher.addObject(std::move(box_uptr));
        
        CHECK(box_2.getClass() == plus_class);
        CHECK(box_2.getClassName() == "plus");
        
        auto& class_base = *box_2.getClass();
        flip::Mold mold(model::DataModel::use());
        class_base.moldMake(box_2, mold);
        auto mold_box_uptr = class_base.moldCast(mold);
        
        CHECK(mold_box_uptr->getClassName() == "plus");
        
        auto& box_3 = patcher.addObject(std::move(mold_box_uptr));
        
        CHECK(box_3.getClass() == plus_class);
        CHECK(box_3.getClassName() == "plus");
        
        auto& box_4 = patcher.addObject(plus_class->create("+", {10}));
        
        CHECK(box_4.getTypedName() == "+");
        CHECK(box_4.getNumberOfInlets() == 1);
        
        CHECK(box_4.getClass() == plus_class);
        CHECK(box_4.getClassName() == "plus");
        
        document.commit();
        
        auto& boxes = patcher.getObjects();
        auto& first_box = *boxes.begin();
        
        CHECK(first_box.getClass() == plus_class);
        CHECK(first_box.getClassName() == "plus");
    }
}

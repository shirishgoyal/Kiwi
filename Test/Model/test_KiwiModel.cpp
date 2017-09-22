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

#include <KiwiModel/KiwiModel.h>

#include "flip/DataModel.h"
#include "flip/Document.h"

using namespace kiwi::model;

// ==================================================================================== //
//                                  KIWI MODEL TEST                                     //
// ==================================================================================== //

TEST_CASE("Model test", "[Model]")
{
    struct Model : public DataModel {};
    
    Model::version("model-test");
    Model::declare();
    Model::use();
    
    using namespace box;
    
    //std::cout << "newbox name: " << flip::Class<NewBox>::use().name() << '\n';
    
    flip::Document document(Model::use(), 123456789UL, 'appl', 'test');
    
    auto& patcher = document.root<Patcher>();
    
    patcher.addObjectBox(std::make_unique<NewBox>());
    patcher.addObjectBox(std::make_unique<ErrorBox>());
    document.commit();
    
    auto& boxes = patcher.getObjectBoxes();
    CHECK(boxes.count_if([](auto&){return true;}) == 2);
    
    auto& user = patcher.useSelfUser();
    user.addView();
    document.commit();
    
    auto& user_views = user.getViews();
    
    CHECK(user_views.count_if([](auto&){return true;}) == 1);
}

TEST_CASE("Model test clone", "[Model]")
{
    struct Model : public flip::DataModel<Model> {};
    
    Model::clone_from<DataModel>();
    Model::set_root<Patcher>();
    Model::use();
    
    using namespace box;
    
    flip::Document document(Model::use(), 123456789UL, 'appl', 'test');
    
    auto& patcher = document.root<Patcher>();
    
    patcher.addObjectBox(std::make_unique<NewBox>());
    patcher.addObjectBox(std::make_unique<ErrorBox>());
    document.commit();
    
    auto& boxes = patcher.getObjectBoxes();
    CHECK(boxes.count_if([](auto&){return true;}) == 2);
    
    auto& user = patcher.useSelfUser();
    user.addView();
    document.commit();
    
    auto& user_views = user.getViews();
    
    CHECK(user_views.count_if([](auto&){return true;}) == 1);
}

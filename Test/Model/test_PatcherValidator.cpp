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

#include <KiwiModel/Kiwi_Patcher.h>
#include <KiwiModel/Kiwi_PatcherDataModel.h>
#include <KiwiModel/Kiwi_PatcherValidator.h>

#include "flip/Document.h"
#include "flip/DocumentServer.h"
#include "flip/PortDirect.h"
#include "flip/CarrierDirect.h"

// ==================================================================================== //
//                           PATCHER DOCUMENT MODEL VALIDATOR                           //
// ==================================================================================== //

namespace kiwi { namespace model { namespace tests {
    
    auto make_plus_box = []() {
        
        auto box = std::make_unique<model::ObjectBox>();
        
        box->addInlet({PinType::IType::Control});
        box->addInlet({PinType::IType::Control});
        box->addOutlet(PinType::IType::Control);
        
        return box;
    };
    
    TEST_CASE("Model Validator", "[Validator]")
    {
        using Model = model::PatcherDataModel;
        
        model::PatcherValidator validator;
        flip::DocumentServer server(Model::use(), validator, 123456789ULL);
        
        // make a patcher with two object
        auto& patcher_serveur = server.root<PatcherRoot>().usePatcher();
        patcher_serveur.addObjectBox(make_plus_box());
        patcher_serveur.addObjectBox(make_plus_box());
        
        REQUIRE(patcher_serveur.getObjectBoxes().count_if([](auto&){return true;}) == 2);
        
        server.commit();
        
        // Connect client 1
        flip::PortDirect port_01;
        server.port_factory_add(port_01);
        
        flip::Document document_01(Model::use(), validator, 11, 'cicm', 'test');
        flip::CarrierDirect carrier_01(document_01, port_01);
        
        document_01.pull();
        
        // Connect client 2
        flip::PortDirect port_02;
        server.port_factory_add(port_02);
        
        flip::Document document_02(Model::use(), validator, 12, 23, 24);
        flip::CarrierDirect carrier_02(document_02, port_02);
        
        document_02.pull();
        
        // client 1 delete the two objects;
        Patcher& patcher_01 = document_01.root<PatcherRoot>().usePatcher();
        auto& p1_boxes = patcher_01.getObjectBoxes();
        
        CHECK(p1_boxes.count_if([](auto&){return true;}) == 2);
        
        auto box_it = p1_boxes.begin();
        
        REQUIRE(box_it != p1_boxes.end());
        REQUIRE(box_it->m_inlets.count_if([](auto){return true;}) == 2);
        
        box_it = p1_boxes.erase(box_it);
        box_it = p1_boxes.erase(box_it);
        
        REQUIRE(box_it == p1_boxes.end());
        
        CHECK(patcher_01.getObjectBoxes().count_if([](auto const&){return true;}) == 2);
        
        // client 2 create link
        Patcher& patcher_02 = document_02.root<PatcherRoot>().usePatcher();
        auto& p2_boxes = patcher_01.getObjectBoxes();
        
        ObjectBox& link_from = *p2_boxes.begin();
        ObjectBox& link_to = *(++p2_boxes.begin());
        
        patcher_02.addLink(std::make_unique<Link>(link_from, 0, link_to, 0));
        
        SECTION("Concurrent object deletion and link creation (deletion first)")
        {
            CHECK(patcher_serveur.getObjectBoxes().count_if([](auto const&){return true;}) == 2);
            
            // client 1 commit and push
            document_01.commit();
            document_01.push();
            
            CHECK(patcher_serveur.getObjectBoxes().count_if([](auto const&){return true;}) == 0);
            
            CHECK(patcher_serveur.getLinks().count_if([](auto&){return true;}) == 0);
            
            // client 2 commit and push (server reject link creation)
            document_02.commit();
            document_02.push();
            
            CHECK(patcher_serveur.getLinks().count_if([](auto&){return true;}) == 0);
            
            // client 1 pull
            document_01.pull();
            CHECK(patcher_01.getObjectBoxes().count_if([](auto&){return true;}) == 0);
            CHECK(patcher_01.getLinks().count_if([](auto&){return true;}) == 0);
            
            // client 2 pull (stack unwind plus link creation rejected)
            document_02.pull();
            CHECK(patcher_02.getObjectBoxes().count_if([](auto&){return true;}) == 0);
            CHECK(patcher_02.getLinks().count_if([](auto&){return true;}) == 0);
        }
        
        SECTION("Concurrent object deletion and link creation (creation first)")
        {
            CHECK(patcher_serveur.getLinks().count_if([](auto&){return true;}) == 0);
            
            // client 2 commit and push
            document_02.commit();
            document_02.push();
            
            CHECK(patcher_serveur.getLinks().count_if([](auto&){return true;}) == 1);
            
            // client 1 commit and push (transaction rejected)
            document_01.commit();
            document_01.push();
            
            CHECK(patcher_serveur.getObjectBoxes().count_if([](auto&){return true;}) == 2);
            
            // client 1 pull (stack unwind plus deletion rejected)
            document_01.pull();
            CHECK(patcher_01.getObjectBoxes().count_if([](auto&){return true;}) == 2);
            CHECK(patcher_01.getLinks().count_if([](auto&){return true;}) == 1);
            
            // client 2 pull
            document_02.pull();
            CHECK(patcher_02.getObjectBoxes().count_if([](auto&){return true;}) == 2);
            CHECK(patcher_02.getLinks().count_if([](model::Link &){return true;}) == 1);
        }
        
        server.port_factory_remove(port_01);
        server.port_factory_remove(port_02);
    }
    
}}}

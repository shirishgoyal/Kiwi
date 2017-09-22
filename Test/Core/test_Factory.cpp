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

/*
#include "flip/Document.h"
#include "flip/DocumentServer.h"
#include "flip/PortDirect.h"
#include "flip/CarrierDirect.h"

#include <KiwiCore/KiwiPatcher/Kiwi_PatcherUser.h>
#include <KiwiCore/KiwiDocument/Kiwi_PatcherValidator.h>
#include <KiwiCore/KiwiFactory/Kiwi_Factory.h>

using namespace kiwi;

// ==================================================================================== //
//                                          VALIDATOR                                   //
// ==================================================================================== //

void printStringVec(std::vector<std::string> const& strings){
    
    for (auto str : strings) {
        std::cout << "* - " << str << + "\n";
    }
    
    std::cout << "\n";
}

TEST_CASE("Dummy DataModel version test", "[Model][Factory]")
{
    struct Model : public model::Factory<Model>
    {
        static void init(std::string const& version)
        {
            DataModel::version(version);
        }
    };
    
    const std::string version = "test";
    Model::init(version);
    
    CHECK(Model::use().version() == version);
}

// ------ 

TEST_CASE("Model Factory 3", "[Model][Factory]")
{
    struct Model : public model::Factory<Model>
    {
        static void init(std::string const& version)
        {
            DataModel::version(version);
            
            add<model::ModelBase>();
            add<model::Link>();
        }
    };
    
    const std::string version = "test";
    Model::init(version);
    
    flip::Document document(Model::use(), 123456789UL, 'appl', 'test');
    
    auto& link = document.root<model::Link>();
    
    //CHECK(link.getClass()->getName() == "Link");
    
    auto link_class = Model::getClass(link);
    
    CHECK(link_class->getName() == "Link");
    
    auto names = Model::getNames(false, false);
    printStringVec(names);
}
*/

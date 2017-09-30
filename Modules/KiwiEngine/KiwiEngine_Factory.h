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

#include "flip/Class.h"

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                   OBJECT FACTORY                                 //
        // ================================================================================ //
        
        //! @brief The engine Object's factory
        class Factory
        {
        public: // methods
            
            //! @brief Adds an object engine into the Factory.
            //! @details This function adds a new object engine to the factory.
            //! If the name of the object already exists, the function does nothing,
            //! otherwise the object is added to the factory.
            //! @param name The name of the object.
            template <class TModel, class TEngine>
            static void add()
            {
                static_assert(std::is_base_of<model::Object, TModel>::value,
                              "TModel class must be a model::Object child class.");
                
                static_assert(std::is_base_of<Object, TEngine>::value,
                              "TEngine class must be an engine::Object child class.");
                
                static_assert(!std::is_abstract<TEngine>::value,
                              "The engine object must not be abstract.");
                
                static_assert(std::is_constructible<TEngine,
                              TModel const&, Patcher&, std::vector<Atom> const&>::value,
                              "The engine object must have a valid constructor.");
                
                auto const* const flip_class = &flip::Class<TModel>::use();
                
                auto& creators = getCreators();
                assert(creators.count(flip_class) == 0 && "The object already exists");
                
                creators[flip_class] = [](model::Object const& model,
                                         Patcher& patcher,
                                         std::vector<Atom> const& args) -> TEngine*
                {
                    return new TEngine(dynamic_cast<TModel const&>(model), patcher, args);
                };
            }
            
            //! @brief Creates a new engine Object.
            //! @param model The model::Object.
            //! @return An engine object.
            static std::unique_ptr<Object> create(Patcher& patcher, model::Object const& model);
            
        private: // methods
            
            using ctor_fn_t = std::function<Object*(model::Object const& model,
                                                    Patcher& patcher,
                                                    std::vector<Atom> const&)>;
            
            using creator_map_t = std::map<flip::ClassBase const* const, ctor_fn_t>;
            
            //! @internal Returns the static map of creators.
            static creator_map_t& getCreators();
            
        private: // deleted methods
            
            Factory() = delete;
            ~Factory() = delete;
        };
    }
}

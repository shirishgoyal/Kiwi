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

#include <KiwiModel/Kiwi_Objects/Kiwi_Basic/Kiwi_ErrorBox.h>
#include <KiwiModel/Kiwi_Objects/Kiwi_Basic/Kiwi_NewBox.h>

#include <KiwiModel/Kiwi_Factory.h>
#include <KiwiModel/Kiwi_ObjectClass.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    std::unique_ptr<model::Object> Factory::create(std::vector<Atom> const& atoms)
    {
        const auto typed_name = !atoms.empty() ? atoms[0].getString() : "";
        
        if(typed_name.empty())
        {
            static auto const* newbox_class = getClassByName("newbox");
            return newbox_class->create<NewBox>();
        }
        
        auto const* object_class = getClassByName(typed_name);
        
        std::vector<Atom> args(atoms.begin() + 1, atoms.end());
        
        static auto const* errorbox_class = getClassByName("errorbox");
        
        if(object_class == nullptr)
        {
            return errorbox_class->create<ErrorBox>(typed_name, args,
                                                    "object \"" + typed_name + "\" not found");
        }
        
        try
        {
            return object_class->create(typed_name, args);
        }
        catch(std::runtime_error & e)
        {
            return errorbox_class->create<ErrorBox>(typed_name, args, e.what());
        }
        
        assert(true && "this should never happen");
        return {};
    }
    
    std::unique_ptr<model::Object> Factory::create(std::string const& name, flip::Mold const& mold)
    {
        auto const* class_ptr = getClassByName(name);
        if(class_ptr != nullptr)
        {
            return class_ptr->moldCast(mold);
        }
        else
        {
            throw std::runtime_error("Factory can't create object " + name);
        }
    }
    
    void Factory::copy(model::Object const& object, flip::Mold& mold)
    {
        //const auto name = object.getClassName();
        auto const* class_ptr = object.getClass();
        if(class_ptr != nullptr)
        {
            class_ptr->moldMake(object, mold);
        }
        else
        {
            throw std::runtime_error("can't copy the object!");
        }
    }
    
    bool Factory::has(std::string const& name)
    {
        const auto& object_classes = getClasses();
        for(const auto& object_class : object_classes)
        {
            if(object_class->getClassName() == name || object_class->hasAlias(name))
                return true;
        }
        
        return false;
    }
    
    ObjectClassBase* Factory::getClassByName(std::string const& name,
                                             const bool ignore_aliases)
    {
        const auto& object_classes = getClasses();
        for(const auto& object_class : object_classes)
        {
            if(object_class->getClassName() == name || (!ignore_aliases && object_class->hasAlias(name)))
                return object_class.get();
        }
        
        return nullptr;
    }
    
    ObjectClassBase* Factory::getClassByModelName(std::string const& data_model_name,
                                                  const bool ignore_aliases)
    {
        const auto& object_classes = getClasses();
        for(const auto& object_class : object_classes)
        {
            if(object_class->getDataModelName() == data_model_name)
                return object_class.get();
        }
        
        return nullptr;
    }
    
    std::vector<std::string> Factory::getNames(const bool ignore_aliases, const bool ignore_internals)
    {
        auto const& object_classes = getClasses();
        std::vector<std::string> names;
        names.reserve(object_classes.size());
        
        for(const auto& object_class : object_classes)
        {
            if(!object_class->isInternal() || !ignore_internals)
            {
                names.emplace_back(object_class->getClassName());
                
                if(!ignore_aliases && object_class->hasAlias())
                {
                    const auto aliases = object_class->getAliases();
                    names.insert(names.end(), aliases.begin(), aliases.end());
                }
            }
        }
        
        return names;
    }
    
    auto Factory::getClasses() -> object_classes_t&
    {
        static object_classes_t static_object_classes;
        return static_object_classes;
    }
    
}}

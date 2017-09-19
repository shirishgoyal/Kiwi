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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_ErrorBox.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_NewBox.h>

#include "KiwiModel_Factory.h"

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    void Factory::add(std::unique_ptr<Factory::ObjectClassBase> object_class)
    {
        const auto name = object_class->getClassName();
        
        // check if the name match the name of another object in the factory.
        if(has(name))
        {
            throw std::runtime_error("The \"" + name + "\" object already exist");
        }
        
        auto& object_classes = getClasses();
        object_classes.emplace_back(std::move(object_class));
    }
    
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
    
    Factory::ObjectClassBase* Factory::getClassByName(std::string const& name,
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
    
    Factory::ObjectClassBase* Factory::getClassByModelName(std::string const& data_model_name,
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
    
    std::string Factory::sanitizeName(std::string const& name)
    {
        std::string model_name = "cicm.kiwi.object.";
        
        static const std::string valid_chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_.");
        
        for(auto const& c : name)
        {
            if(c == '~')
            {
                model_name.append("_tilde");
                continue;
            }
            
            model_name += (valid_chars.find(c) != std::string::npos) ? c : '_';
        }
        
        return model_name;
    }
    
    auto Factory::getClasses() -> object_classes_t&
    {
        static object_classes_t static_object_classes;
        return static_object_classes;
    }
    
    // ================================================================================ //
    //                             FACTORY OBJECT CLASS BASE                            //
    // ================================================================================ //
    
    Factory::ObjectClassBase::ObjectClassBase(std::string const& name,
                                              std::string const& model_name,
                                              const ctor_fn_t ctor,
                                              const mold_maker_fn_t mold_maker,
                                              const mold_caster_fn_t  mold_caster)
    : m_name(name)
    , m_model_name(model_name)
    , m_ctor(ctor)
    , m_mold_maker(mold_maker)
    , m_mold_caster(mold_caster)
    {
        ;
    }
    
    std::string const& Factory::ObjectClassBase::getClassName() const
    {
        return m_name;
    }
    
    std::string const& Factory::ObjectClassBase::getDataModelName() const
    {
        return m_model_name;
    }
    
    bool Factory::ObjectClassBase::isInternal() const noexcept
    {
        return m_internal;
    }
    
    void Factory::ObjectClassBase::setInternal(const bool is_internal) noexcept
    {
        m_internal = is_internal;
    }
    
    bool Factory::ObjectClassBase::hasAlias() const noexcept
    {
        return !m_aliases.empty();
    }
    
    std::set<std::string> const& Factory::ObjectClassBase::getAliases() const noexcept
    {
        return m_aliases;
    }
    
    bool Factory::ObjectClassBase::hasAlias(std::string const& alias) const noexcept
    {
        return (m_aliases.count(alias) != 0);
    }
    
    void Factory::ObjectClassBase::addAlias(std::string alias)
    {
        if(!Factory::has(alias))
        {
            m_aliases.emplace(std::move(alias));
        }
    }
    
    std::unique_ptr<model::Object> Factory::ObjectClassBase::create(std::vector<Atom> const& args) const
    {
        return create(getClassName(), args);
    }
    
    std::unique_ptr<model::Object> Factory::ObjectClassBase::create(std::string const& typed_name,
                                                                    std::vector<Atom> const& args) const
    {
        auto object = m_ctor(typed_name, args);
        object->m_class = this;
        return object;
    }
    
    void Factory::ObjectClassBase::moldMake(model::Object const& object, flip::Mold& mold) const
    {
        m_mold_maker(object, mold);
    }
    
    std::unique_ptr<model::Object> Factory::ObjectClassBase::moldCast(flip::Mold const& mold) const
    {
        auto object = m_mold_caster(mold);
        object->m_class = this;
        return object;
    }
}}

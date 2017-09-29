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

#include <KiwiModel/KiwiModel_Factory.h>
#include <KiwiModel/KiwiModel_ObjectClass.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_ErrorBox.h>
#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_NewBox.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                             FACTORY OBJECT CLASS BASE                            //
    // ================================================================================ //
    
    ObjectClassBase::ObjectClassBase(std::string const& name,
                                     const ctor_fn_t ctor,
                                     const mold_maker_fn_t mold_maker,
                                     const mold_caster_fn_t  mold_caster)
    : m_name(name)
    , m_model_name(sanitizeName(name))
    , m_ctor(ctor)
    , m_mold_maker(mold_maker)
    , m_mold_caster(mold_caster)
    {
        ;
    }
    
    std::string const& ObjectClassBase::getClassName() const
    {
        return m_name;
    }
    
    std::string const& ObjectClassBase::getDataModelName() const
    {
        return m_model_name;
    }
    
    bool ObjectClassBase::isInternal() const noexcept
    {
        return m_internal;
    }
    
    void ObjectClassBase::setInternal(const bool is_internal) noexcept
    {
        m_internal = is_internal;
    }
    
    bool ObjectClassBase::hasAlias() const noexcept
    {
        return !m_aliases.empty();
    }
    
    std::set<std::string> const& ObjectClassBase::getAliases() const noexcept
    {
        return m_aliases;
    }
    
    bool ObjectClassBase::hasAlias(std::string const& alias) const noexcept
    {
        return (m_aliases.count(alias) != 0);
    }
    
    void ObjectClassBase::addAlias(std::string alias)
    {
        if(!Factory::has(alias))
        {
            m_aliases.emplace(std::move(alias));
        }
    }
    
    std::unique_ptr<model::Object> ObjectClassBase::create(std::vector<Atom> const& args) const
    {
        return create(getClassName(), args);
    }
    
    std::unique_ptr<model::Object> ObjectClassBase::create(std::string const& typed_name,
                                                           std::vector<Atom> const& args) const
    {
        auto object = m_ctor(typed_name, args);
        object->m_class = this;
        return object;
    }
    
    void ObjectClassBase::moldMake(model::Object const& object, flip::Mold& mold) const
    {
        m_mold_maker(object, mold);
    }
    
    std::unique_ptr<model::Object> ObjectClassBase::moldCast(flip::Mold const& mold) const
    {
        auto object = m_mold_caster(mold);
        object->m_class = this;
        return object;
    }
    
    std::string ObjectClassBase::sanitizeName(std::string const& name)
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
}}

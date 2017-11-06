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

#include <KiwiModel/KiwiModel_ObjectClass.h>
#include <KiwiModel/KiwiModel_Object.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      PARAMETER CLASS                             //
    // ================================================================================ //
    
    ParameterClass::ParameterClass(tool::Parameter::Type type):
    m_type(type),
    m_flags()
    {
    }
    
    ParameterClass::~ParameterClass()
    {
    }
    
    void ParameterClass::setFlag(Flag flag)
    {
        m_flags.insert(flag);
    }
    
    bool ParameterClass::hasFlag(Flag flag) const
    {
        return m_flags.find(flag) != m_flags.end();
    }
    
    tool::Parameter::Type ParameterClass::getType() const
    {
        return m_type;
    }
    
    
    // ================================================================================ //
    //                                     OBJECT CLASS                                 //
    // ================================================================================ //
    
    ObjectClass::ObjectClass(std::string const& name, ctor_t ctor):
    m_name(name),
    m_model_name(),
    m_aliases(),
    m_params(),
    m_ctor(ctor),
    m_flags(),
    m_mold_maker(),
    m_mold_caster()
    {
    }
    
    ObjectClass::~ObjectClass()
    {
    }
    
    bool ObjectClass::hasAlias() const noexcept
    {
        return !m_aliases.empty();
    }
    
    std::string ObjectClass::getName() const
    {
        return m_name;
    }
    
    std::string ObjectClass::getModelName() const
    {
        return m_model_name;
    }
    
    std::set<std::string> const& ObjectClass::getAliases() const noexcept
    {
        return m_aliases;
    }
    
    bool ObjectClass::hasAlias(std::string const& alias) const noexcept
    {
        return (m_aliases.count(alias) != 0);
    }
    
    void ObjectClass::addAlias(std::string const& alias)
    {
        m_aliases.insert(alias);
    }
    
    void ObjectClass::addParameter(std::string name, std::unique_ptr<ParameterClass> param_class)
    {
        m_params[name] = std::move(param_class);
    }
    
    std::map<std::string, std::unique_ptr<ParameterClass>> const& ObjectClass::getParameters() const
    {
        return m_params;
    }
    
    void ObjectClass::setFlag(Flag const& flag)
    {
        m_flags.insert(flag);
    }
    
    bool ObjectClass::hasFlag(Flag const& flag) const
    {
        return m_flags.find(flag) != m_flags.end();
    }
    
    void ObjectClass::setMoldMaker(mold_maker_t maker)
    {
        m_mold_maker = maker;
    }
    
    void ObjectClass::setMoldCaster(mold_caster_t caster)
    {
        m_mold_caster = caster;
    }
    
    void ObjectClass::setModelName(std::string const& model_name)
    {
        m_model_name = model_name;
    }
    
    std::unique_ptr<model::Object> ObjectClass::create(std::vector<tool::Atom> const& args) const
    {
        return m_ctor(args);
    }
    
    void ObjectClass::moldMake(model::Object const& object, flip::Mold& mold) const
    {
        m_mold_maker(object, mold);
    }
    std::unique_ptr<model::Object> ObjectClass::moldCast(flip::Mold const& mold) const
    {
        return m_mold_caster(mold);
    }
    
}}

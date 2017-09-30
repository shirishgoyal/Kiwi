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

#include <KiwiModel/Kiwi_Object.h>
#include <KiwiModel/Kiwi_DataModel.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  INLET/OUTLET                                    //
    // ================================================================================ //
    
    PinType::PinType(flip::Default&)
    {
        ;
    }
    
    PinType::PinType(IType type):
    m_type(type)
    {
        
    }
    
    PinType::IType PinType::getType() const
    {
        return !removed() ? m_type.value() : m_type.before();
    }
    
    bool PinType::operator<(PinType const& other) const
    {
        return getType() < other.getType();
    }
    
    bool PinType::operator==(PinType const& other) const
    {
        return !(*this < other) && !(other < *this);
    }
    
    void PinType::declare()
    {
        if(DataModel::has<model::PinType>()) return;
        
        DataModel::declare<PinType::IType>()
        .name("cicm.kiwi.patch.Object.PinType.TypeEnum")
        .enumerator<PinType::IType::Control>("Control")
        .enumerator<PinType::IType::Signal>("Signal");
        
        DataModel::declare<model::PinType>()
        .name("cicm.kiwi.patch.Object.PinType")
        .member<flip::Enum<IType>, &PinType::m_type>("type");
    }
    
    Inlet::Inlet(flip::Default&)
    {
        ;
    }
    
    Inlet::Inlet(std::set<PinType> types):
    m_types()
    {
        m_types.insert(m_types.begin(), types.begin(), types.end());
    }
    
    bool Inlet::hasType(PinType type) const
    {
        return m_types.find_if([type](PinType const& inlet_type){return inlet_type == type; }) != m_types.end();
    }
    
    void Inlet::declare()
    {
        if(DataModel::has<model::Inlet>()) return;
        
        PinType::declare();
        
        DataModel::declare<model::Inlet>()
        .name("cicm.kiwi.patch.Object.Inlet")
        .member<flip::Array<PinType>, &Inlet::m_types>("types");
    };
    
    Outlet::Outlet(flip::Default&):
    m_type(PinType::IType::Control)
    {
        ;
    }
    
    Outlet::Outlet(PinType type):
    m_type(type)
    {
    }
    
    PinType const& Outlet::getType() const
    {
        return m_type;
    }
    
    void Outlet::declare()
    {
        if(DataModel::has<model::Outlet>()) return;
        
        PinType::declare();
        
        DataModel::declare<model::Outlet>()
        .name("cicm.kiwi.patch.Object.Outlet")
        .member<PinType, &Outlet::m_type>("type");
    };
    
    // ================================================================================ //
    //                                  OBJECT::declare                                 //
    // ================================================================================ //
    
    void Object::declare()
    {
        if(DataModel::has<model::Object>()) return;
        
        Outlet::declare();
        Inlet::declare();
        
        DataModel::declare<model::Object>()
        .name("cicm.kiwi.patch.Object")
        .member<flip::String, &Object::m_typed_name>("typed_name")
        .member<flip::String, &Object::m_additional_text>("additional_text")
        .member<flip::Array<Inlet>, &Object::m_inlets>("inlets")
        .member<flip::Array<Outlet>, &Object::m_outlets>("outlets")
        .member<flip::Float, &Object::m_position_x>("pos_x")
        .member<flip::Float, &Object::m_position_y>("pos_y")
        .member<flip::Float, &Object::m_width>("width")
        .member<flip::Float, &Object::m_height>("height");
    }
    
    // ================================================================================ //
    //                                      OBJECT                                      //
    // ================================================================================ //
    
    Object::Object(flip::Default&)
    : m_typed_name()
    , m_additional_text()
    , m_inlets()
    , m_outlets()
    , m_flags()
    , m_position_x(0.)
    , m_position_y(0.)
    , m_width(0.)
    , m_height(0.)
    {
        ;
    }
    
    Object::Object(std::string const& typed_name,
                   std::vector<Atom> const& args)
    : m_typed_name(typed_name)
    , m_additional_text(AtomHelper::toString(args))
    , m_inlets()
    , m_outlets()
    , m_flags()
    , m_position_x(0.)
    , m_position_y(0.)
    , m_width(60.)
    , m_height(20.)
    {
        ;
    }
    
    ObjectClassBase const* Object::getClass() const
    {
        if(m_class == nullptr)
        {
            assert(is_bound() && "The object has not been created by the model::Factory neither by flip...");
            
            m_class = Factory::getClassByModelName(get_class().name());
        }
        
        return m_class;
    }
    
    std::string Object::getClassName() const
    {
        if(const auto* class_ptr = getClass())
        {
            return class_ptr->getClassName();
        }
        
        return {};
    }
    
    std::string const& Object::getTypedName() const
    {
        return m_typed_name;
    }
    
    std::string const& Object::getAdditionalText() const
    {
        return m_additional_text;
    }
    
    std::string Object::getDisplayText() const
    {
        if(getAdditionalText().empty())
        {
            return getTypedName();
        }
        
        return {getTypedName() + " " + getAdditionalText()};
    }
    
    bool Object::isErrorBox() const
    {
        return getClassName() == "errorbox";
    }
    
    flip::Array<Inlet> const& Object::getInlets() const
    {
        return m_inlets;
    }
    
    Inlet const& Object::getInlet(size_t index) const
    {
        flip::Array<Inlet>::const_iterator it = m_inlets.begin();
        std::advance(it, index);
        return *it;
    }
    
    size_t Object::getNumberOfInlets() const
    {
        return m_inlets.count_if([](Inlet const&){return true;});
    }
    
    bool Object::inletsChanged() const noexcept
    {
        return m_inlets.changed();
    }
    
    flip::Array<Outlet> const& Object::getOutlets() const
    {
        return m_outlets;
    }
    
    Outlet const& Object::getOutlet(size_t index) const
    {
        flip::Array<Outlet>::const_iterator it = m_outlets.begin();
        std::advance(it, index);
        return (*it);
    }
    
    size_t Object::getNumberOfOutlets() const
    {
        return m_outlets.count_if([](Outlet const&){return true;});
    }
    
    bool Object::outletsChanged() const noexcept
    {
        return m_outlets.changed();
    }
    
    void Object::setPosition(double x, double y)
    {
        m_position_x = x;
        m_position_y = y;
    }
    
    bool Object::positionChanged() const noexcept
    {
        return (m_position_x.changed() || m_position_y.changed());
    }
    
    bool Object::sizeChanged() const noexcept
    {
        return (m_width.changed() || m_height.changed());
    }
    
    bool Object::boundsChanged() const noexcept
    {
        return (positionChanged() || sizeChanged());
    }
    
    double Object::getX() const noexcept
    {
        return !removed() ? m_position_x.value() : m_position_x.before();
    }
    
    double Object::getY() const noexcept
    {
        return !removed() ? m_position_y.value() : m_position_y.before();
    }
    
    void Object::setWidth(double new_width)
    {
        m_width = std::max(0., new_width);
    }
    
    void Object::setHeight(double new_height)
    {
        m_height = std::max(0., new_height);
    }
    
    double Object::getWidth() const noexcept
    {
        return !removed() ? m_width.value() : m_width.before();
    }
    
    double Object::getHeight() const noexcept
    {
        return !removed() ? m_height.value() : m_height.before();
    }
    
    void Object::setInlets(flip::Array<Inlet> const& inlets)
    {
        m_inlets = inlets;
    }
    
    void Object::setOutlets(flip::Array<Outlet> const& outlets)
    {
        m_outlets = outlets;
    }
    
    void Object::pushInlet(std::set<PinType> types)
    {
        m_inlets.emplace(m_inlets.end(), types);
    }
    
    void Object::pushOutlet(PinType type)
    {
        m_outlets.emplace(m_outlets.end(), type);
    }
    
    std::string Object::getIODescription(bool is_inlet, size_t index) const
    {
        return (is_inlet ? "inlet " : "outlet ") + std::to_string(index);
    }
    
    bool Object::hasFlag(Flag flag) const
    {
        return (static_cast<unsigned int>(flag) & static_cast<unsigned int>(m_flags.value())) != 0;
    }
    
    void Object::setFlag(Flag flag)
    {
        m_flags = static_cast<Flag>(static_cast<unsigned int>(flag) | static_cast<unsigned int>(m_flags.value()));
    }
    
}}
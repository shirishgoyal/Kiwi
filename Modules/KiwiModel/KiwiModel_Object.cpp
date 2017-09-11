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

#include "KiwiModel_Object.h"

#include "KiwiModel_DataModel.h"

namespace kiwi
{
    namespace model
    {
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
            .name("cicm.kiwi.Type")
            .enumerator<PinType::IType::Control>("Control")
            .enumerator<PinType::IType::Signal>("Signal");
            
            DataModel::declare<model::PinType>()
            .name("cicm.kiwi.PinType")
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
            .name("cicm.kiwi.Inlet")
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
            .name("cicm.kiwi.Outlet")
            .member<PinType, &Outlet::m_type>("type");
        };
        
        // ================================================================================ //
        //                                  FLAG                                            //
        // ================================================================================ //
        
        Flag::Flag(Flag::IFlag flag):
        flip::Object(),
        m_flag(flag)
        {
        }
        
        Flag::Flag(flip::Default& d):
        flip::Object()
        {
        }
        
        void Flag::declare()
        {
            if(DataModel::has<model::Flag>()) return;
            
            DataModel::declare<Flag::IFlag>()
            .name("cicm.kiwi.iflag")
            .enumerator<Flag::IFlag::DefinedSize>("DefinedSize")
            .enumerator<Flag::IFlag::ResizeWidth>("ResizeWidth")
            .enumerator<Flag::IFlag::ResizeHeight>("ResizeHeight");
            
            DataModel::declare<model::Flag>()
            .name("cicm.kiwi.flag")
            .member<flip::Enum<IFlag>, &Flag::m_flag>("flag");
        }
        
        bool Flag::operator==(Flag const& other) const
        {
            return m_flag.value() == other.m_flag.value();
        }
        
        bool Flag::operator!=(Flag const& other) const
        {
            return !this->operator==(other);
        }
        
        // ================================================================================ //
        //                                  OBJECT::declare                                 //
        // ================================================================================ //
        
        void Object::declare()
        {
            if(DataModel::has<model::Object>()) return;
            
            Outlet::declare();
            Inlet::declare();
            Flag::declare();
            
            DataModel::declare<model::Object>()
            .name("cicm.kiwi.Object")
            .member<flip::String, &Object::m_name>("name")
            .member<flip::String, &Object::m_text>("text")
            .member<flip::Array<Inlet>, &Object::m_inlets>("inlets")
            .member<flip::Array<Outlet>, &Object::m_outlets>("outlets")
            .member<flip::Collection<Flag>, &Object::m_flags>("flags")
            .member<flip::Float, &Object::m_position_x>("pos_x")
            .member<flip::Float, &Object::m_position_y>("pos_y")
            .member<flip::Float, &Object::m_width>("width")
            .member<flip::Float, &Object::m_height>("height")
            .member<flip::Float, &Object::m_min_width>("min_width")
            .member<flip::Float, &Object::m_min_height>("min_height")
            .member<flip::Float, &Object::m_ratio>("ratio");
        }
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object(flip::Default&)
        {
            ;
        }
        
        Object::Object() :
        m_name("noobj"),
        m_inlets(),
        m_outlets(),
        m_flags(),
        m_position_x(0.),
        m_position_y(0.),
        m_width(60.),
        m_height(20.),
        m_min_width(0.),
        m_min_height(0.),
        m_ratio(0.)
        {
            ;
        }
        
        std::string Object::getName() const
        {
            return m_name;
        }
        
        std::string Object::getText() const
        {
            return m_text;
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
        
        void Object::setRatio(double ratio)
        {
            if (ratio > 0.)
            {
                m_ratio = ratio;
                m_height = m_width * m_ratio;
                m_min_height = m_min_width * m_ratio;
            }
        }
        
        double Object::getRatio() const
        {
            return m_ratio;
        }
        
        void Object::setMinWidth(double min_width)
        {
            if (min_width >= 0.)
            {
                m_min_width = min_width;
                
                if (m_ratio > 0.)
                {
                    m_min_height = m_min_width * m_ratio;
                }
                
                setWidth(getWidth());
            }
        }
        
        void Object::setMinHeight(double min_height)
        {
            if (min_height >= 0.)
            {
                m_min_height = min_height;
                
                if (m_ratio > 0.)
                {
                    m_min_width = m_min_height / m_ratio;
                }
                
                setHeight(getHeight());
            }
        }
        
        void Object::setWidth(double new_width)
        {
            m_width = std::max(m_min_width.value(), new_width);
            
            if (m_ratio > 0.)
            {
                m_height = m_ratio * m_width;
            }
        }
        
        void Object::setHeight(double new_height)
        {
            m_height = std::max(m_min_height.value(), new_height);
            
            if (m_ratio > 0.)
            {
                m_width = m_height / m_ratio;
            }
        }
        
        double Object::getMinWidth() const noexcept
        {
            return m_min_width.value();
        }
        
        double Object::getMinHeight() const noexcept
        {
            return m_min_height.value();
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
            return m_flags.count_if([&flag](Flag const& internal_flag) { return internal_flag == flag;}) != 0;
        }
        
        void Object::setFlag(Flag flag)
        {
            if (!hasFlag(flag))
            {
                m_flags.insert(flag);
            }
        }
    }
}

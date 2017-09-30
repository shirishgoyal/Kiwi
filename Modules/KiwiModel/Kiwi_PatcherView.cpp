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

#include <KiwiModel/Kiwi_PatcherView.h>
#include <KiwiModel/Kiwi_Patcher.h>
#include <KiwiModel/Kiwi_DataModel.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  PATCHER::declare                                //
    // ================================================================================ //
    
    void PatcherView::Object::declare()
    {
        assert(! DataModel::has<PatcherView::Object>());
        
        DataModel::declare<PatcherView::Object>()
        .name("cicm.kiwi.Patcher.View.Object")
        .member<flip::ObjectRef<model::Object>, &PatcherView::Object::m_ref>("ref");
    }
    
    void PatcherView::Link::declare()
    {
        assert(! DataModel::has<PatcherView::Link>());
        
        DataModel::declare<PatcherView::Link>()
        .name("cicm.kiwi.Patcher.View.Link")
        .member<flip::ObjectRef<model::Link>, &PatcherView::Link::m_ref>("ref");
    }
    
    void PatcherView::declare()
    {
        assert(! DataModel::has<PatcherView>());
        
        PatcherView::Object::declare();
        PatcherView::Link::declare();
        
        DataModel::declare<PatcherView>()
        .name("cicm.kiwi.Patcher.View")
        .member<flip::Collection<PatcherView::Object>, &PatcherView::m_selected_objects>("selected_objects")
        .member<flip::Collection<PatcherView::Link>, &PatcherView::m_selected_links>("selected_links")
        .member<flip::Bool, &PatcherView::m_is_locked>("locked")
        .member<flip::Float, &PatcherView::m_zoom_factor>("zoom_factor");
    }
    
    // ================================================================================ //
    //                                   PATCHER VIEW                                   //
    // ================================================================================ //
    
    PatcherView::PatcherView()
    : m_is_locked(false)
    , m_zoom_factor(1.)
    {
        ;
    }
    
    PatcherView::~PatcherView()
    {
        m_selected_links.clear();
        m_selected_objects.clear();
    }
    
    Patcher& PatcherView::getPatcher()
    {
        assert(is_bound() && "The PatcherView has not yet been added to a Patcher");
        
        return ancestor<Patcher>();
    }
    
    void PatcherView::setLock(bool locked)
    {
        m_is_locked = locked;
    }
    
    bool PatcherView::getLock() const noexcept
    {
        return m_is_locked;
    }
    
    bool PatcherView::lockChanged() const noexcept
    {
        return m_is_locked.changed();
    }
    
    void PatcherView::setZoomFactor(double zoom_factor)
    {
        const double min_zoom = 0.25;
        const double max_zoom = 4.;
        m_zoom_factor = (zoom_factor < min_zoom ? min_zoom
                         : (zoom_factor > max_zoom ? max_zoom : zoom_factor));
    }
    
    double PatcherView::getZoomFactor() const noexcept
    {
        return m_zoom_factor;
    }
    
    bool PatcherView::zoomFactorChanged() const noexcept
    {
        return m_zoom_factor.changed();
    }
    
    std::vector<model::Object*> PatcherView::getSelectedObjects()
    {
        std::vector<model::Object*> objects;
        for(auto& object : m_selected_objects)
        {
            objects.push_back(object.get());
        }
        
        return objects;
    }
    
    std::vector<model::Link*> PatcherView::getSelectedLinks()
    {
        std::vector<model::Link*> links;
        for(auto& link : m_selected_links)
        {
            links.push_back(link.get());
        }
        
        return links;
    }
    
    bool PatcherView::isSelected(model::Object const& object) const
    {
        for(auto& ref : m_selected_objects)
        {
            if(!ref.removed())
            {
                auto* sel_object = ref.get();
                
                if(sel_object != nullptr && sel_object == &object) return true;
            }
        }
        
        return false;
    }
    
    bool PatcherView::isSelected(model::Link const& link) const
    {
        for(auto& ref : m_selected_links)
        {
            if(!ref.removed())
            {
                auto* sel_link = ref.get();
                
                if(sel_link != nullptr && sel_link == &link) return true;
            }
        }
        
        return false;
    }
    
    bool PatcherView::selectionChanged() const
    {
        return (m_selected_objects.changed() || m_selected_links.changed());
    }
    
    void PatcherView::selectObject(model::Object& object)
    {
        if(!isSelected(object))
        {
            m_selected_objects.emplace(object);
        }
    }
    
    void PatcherView::selectLink(model::Link& link)
    {
        if(!isSelected(link))
        {
            m_selected_links.emplace(link);
        }
    }
    
    void PatcherView::unselectObject(model::Object& object)
    {
        const auto find_object = [&object] (PatcherView::Object& ref)
        {
            if(!ref.removed())
            {
                auto* sel_object = ref.get();
                
                if(sel_object != nullptr && sel_object == &object) return true;
            }
            
            return false;
        };
        
        auto it = std::find_if(m_selected_objects.begin(), m_selected_objects.end(), find_object);
        
        if(it != m_selected_objects.end())
        {
            m_selected_objects.erase(it);
        }
    }
    
    void PatcherView::unselectLink(model::Link& link)
    {
        const auto find_link = [&link] (PatcherView::Link& ref)
        {
            if(!ref.removed())
            {
                auto* sel_link = ref.get();
                
                if(sel_link != nullptr && sel_link == &link) return true;
            }
            
            return false;
        };
        
        auto it = std::find_if(m_selected_links.begin(), m_selected_links.end(), find_link);
        
        if(it != m_selected_links.end())
        {
            m_selected_links.erase(it);
        }
    }
    
    void PatcherView::unselectAll()
    {
        m_selected_links.clear();
        m_selected_objects.clear();
    }
    
    void PatcherView::selectAll()
    {
        unselectAll();
        
        auto& patcher = getPatcher();
        
        for(auto& object : patcher.getObjects())
        {
            if(!object.removed())
            {
                m_selected_objects.emplace(object);
            }
        }
        
        for(auto& link : patcher.getLinks())
        {
            if(!link.removed())
            {
                m_selected_links.emplace(link);
            }
        }
    }
    
    // ================================================================================ //
    //                                PATCHER VIEW OBJECT                               //
    // ================================================================================ //
    
    PatcherView::Object::Object(model::Object& object) : m_ref(&object)
    {
        ;
    }
    
    model::Object* PatcherView::Object::get() const
    {
        return m_ref.value();
    }
    
    // ================================================================================ //
    //                                 PATCHER VIEW LINK                                //
    // ================================================================================ //
    
    PatcherView::Link::Link(model::Link& link) : m_ref(&link)
    {
        ;
    }
    
    model::Link* PatcherView::Link::get() const
    {
        return m_ref.value();
    }
    
}}

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
#include <KiwiModel/Kiwi_Link.h>
#include <KiwiModel/Kiwi_ObjectBox.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                               PATCHER VIEW MODEL                                 //
    // ================================================================================ //
    
    PatcherView::PatcherView(flip::Default&)
    {
        ;
    }
    
    PatcherView::PatcherView()
    : m_is_locked(false)
    , m_zoom_factor(1.)
    {
        ;
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
        m_zoom_factor = zoom_factor < min_zoom ? min_zoom : zoom_factor;
    }
    
    double PatcherView::getZoomFactor() const noexcept
    {
        return m_zoom_factor;
    }
    
    bool PatcherView::zoomFactorChanged() const noexcept
    {
        return m_zoom_factor.changed();
    }
    
    std::vector<ObjectBox*> PatcherView::getSelectedObjectBoxes()
    {
        std::vector<ObjectBox*> objects;
        for(auto& object : m_selected_objects)
        {
            objects.push_back(object.get());
        }
        
        return objects;
    }
    
    std::vector<Link*> PatcherView::getSelectedLinks()
    {
        std::vector<Link*> links;
        for(auto& link : m_selected_links)
        {
            links.push_back(link.get());
        }
        
        return links;
    }
    
    bool PatcherView::isSelected(ObjectBox const& object) const
    {
        for(auto& ref : m_selected_objects)
        {
            //if(auto* ptr = document().object_ptr<ObjectBox>(ref))
            
            if(!ref.removed())
            {
                auto* sel_object = ref.get();
                
                if(sel_object != nullptr && sel_object == &object) return true;
            }
        }
        
        return false;
    }
    
    bool PatcherView::isSelected(Link const& link) const
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
    
    void PatcherView::selectObject(ObjectBox& object)
    {
        if(!isSelected(object))
        {
            m_selected_objects.emplace(object);
        }
    }
    
    void PatcherView::selectLink(Link& link)
    {
        if(!isSelected(link))
        {
            m_selected_links.emplace(link);
        }
    }
    
    void PatcherView::unselectObject(ObjectBox& object)
    {
        const auto find_object = [&object] (objectbox_view_t& ref)
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
    
    void PatcherView::unselectLink(Link& link)
    {
        const auto find_link = [&link] (link_view_t& ref)
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
        /*
        unselectAll();
        
        auto& patcher = getPatcher();
        
        for(auto& object : patcher.getObjectBoxes())
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
        */
    }
    
    // ================================================================================ //
    //                                PATCHER VIEW OBJECT                               //
    // ================================================================================ //
    
    PatcherView::ObjectBoxView::ObjectBoxView(ObjectBox& object)
    : m_ref(&object)
    {
        ;
    }
    
    ObjectBox* PatcherView::ObjectBoxView::get() const
    {
        return m_ref.value();
    }
    
    // ================================================================================ //
    //                                 PATCHER VIEW LINK                                //
    // ================================================================================ //
    
    PatcherView::LinkView::LinkView(Link& link)
    : m_ref(&link)
    {
        ;
    }
    
    Link* PatcherView::LinkView::get() const
    {
        return m_ref.value();
    }
}}

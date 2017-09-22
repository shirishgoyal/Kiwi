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

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                               PATCHER VIEW MODEL                                 //
    // ================================================================================ //
    
    template<class TDataModel>
    void PatcherView::ObjectBoxView::declare()
    {
        flip::Class<PatcherView::ObjectBoxView>::declare()
        .name("cicm.kiwi.patch.PatcherView.ObjectView")
        .member<flip::ObjectRef<ObjectBox>, &PatcherView::ObjectBoxView::m_ref>("ref");
    }
    
    template<class TDataModel>
    void PatcherView::LinkView::declare()
    {
        flip::Class<PatcherView::LinkView>::declare()
        .name("cicm.kiwi.patch.PatcherView.LinkView")
        .member<flip::ObjectRef<Link>, &PatcherView::LinkView::m_ref>("ref");
    }
    
    template<class TDataModel>
    void PatcherView::declare()
    {
        PatcherView::ObjectBoxView::declare<TDataModel>();
        TDataModel::template add<PatcherView::ObjectBoxView>();
        
        PatcherView::LinkView::declare<TDataModel>();
        TDataModel::template add<PatcherView::LinkView>();
        
        flip::Class<PatcherView>::declare()
        .name("cicm.kiwi.patch.PatcherView")
        .member<object_selection_t, &PatcherView::m_selected_objects>("object_selection")
        .member<link_selection_t, &PatcherView::m_selected_links>("link_selection")
        .member<flip::Bool, &PatcherView::m_is_locked>("locked")
        .member<flip::Float, &PatcherView::m_zoom_factor>("zoom_factor");
        
        TDataModel::template add<PatcherView>();
    }
    
}}

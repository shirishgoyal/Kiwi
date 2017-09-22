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

#include <KiwiModel/Kiwi_ObjectBase.h>

#include "flip/Bool.h"
#include "flip/Float.h"
#include "flip/Collection.h"
#include "flip/ObjectRef.h"

namespace kiwi { namespace model {
    
    class Link;
    class ObjectBox;
    
    // ================================================================================ //
    //                               PATCHER VIEW MODEL                                 //
    // ================================================================================ //
    
    //! @brief The PatcherView Model class holds the informations about a view of a Patcher.
    class PatcherView : public ObjectBase
    {
    public: // methods
        
        //! @internal flip Default constructor.
        PatcherView(flip::Default&);
        
        //! @brief Constructor.
        PatcherView();
        
        //! @brief Destructor.
        ~PatcherView() = default;
        
        //! @brief Set the lock status.
        void setLock(bool locked);
        
        //! @brief Returns true if the view is locked.
        bool getLock() const noexcept;
        
        //! @brief Returns true if the lock status changed.
        bool lockChanged() const noexcept;
        
        //! @brief Set zoom factor.
        void setZoomFactor(double zoom_factor);
        
        //! @brief Returns the current zoom factor.
        double getZoomFactor() const noexcept;
        
        //! @brief Returns true if the zoom factor changed.
        bool zoomFactorChanged() const noexcept;
        
        // ================================================================================ //
        //                                   SELECTION                                      //
        // ================================================================================ //
        
        //! @brief Return the selected ObjectBoxes.
        std::vector<ObjectBox*> getSelectedObjectBoxes();
        
        //! @brief Return the selected Links.
        std::vector<Link*> getSelectedLinks();
        
        //! @brief Return true if the given Object is selected in this view.
        bool isSelected(ObjectBox const& object) const;
        
        //! @brief Return true if the given Link is selected in this view.
        bool isSelected(Link const& link) const;
        
        //! @brief Returns true if selection has changed.
        bool selectionChanged() const;
        
        //! @brief Select an Object.
        void selectObject(ObjectBox& object);
        
        //! @brief Select a Link.
        void selectLink(Link& object);
        
        //! @brief Unselect an Object.
        void unselectObject(ObjectBox& object);
        
        //! @brief Unselect a Link.
        void unselectLink(Link& object);
        
        //! @brief Unselect all objects and links
        void unselectAll();
        
        //! @brief Select all objects and links
        void selectAll();
        
    public: // internal methods
        
        //! @brief flip class declaration
        template<class TDataModel>
        static void declare();
        
    private: // nested classes
        
        struct ObjectBoxView;
        struct LinkView;
        
    private: // members
        
        using objectbox_view_t = PatcherView::ObjectBoxView;
        using link_view_t = PatcherView::LinkView;
        
        using object_selection_t = flip::Collection<objectbox_view_t>;
        using link_selection_t = flip::Collection<link_view_t>;
        
        object_selection_t  m_selected_objects;
        link_selection_t    m_selected_links;
        flip::Bool          m_is_locked;
        flip::Float         m_zoom_factor;
    };
    
    // ================================================================================ //
    //                                PATCHER VIEW OBJECT                               //
    // ================================================================================ //
    
    //! @internal An ObjectBox reference wrapper.
    struct PatcherView::ObjectBoxView : public flip::Object
    {
    public: // methods
        
        ObjectBoxView() = default;
        ~ObjectBoxView() = default;
        ObjectBoxView(ObjectBox& object);
        ObjectBox* get() const;
        
    private: // properties
        
        flip::ObjectRef<ObjectBox> m_ref;
        
    private: // declaration
        
        //! @internal class declarator (called by PatcherView::declare).
        template<class TDataModel>
        static void declare();
        
        template<class TDataModel>
        friend void PatcherView::declare();
    };
    
    // ================================================================================ //
    //                                 PATCHER VIEW LINK                                //
    // ================================================================================ //
    
    //! @internal A Link reference wrapper.
    struct PatcherView::LinkView : public flip::Object
    {
    public: // methods
        
        LinkView() = default;
        ~LinkView() = default;
        LinkView(Link& link);
        Link* get() const;
        
    private: // properties
        
        flip::ObjectRef<Link> m_ref;
        
    private: // declaration
        
        //! @internal class declarator (called by PatcherView::declare).
        template<class TDataModel>
        static void declare();
        
        template<class TDataModel>
        friend void PatcherView::declare();
    };
}}

#include <KiwiModel/Kiwi_PatcherView.hpp>

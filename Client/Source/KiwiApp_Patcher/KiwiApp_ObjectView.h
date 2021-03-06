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

#include <KiwiModel/KiwiModel_PatcherUser.h>

#include <juce_gui_basics/juce_gui_basics.h>

#include "../KiwiApp_Components/KiwiApp_SuggestEditor.h"

namespace kiwi
{
    class HitTester;
    class PatcherView;
    class ClassicBox;
    
    // ================================================================================ //
    //                                     OBJECT VIEW                                  //
    // ================================================================================ //
    
    //! @brief The juce object Component.
    class ObjectView : public juce::Component
    {
    public:
        
        ObjectView(PatcherView& patcher_view, model::Object& object_m);
        ~ObjectView();
        
        void objectChanged(model::Patcher::View& view, model::Object& object);
        void localSelectionChanged(bool selected_for_view);
        void distantSelectionChanged(std::set<uint64_t> distant_user_id_selection);
        void lockStatusChanged(bool locked);
        void patcherViewOriginPositionChanged();
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        void mouseDrag(juce::MouseEvent const& event) override;
        
        //! @brief Returns The box bounds relative to the parent Component
        juce::Rectangle<int> getBoxBounds() const;
        
        //! @brief Returns the inlet position relative to the parent PatcherView component for a given index.
        juce::Point<int> getInletPatcherPosition(const size_t index) const;
        
        //! @brief Returns the outlet position relative to the parent PatcherView component for a given index.
        juce::Point<int> getOutletPatcherPosition(const size_t index) const;
        
        //! Get the Object model
        model::Object& getModel() const {return *m_model;};
        
        //! @brief overloaded from Component to exclude border size.
        bool hitTest(int x, int y) override;
        
        //! @brief internal kiwi PatcherView HitTesting.
        bool hitTest(juce::Point<int> const& pt, HitTester& result) const;
        
        //! @brief internal kiwi PatcherView HitTesting (overlaps a rectangle).
        bool hitTest(juce::Rectangle<int> const& rect);
        
        //! @brief Returns true if the object is selected.
        bool isSelected();
        
        //! @brief Returns true if the object is currently being editing.
        bool isEditing();
        
    private:
        
        void updateBounds(const bool animate);
        void drawInletsOutlets(juce::Graphics & g);
        
        //! @brief Returns the inlet local bounds for a given index.
        juce::Rectangle<int> getInletLocalBounds(const size_t index,
                                                 juce::Rectangle<int> const& object_bounds) const;
        
        //! @brief Returns the outlet local bounds for a given index.
        juce::Rectangle<int> getOutletLocalBounds(const size_t index,
                                                  juce::Rectangle<int> const& object_bounds) const;
        
    private: // members
        
        PatcherView&            m_patcher_view;
        model::Object*          m_model = nullptr;
        const unsigned int      m_io_width = 6;
        const unsigned int      m_io_height = 3;
        size_t                  m_inlets = 0;
        size_t                  m_outlets = 0;
        const juce::Colour      m_io_color;
        float                   m_selection_width = 3.;
        juce::Rectangle<int>    m_local_box_bounds;
        
        bool                    m_is_locked = 0;
        
        bool                    m_is_selected = 0;
        std::set<uint64_t>      m_distant_selection;
        bool                    m_is_editing;
        bool                    m_is_errorbox;
        
        friend ClassicBox;
    };
    
    // ================================================================================ //
    //                                   CLASSIC BOX                                    //
    // ================================================================================ //
    
    //! @brief The ClassicBox let the user change the text of the box
    class ClassicBox : public ObjectView, public SuggestEditor::Listener
    {
    public:
        
        //! @brief Constructor.
        ClassicBox(PatcherView& patcher_view, model::Object& object_m);
        
        //! @brief Destructor.
        ~ClassicBox();
        
        //! @brief Let the user edit the text of the box.
        void edit();
        
        //! @brief Remove the text editor and give focus back to the PatcherView.
        void removeTextEditor();
        
        //! @brief called when the object is resized.
        void resized() override;
        
        void suggestEditorTextChanged(SuggestEditor& ed) override;
        void suggestEditorReturnKeyPressed(SuggestEditor& ed) override;
        void suggestEditorEscapeKeyPressed(SuggestEditor& ed) override;
        void suggestEditorFocusLost(SuggestEditor& ed) override;
        
    private: // members
        
        std::unique_ptr<SuggestEditor> m_editor;
    };
}

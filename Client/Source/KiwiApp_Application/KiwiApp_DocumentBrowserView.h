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

#include <KiwiEngine/KiwiEngine_Listeners.h>

#include "../KiwiApp_Network/KiwiApp_DocumentBrowser.h"
#include "../KiwiApp_Components/KiwiApp_ImageButton.h"

namespace kiwi
{
    // ================================================================================ //
    //                               DOCUMENT BROWSER VIEW                              //
    // ================================================================================ //
    
    class DocumentBrowserView : public juce::Component, public DocumentBrowser::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentBrowserView(DocumentBrowser& browser);
        
        //! @brief Destructor.
        ~DocumentBrowserView();
        
        //! @brief Called when the document list changed.
        void driveAdded(DocumentBrowser::Drive& drive) override;
        
        //! @brief Called when a drive changed.
        void driveChanged(DocumentBrowser::Drive const& drive) override;
        
        //! @brief Called when the document list changed.
        void driveRemoved(DocumentBrowser::Drive const& drive) override;
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
    private: // nested classes
        
        class DriveView;
        
    private: // members
        
        DocumentBrowser&                        m_browser;
        std::vector<std::unique_ptr<DriveView>> m_drives;
    };
    
    // ================================================================================ //
    //                                 BROWSER DRIVE VIEW                               //
    // ================================================================================ //
    
    //! @brief Listen to document browser changes.
    class DocumentBrowserView::DriveView
    : public juce::Component
    , public juce::ListBoxModel
    , public juce::DragAndDropContainer
    , public DocumentBrowser::Drive::Listener
    {
    public:
        
        //! @brief Constructor.
        DriveView(DocumentBrowser::Drive& drive);
        
        //! @brief Destructor.
        ~DriveView();
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief Called by the DocumentBrowser::Drive changed.
        //! @details Called when one or more document has been changed / removed or added.
        void driveChanged() override;
        
        //! @brief Returns the number of items in the list.
        int getNumRows() override;
        
        //! @brief Draw a row of the list.
        //! @details Note that the rowNumber value may be greater than the number of rows in your
        //! list, so be careful that you don't assume it's less than getNumRows().
        void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool selected) override;
        
        //! @brief Used to create or update a custom component to go in a row of the list.
        juce::Component* refreshComponentForRow(int row, bool selected,
                                                juce::Component* component_to_update) override;
        
        //! @brief Called when the user clicking on a part of the list where there are no rows.
        void backgroundClicked(juce::MouseEvent const& e) override;
        
        //! @brief Called when the return key is pressed.
        void returnKeyPressed(int last_row_selected) override;
        
        //! @brief Called when the user double-clicking on a row.
        void listBoxItemDoubleClicked(int row, juce::MouseEvent const& e) override;
        
        //! @brief Returns true if the two drive view refer to the same drive.
        bool operator==(DocumentBrowser::Drive const& other_drive) const;
        
        //! @brief Opens document for the given row.
        void openDocument(int row);
        
        //! @brief Make an API call to rename the remote document
        void renameDocumentForRow(int row, std::string const& new_name);
        
        //! @brief Returns the document pointer corresponding to the row.
        DocumentBrowser::Drive::DocumentSession* getDocumentForRow(int row);
        
    private: // methods
        
        //! @brief Allows rows from the list to be dragged-and-dropped.
        //! @see juce::ListBoxModel::getDragSourceDescription, DragAndDropContainer::startDragging
        juce::var getDragSourceDescription(juce::SparseSet<int> const& rows_to_describe) override;
        
        //! @brief Called when a drag starts.
        void dragOperationStarted(juce::DragAndDropTarget::SourceDetails const& drag_source_details) override;
        
        //! @brief Called when a drag finishes.
        void dragOperationEnded(juce::DragAndDropTarget::SourceDetails const& drag_source_details) override;
        
    private: // classes
        
        class FileList;
        class Header;
        class FileElem;
        
        DocumentBrowser::Drive& useDrive();
        friend FileList;
        
    private: // members
        
        DocumentBrowser::Drive&     m_drive;
        std::unique_ptr<FileList>   m_file_list;
    };
    
    // ================================================================================ //
    //                             BROWSER DRIVE VIEW HEADER                            //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::Header : public juce::Component
    {
    public: // methods
        
        //! @brief Constructor
        Header(juce::ListBox& listbox, DocumentBrowser::Drive& drive);
        
        //! @brief Destructor
        ~Header() = default;
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief juce::Component::mouseDown
        void mouseDown(juce::MouseEvent const& event) override;
        
    private: // members
        
        juce::ListBox&              m_listbox;
        DocumentBrowser::Drive&     m_drive;
        ImageButton                 m_refresh_btn;
        ImageButton                 m_create_document_btn;
        const juce::Image           m_folder_img;
    };
    
    // ================================================================================ //
    //                              BROWSER DRIVE FILE LIST                             //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::FileList
    : public juce::ListBox
    {
    public: // methods
        
        //! @brief Constructor
        FileList(DriveView& drive_view);
        
    private: // variables
        
        DriveView& m_drive_view;
    };
    
    // ================================================================================ //
    //                            BROWSER DRIVE VIEW ROW ELEM                           //
    // ================================================================================ //
    
    class DocumentBrowserView::DriveView::FileElem
    : public juce::Component
    , public juce::Label::Listener
    , public juce::DragAndDropTarget
    {
    public: // methods
        
        //! @brief Constructor.
        FileElem(DriveView& drive_view, FileList& file_list, std::string const& name);
        
        //! @brief Destructor.
        ~FileElem();
        
        //! @brief Show the document name editor.
        void showEditor();
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        //! @brief Called when resized.
        void resized() override;
        
        //! @brief Called when a Label's text has changed.
        void labelTextChanged(juce::Label* label_text_that_has_changed) override;
        
        //! @brief Update the document session
        void update(std::string const& name,
                    Api::Document::Type const& type,
                    int row, bool now_selected);
        
        bool isInterestedInDragSource(SourceDetails const& drag_source_details) override;
        
    private: // methods
        
        //! @note In order to receive DragAndDropTarget notifications,
        //! this component needs to intercept mouse events,
        //! but as we do so, the (private) ListBox::RowComponent machinery (selection, DnD...) is broken,
        //! so we just forward mouse events to its parent component
        
        void mouseDown(juce::MouseEvent const& e) override;
        void mouseUp(juce::MouseEvent const& e) override;
        void mouseDoubleClick(juce::MouseEvent const& e) override;
        void mouseDrag(juce::MouseEvent const& e) override;
        
        void itemDropped(SourceDetails const& dragSourceDetails) override;
        void itemDragEnter(SourceDetails const& drag_source_details) override;
        void itemDragMove(SourceDetails const& drag_source_details) override;
        void itemDragExit(SourceDetails const& drag_source_details) override;
        
    private: // variables
        
        DriveView&          m_drive_view;
        std::string         m_name;
        Api::Document::Type m_type;
        juce::Label         m_name_label;
        
        const juce::Image   m_kiwi_filetype_img;
        const juce::Image   m_folder_img;
        
        int                 m_row;
        bool                m_selected;
        bool                m_something_is_being_dragged_over = false;
    };
}

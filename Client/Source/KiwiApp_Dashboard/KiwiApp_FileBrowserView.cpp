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

#include <KiwiEngine/KiwiEngine_Console.h>

#include "../KiwiApp_Application/KiwiApp_Instance.h"
#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"
#include "../KiwiApp.h"

#include "KiwiApp_FileBrowserView.h"

namespace kiwi
{
    // ================================================================================ //
    //                                    FILE BROWSER                                  //
    // ================================================================================ //
    
    FileBrowserView::FileBrowserView(FileBrowser& browser) :
    m_browser(browser)
    {
        setSize(1, 1);
        m_browser.addListener(*this);
        for(auto* drive : m_browser.getDrives())
        {
            driveAdded(*drive);
        }
        
        setSize(200, 300);
    }
    
    FileBrowserView::~FileBrowserView()
    {
        m_browser.removeListener(*this);
    }
    
    void FileBrowserView::driveAdded(FileBrowser::Drive& drive)
    {
        auto drive_view = std::make_unique<DriveView>(drive);
        drive_view->setSize(getWidth(), drive_view->getHeight());
        addAndMakeVisible(drive_view.get());
        m_drives.emplace_back(std::move(drive_view));
        resized();
    }
    
    void FileBrowserView::driveRemoved(FileBrowser::Drive const& drive)
    {
        const auto drive_view_it = std::find_if(m_drives.begin(), m_drives.end(), [&drive](std::unique_ptr<DriveView> const& drive_view){
            return true; //(*drive_view.get() == drive);
        });
        
        if(drive_view_it != m_drives.end())
        {
            removeChildComponent((*drive_view_it).get());
            m_drives.erase(drive_view_it);
        }
    }
    
    void FileBrowserView::driveChanged(FileBrowser::Drive const& drive)
    {
        resized();
    }
    
    void FileBrowserView::resized()
    {
        const auto bounds = getLocalBounds();
        
        int last_bottom = 0;
        for(auto const& drive_view_uptr : m_drives)
        {
            drive_view_uptr->setBounds(bounds);
            last_bottom = drive_view_uptr->getBottom();
        }
    }
    
    void FileBrowserView::paint(juce::Graphics& g)
    {
        ;
    }
    
    // ================================================================================ //
    //                                 BROWSER DRIVE VIEW                               //
    // ================================================================================ //
    
    FileBrowserView::DriveView::DriveView(FileBrowser::Drive& drive)
    : m_drive(drive)
    , m_file_list(std::make_unique<FileList>(*this))
    {
        m_drive.addListener(*this);
        m_file_list->updateContent();
        
        addAndMakeVisible(*m_file_list);
    }
    
    FileBrowserView::DriveView::~DriveView()
    {
        m_drive.removeListener(*this);
    }
    
    void FileBrowserView::DriveView::resized()
    {
        m_file_list->setBounds(getLocalBounds());
    }
    
    void FileBrowserView::DriveView::driveChanged()
    {
        m_file_list->updateContent();
        repaint();
    }
    
    int FileBrowserView::DriveView::getNumRows()
    {
        return m_drive.getDocuments().size();
    }
    
    void FileBrowserView::DriveView::paintListBoxItem(int rowNumber, juce::Graphics& g,
                                                          int width, int height, bool selected)
    {
        // nothing here, we use a custom row component instead.
    }
    
    juce::Component* FileBrowserView::DriveView::refreshComponentForRow(int row, bool selected,
                                                                            juce::Component* c)
    {
        auto const& documents = m_drive.getDocuments();
        
        if(row >= documents.size())
        {
            if(c != nullptr)
            {
                delete c;
                c = nullptr;
            }
        }
        else
        {
            auto& doc = *documents[row];
            
            if(c == nullptr)
            {
                c = new FileElem(*this, *m_file_list, doc.getName());
            }
            
            static_cast<FileElem*>(c)->update(doc.getName(), doc.getType(), row, selected);
        }
        
        return c;
    }
    
    void FileBrowserView::DriveView::backgroundClicked(juce::MouseEvent const&)
    {
        m_file_list->deselectAllRows();
    }
    
    void FileBrowserView::DriveView::returnKeyPressed(int last_row_selected)
    {
        if(m_file_list->getNumSelectedRows() > 0)
        {
            auto* c = m_file_list->getComponentForRowNumber(last_row_selected);
            if(c)
            {
                static_cast<FileElem*>(c)->showEditor();
            }
        }
    }
    
    FileBrowser::Drive::DocumentSession* FileBrowserView::DriveView::getDocumentForRow(int row)
    {
        auto& documents = m_drive.getDocuments();
        if(row < documents.size())
        {
            return documents[row].get();
        }
        
        return nullptr;
    }
    
    void FileBrowserView::DriveView::openDocument(int row)
    {
        if(auto* doc = getDocumentForRow(row))
        {
            doc->open();
        }
    }
    
    void FileBrowserView::DriveView::renameDocumentForRow(int row, std::string const& new_name)
    {
        if(auto* doc = getDocumentForRow(row))
        {
            doc->rename(new_name);
        }
    }
    
    juce::var FileBrowserView::DriveView::getDragSourceDescription(juce::SparseSet<int> const& rows_to_describe)
    {
        // for our drag description, we'll just make a comma-separated list of the selected row
        // numbers - this will be picked up by the drag target and displayed in its box.
        juce::StringArray rows;
        
        auto& documents = m_drive.getDocuments();
        
        for(int i = 0; i < documents.size(); ++i)
            rows.add(juce::String(documents[i]->getSessionId()));
        
        return rows.joinIntoString(", ");
    }
    
    void FileBrowserView::DriveView::dragOperationStarted(juce::DragAndDropTarget::SourceDetails const& drag_source_details)
    {
        m_file_list->deselectAllRows();
    }
    
    void FileBrowserView::DriveView::dragOperationEnded(juce::DragAndDropTarget::SourceDetails const& drag_source_details)
    {
        
    }
    
    void FileBrowserView::DriveView::listBoxItemDoubleClicked(int row, juce::MouseEvent const& e)
    {
        auto* doc = getDocumentForRow(row);
        if(doc)
        {
            doc->open();
        }
    }
    
    FileBrowser::Drive& FileBrowserView::DriveView::useDrive()
    {
        return m_drive;
    }
    
    // ================================================================================ //
    //                             BROWSER DRIVE VIEW HEADER                            //
    // ================================================================================ //
    
    FileBrowserView::DriveView::Header::Header(juce::ListBox& listbox, FileBrowser::Drive& drive) :
    m_listbox(listbox),
    m_drive(drive),
    m_refresh_btn("refresh", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::refresh_png, binary_data::images::refresh_png_size))),
    m_create_document_btn("create", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::plus_png, binary_data::images::plus_png_size))),
    m_folder_img(juce::ImageCache::getFromMemory(binary_data::images::folder_png,
                                                 binary_data::images::folder_png_size))
    {
        m_create_document_btn.setCommand(std::bind(&FileBrowser::Drive::createNewDocument, &m_drive));
        m_create_document_btn.setSize(40, 40);
        m_create_document_btn.setTooltip("Create a new patcher on this drive");
        m_create_document_btn.setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_create_document_btn);
        
        m_refresh_btn.setCommand(std::bind(&FileBrowser::Drive::refresh, &m_drive));
        m_refresh_btn.setSize(40, 40);
        m_refresh_btn.setTooltip("Refresh Document list");
        m_refresh_btn.setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_refresh_btn);
    }
    
    void FileBrowserView::DriveView::Header::resized()
    {
         m_refresh_btn.setTopRightPosition(getWidth(), 5);
         m_create_document_btn.setTopRightPosition(m_refresh_btn.getX(), 5);
    }
    
    void FileBrowserView::DriveView::Header::paint(juce::Graphics& g)
    {
        auto& lf = KiwiApp::useLookAndFeel();
        const juce::Colour color = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
        
        g.fillAll(color);
        
        const auto bounds = getLocalBounds();
        
        const auto folder_img_bounds = bounds.withRight(getHeight()).reduced(10);
        
        g.drawImage(m_folder_img, folder_img_bounds.toFloat(),
                    juce::RectanglePlacement::yMid, false);
        
        g.setColour(juce::Colours::whitesmoke);
        g.setFont(18);
        g.drawFittedText(m_drive.getName(),
                         bounds
                         .withLeft(folder_img_bounds.getRight() + 10)
                         .withRight(m_create_document_btn.getX() - 5),
                         juce::Justification::centredLeft, 1);
        
        g.setColour(color.darker(0.3));
        g.drawHorizontalLine(bounds.getBottom() - 1, 0, getWidth());
    }
    
    void FileBrowserView::DriveView::Header::mouseDown(juce::MouseEvent const& e)
    {
        m_listbox.getModel()->backgroundClicked(e);
    }
    
    // ================================================================================ //
    //                              BROWSER DRIVE FILE LIST                             //
    // ================================================================================ //
    
    FileBrowserView::DriveView::FileList::FileList(DriveView& drive_view)
    : juce::ListBox("document list", &drive_view)
    , m_drive_view(drive_view)
    {
        setMultipleSelectionEnabled(true);
        setRowSelectedOnMouseDown(true);
        setRowHeight(40);
        
        auto* header = new Header(*this, drive_view.useDrive());
        header->setSize(getWidth(), 50);
        setHeaderComponent(header);
        
        getViewport()->setScrollBarThickness(10);
        setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFFD4D4D4));
        
        updateContent();
    }
    
    // ================================================================================ //
    //                                  FILE ITEM VIEW                                  //
    // ================================================================================ //
    
    FileBrowserView::DriveView::FileElem::FileElem(DriveView& drive_view,
                                                     FileList& file_list,
                                                     std::string const& name)
    : m_drive_view(drive_view)
    , m_name(name)
    , m_kiwi_filetype_img(juce::ImageCache::getFromMemory(binary_data::images::kiwi_filetype_png,
                                                          binary_data::images::kiwi_filetype_png_size))
    , m_folder_img(juce::ImageCache::getFromMemory(binary_data::images::folder_png,
                                                   binary_data::images::folder_png_size))
    , m_row(-1)
    , m_something_is_being_dragged_over(false)
    {
        // label setup
        m_name_label.setText(m_name, juce::NotificationType::dontSendNotification);
        m_name_label.setSize(1, 1);
        m_name_label.setEditable(false, true, true);
        m_name_label.addListener(this);
        m_name_label.setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(m_name_label);
        
        m_name_label.setInterceptsMouseClicks(false, false);
        
        setInterceptsMouseClicks(true, false);
        setRepaintsOnMouseActivity(true);
    }
    
    FileBrowserView::DriveView::FileElem::~FileElem()
    {
        ;
    }
    
    void FileBrowserView::DriveView::FileElem::update(std::string const& name,
                                                         Api::Document::Type const& type,
                                                         int row, bool now_selected)
    {
        bool changed = false;
        if(row != m_row)
        {
            m_row = row;
            changed = true;
        }
        
        if(name != m_name)
        {
            m_name = name;
            m_name_label.setText(m_name, juce::NotificationType::dontSendNotification);
        }
        
        if(m_selected != now_selected)
        {
            m_selected = now_selected;
            m_name_label.setColour(juce::Label::textColourId, m_selected ? juce::Colours::whitesmoke : juce::Colours::black);
            changed = true;
        }
        
        if(m_type != type)
        {
            m_type = type;
            changed = true;
            repaint();
        }
        
        if(changed)
        {
            repaint();
        }
    }
    
    void FileBrowserView::DriveView::FileElem::paint(juce::Graphics& g)
    {
        const bool is_folder = (m_type == Api::Document::Type::Folder);
        auto bounds = getLocalBounds();
        const juce::Colour bg_color(0xDDFFFFFF);
        const juce::Colour selected_color(0xFF4285f4);
        
        if(!m_something_is_being_dragged_over)
        {
            g.setColour(bg_color.contrasting(0.3f));
            g.fillRect(bounds.removeFromBottom(1));
        }
        
        g.setColour(m_selected ? selected_color : isMouseOver(true) ? bg_color.darker(0.1f) : bg_color);
        g.fillRect(bounds);
        
        if(m_something_is_being_dragged_over)
        {
            g.setColour(selected_color.withAlpha(0.6f));
            g.fillRect(bounds);
            g.setColour(selected_color);
            g.drawRect(bounds, 2);
        }
        
        g.drawImage(is_folder ? m_folder_img : m_kiwi_filetype_img,
                    juce::Rectangle<float>(8, 5, 30, 30),
                    juce::RectanglePlacement::stretchToFit, false);
    }
    
    void FileBrowserView::DriveView::FileElem::resized()
    {
        const auto bounds = getLocalBounds();
        m_name_label.setBounds(bounds.reduced(5).withLeft(40));
    }
    
    void FileBrowserView::DriveView::FileElem::mouseDown(juce::MouseEvent const& e)
    {
        if(auto* c = getParentComponent())
        {
            c->mouseDown(e.getEventRelativeTo(c));
        }
    }
    
    void FileBrowserView::DriveView::FileElem::mouseUp(juce::MouseEvent const& e)
    {
        if(auto* c = getParentComponent())
        {
            c->mouseUp(e.getEventRelativeTo(c));
        }
    }
    
    void FileBrowserView::DriveView::FileElem::mouseDoubleClick(juce::MouseEvent const& e)
    {
        if(auto* c = getParentComponent())
        {
            c->mouseDoubleClick(e.getEventRelativeTo(c));
        }
    }
    
    void FileBrowserView::DriveView::FileElem::mouseDrag(juce::MouseEvent const& e)
    {
        if(auto* c = getParentComponent())
        {
            c->mouseDrag(e.getEventRelativeTo(c));
        }
    }
    
    void FileBrowserView::DriveView::FileElem::labelTextChanged(juce::Label* label)
    {
        if(label == &m_name_label)
        {
            m_drive_view.renameDocumentForRow(m_row, m_name_label.getText().toStdString());
        }
    }
    
    void FileBrowserView::DriveView::FileElem::showEditor()
    {
        m_name_label.showEditor();
    }
    
    bool FileBrowserView::DriveView::FileElem::isInterestedInDragSource(SourceDetails const& details)
    {
        return ((details.sourceComponent.get() != this)
                && (m_type == Api::Document::Type::Folder)
                && (dynamic_cast<FileList*>(details.sourceComponent.get()) != nullptr));
    }
    
    void FileBrowserView::DriveView::FileElem::itemDragEnter(SourceDetails const& drag_source_details)
    {
        m_something_is_being_dragged_over = true;
        m_name_label.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
        repaint();
    }
    
    void FileBrowserView::DriveView::FileElem::itemDragMove(SourceDetails const& drag_source_details)
    {
    }
    
    void FileBrowserView::DriveView::FileElem::itemDragExit(SourceDetails const& drag_source_details)
    {
        m_something_is_being_dragged_over = false;
        m_name_label.setColour(juce::Label::textColourId, juce::Colours::black);
        repaint();
    }
    
    void FileBrowserView::DriveView::FileElem::itemDropped(SourceDetails const& details)
    {
        std::cout << "item droped in file item\n";
        std::cout << details.description.toString() << '\n';
        m_something_is_being_dragged_over = false;
        m_name_label.setColour(juce::Label::textColourId, juce::Colours::black);
        repaint();
    }
}

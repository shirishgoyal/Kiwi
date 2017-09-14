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

#include "../KiwiApp_Network/KiwiApp_Api.h"
#include "../KiwiApp_Network/KiwiApp_ApiController.h"

#include "../KiwiApp_General/KiwiApp_StoredSettings.h"

#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  FILE BROWSER                                    //
    // ================================================================================ //
    
    //! @brief Request Patcher document informations through a Kiwi API.
    class FileBrowser
    : public juce::Timer
    , public NetworkSettings::Listener
    , public ApiConnectStatusListener
    {
    public: // nested classes
        
        struct Listener;
        class Drive;
        
    public: // methods
        
        //! @brief Constructor
        FileBrowser();
        
        //! @brief Destructor
        ~FileBrowser();
        
        //! @brief start processing
        void start(const int interval = 5000);
        
        //! @brief stop processing
        void stop();
        
        //! @brief Update document list.
        void process();
        
        //! @brief juce::Timer callback.
        void timerCallback() override;
        
        //! @brief Returns a list of drives.
        std::vector<Drive*> getDrives() const;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
    private: // methods
        
        void networkSettingsChanged(NetworkSettings const&, const juce::Identifier& id) override;
        
        void userLoggedIn(Api::AuthUser const&) override;
        void userLoggedOut(Api::AuthUser const&) override;
        void authUserChanged(Api::AuthUser const&) override;
        
    private: // variables
        
        std::unique_ptr<Drive>          m_distant_drive;
        engine::Listeners<Listener>     m_listeners = {};
    };
    
    // ================================================================================ //
    //                               FILE BROWSER LISTENER                              //
    // ================================================================================ //
    
    //! @brief Listen to document explorer changes.
    struct FileBrowser::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when the document list changed.
        virtual void driveAdded(FileBrowser::Drive& drive) = 0;
        
        //! @brief Called when the document list changed.
        virtual void driveChanged(FileBrowser::Drive const& drive) = 0;
        
        //! @brief Called when the document list changed.
        virtual void driveRemoved(FileBrowser::Drive const& drive) = 0;
    };
    
    // ================================================================================ //
    //                                FILE BROWSER DRIVE                                //
    // ================================================================================ //
    
    class FileBrowser::Drive
    {
    public: // nested classes
        
        struct Listener;
        class DocumentSession;
        
        using DocumentSessions = std::vector<std::unique_ptr<DocumentSession>>;
        
    public: // methods
        
        Drive(std::string const& name, uint16_t session_port);
        
        ~Drive() = default;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Set the kiwi document session port.
        void setSessionPort(uint16_t port);
        
        //! @brief Returns the kiwi document session port.
        uint16_t getSessionPort() const;
        
        //! @brief Set the name of this drive.
        void setName(std::string const& host);
        
        //! @brief Returns the name of this drive.
        std::string const& getName() const;
        
        //! @brief Creates and opens a new document on this drive.
        void createNewDocument();
        
        //! @brief Returns the documents.
        DocumentSessions const& getDocuments() const;
        
        //! @brief Returns the documents.
        DocumentSessions& getDocuments();
        
        //! @brief Refresh all the document list.
        void refresh();
        
    private: // members
        
        //! @internal Update the document list (need to be called in the juce Message thread)
        void updateDocumentList(Api::Documents docs);
        
        uint16_t                    m_session_port = 9090;
        std::string                 m_name = "Drive";
        DocumentSessions            m_documents;
        engine::Listeners<Listener> m_listeners;
        
        friend class FileBrowser;
    };
    
    // ================================================================================ //
    //                           FILE BROWSER LISTENER                           //
    // ================================================================================ //
    
    //! @brief Listen to file browser changes.
    struct FileBrowser::Drive::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when a document session has been added.
        virtual void documentAdded(FileBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when a document session changed.
        virtual void documentChanged(FileBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when a document session has been removed.
        virtual void documentRemoved(FileBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when one or more documents has been added, removed or changed.
        virtual void driveChanged() {};
    };
    
    // ================================================================================ //
    //                                  DRIVE FILE                                  //
    // ================================================================================ //
    
    class FileBrowser::Drive::DocumentSession
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentSession(FileBrowser::Drive& parent, Api::Document document);
        
        //! @brief Destructor.
        ~DocumentSession();
        
        //! @brief Returns the document drive
        Drive& useDrive();
        
        //! @brief Tells the Kiwi instance to open up this document.
        void open();
        
        //! @brief Returns the document name
        std::string getName() const;
        
        //! @brief Returns the document type
        Api::Document::Type getType() const;
        
        //! @brief Returns the session id of the document.
        uint64_t getSessionId() const;
        
        //! @brief Returns the drive that holds this document.
        FileBrowser::Drive const& useDrive() const;
        
        //! @brief Rename the document (remotely).
        void rename(std::string const& new_name);
        
        //! @brief Returns true if the DocumentSession match another DocumentSession
        //! @details this operator uses the session_id field to compare.
        bool operator==(DocumentSession const& other_doc) const;
        
    private: // members
        
        FileBrowser::Drive&             m_drive;
        Api::Document                       m_document;
        
        friend class FileBrowser::Drive;
    };
}

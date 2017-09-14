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

#include "KiwiApp_FileBrowser.h"
#include "../KiwiApp.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include "../KiwiApp_General/KiwiApp_IDs.h"

namespace kiwi
{
    // ================================================================================ //
    //                                    FILE BROWSER                                  //
    // ================================================================================ //
    
    FileBrowser::FileBrowser() :
    m_distant_drive(nullptr)
    {
        auto& settings = getAppSettings().network();
        settings.addListener(*this);
        
        m_distant_drive.reset(new Drive("Remote Drive", settings.getSessionPort()));
        
        int time = settings.getRefreshInterval();
        if(time > 0)
        {
            start(time);
        }
        
        authUserChanged(KiwiApp::getCurrentUser());
        KiwiApp::addApiConnectStatusListener(*this);
    }
    
    FileBrowser::~FileBrowser()
    {
        KiwiApp::removeApiConnectStatusListener(*this);
        getAppSettings().network().removeListener(*this);
        stop();
    }
    
    void FileBrowser::start(const int interval)
    {
        startTimer(interval);
    }
    
    void FileBrowser::stop()
    {
        stopTimer();
    }
    
    std::vector<FileBrowser::Drive*> FileBrowser::getDrives() const
    {
        return {m_distant_drive.get()};
    }
    
    void FileBrowser::networkSettingsChanged(NetworkSettings const& settings, const juce::Identifier& id)
    {
        if(id == Ids::session_port)
        {
            m_distant_drive->setSessionPort(settings.getSessionPort());
        }
        else if(id == Ids::refresh_interval)
        {
            auto time = settings.getRefreshInterval();
            
            if(time > 0)
            {
                start(time);
            }
            else
            {
                stop();
            }
        }
    }
    
    void FileBrowser::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void FileBrowser::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void FileBrowser::timerCallback()
    {
        process();
    }
    
    void FileBrowser::process()
    {
        m_distant_drive->refresh();
    }
    
    void FileBrowser::userLoggedIn(Api::AuthUser const& user)
    {
        m_distant_drive->setName(user.getName());
    }
    
    void FileBrowser::userLoggedOut(Api::AuthUser const&)
    {
        m_distant_drive->setName("logged out...");
    }
    
    void FileBrowser::authUserChanged(Api::AuthUser const& user)
    {
        m_distant_drive->setName(user.isLoggedIn() ? user.getName() : "logged out...");
    }
    
    // ================================================================================ //
    //                                 FILE BROWSER DRIVE                               //
    // ================================================================================ //
    
    FileBrowser::Drive::Drive(std::string const& name,
                                  uint16_t session_port) :
    m_session_port(session_port),
    m_name(name)
    {
        ;
    };
    
    void FileBrowser::Drive::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void FileBrowser::Drive::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void FileBrowser::Drive::setSessionPort(uint16_t port)
    {
        m_session_port = port;
    }
    
    uint16_t FileBrowser::Drive::getSessionPort() const
    {
        return m_session_port;
    }
    
    void FileBrowser::Drive::setName(std::string const& name)
    {
        m_name = name;
        m_listeners.call(&Listener::driveChanged);
    }
    
    std::string const& FileBrowser::Drive::getName() const
    {
        return m_name;
    }
    
    void FileBrowser::Drive::createNewDocument()
    {
        KiwiApp::useApi().createDocument("", [this](Api::Response res, Api::Document document) {
            
            if(res.error)
            {
                juce::MessageManager::callAsync([message = res.error.message()](){
                    KiwiApp::error("Error: can't create document");
                    KiwiApp::error("=> " + message);
                });
            }
            else
            {
                juce::MessageManager::callAsync([this, document]() {
                    
                    auto it = m_documents.emplace(m_documents.end(),
                                                  std::make_unique<DocumentSession>(*this,
                                                                                    std::move(document)));
                    
                    m_listeners.call(&Listener::documentAdded, *(it->get()));
                    m_listeners.call(&Listener::driveChanged);
                    
                    (*it)->open();
                });
            }
        });
    }
    
    FileBrowser::Drive::DocumentSessions const& FileBrowser::Drive::getDocuments() const
    {
        return m_documents;
    }
    
    FileBrowser::Drive::DocumentSessions& FileBrowser::Drive::getDocuments()
    {
        return m_documents;
    }
    
    void FileBrowser::Drive::updateDocumentList(Api::Documents docs)
    {
        bool changed = false;
        
        DocumentSessions new_documents;
        new_documents.reserve(docs.size());
        
        for(auto && doc : docs)
        {
            new_documents.emplace_back(std::make_unique<DocumentSession>(*this, std::move(doc)));
        }
        
        // drive removed notification
        for(auto doc_it = m_documents.begin(); doc_it != m_documents.end();)
        {
            auto it = std::find_if(new_documents.begin(), new_documents.end(),
                                   [&new_doc = *(doc_it->get())](auto const& doc){
                                       return *(doc.get()) == new_doc;
                                   });
            
            if(it == new_documents.end())
            {
                m_listeners.call(&Listener::documentRemoved, *(doc_it->get()));
                doc_it = m_documents.erase(doc_it);
                changed = true;
                continue;
            }
            
            ++doc_it;
        }
        
        // drive added or changed notification
        for(auto && new_doc : new_documents)
        {
            auto it = std::find_if(m_documents.begin(), m_documents.end(),
                                   [&new_doc = *new_doc](auto const& doc){
                                       return *(doc.get()) == new_doc;
                                   });
            
            // new document
            if(it == m_documents.end())
            {
                it = m_documents.emplace(it, std::move(new_doc));
                auto& doc = *(it->get());
                
                m_listeners.call(&Listener::documentAdded, doc);
                changed = true;
            }
            else
            {
                // name is currently the only document field that can change.
                if(new_doc->getName() != (*it)->getName())
                {
                    (*it)->m_document.setName(new_doc->getName());
                    auto& doc = *(it->get());
                    
                    m_listeners.call(&Listener::documentChanged, doc);
                    changed = true;
                }
            }
        }
        
        if(changed)
        {
            m_listeners.call(&Listener::driveChanged);
        }
    }
    
    void FileBrowser::Drive::refresh()
    {
        KiwiApp::useApi().getDocuments([this](Api::Response res, Api::Documents docs) {
            
            if(res.error)
            {
                KiwiApp::error("Kiwi API error: can't get documents => " + res.error.message());
            }
            else
            {
                KiwiApp::useInstance().useScheduler().schedule([this, docs]() {
                    updateDocumentList(docs);
                });
            }
        });
    }
    
    // ================================================================================ //
    //                                    DRIVE FILE                                    //
    // ================================================================================ //
    
    FileBrowser::Drive::DocumentSession::DocumentSession(FileBrowser::Drive& parent,
                                                             Api::Document document) :
    m_drive(parent),
    m_document(std::move(document))
    {
        
    }
    
    FileBrowser::Drive::DocumentSession::~DocumentSession()
    {
        ;
    }
    
    std::string FileBrowser::Drive::DocumentSession::getName() const
    {
        return m_document.getName();
    }
    
    Api::Document::Type FileBrowser::Drive::DocumentSession::getType() const
    {
        return m_document.getType();
    }
    
    uint64_t FileBrowser::Drive::DocumentSession::getSessionId() const
    {
        return m_document.getIdAsInt();
    }
    
    FileBrowser::Drive const& FileBrowser::Drive::DocumentSession::useDrive() const
    {
        return m_drive;
    }
    
    void FileBrowser::Drive::DocumentSession::rename(std::string const& new_name)
    {
        if(new_name.empty())
        {
            return;
        }
        
        KiwiApp::useApi().renameDocument(m_document.getIdAsString(), new_name, [](Api::Response res) {
            
            if(!res.error)
            {
                std::cout << "document update failed, err: " + res.error.message() << '\n';
            }
            else
            {
                std::cout << "document successfully updated\n";
            }
            
        });
    }
    
    bool FileBrowser::Drive::DocumentSession::operator==(DocumentSession const& other_doc) const
    {
        return (&m_drive == &other_doc.useDrive()) && (m_document == other_doc.m_document);
    }
    
    FileBrowser::Drive& FileBrowser::Drive::DocumentSession::useDrive()
    {
        return m_drive;
    }
    
    void FileBrowser::Drive::DocumentSession::open()
    {
        if(m_document.getType() == Api::Document::Type::Patcher)
        {
            KiwiApp::useInstance().openRemotePatcher(*this);
        }
    }
}

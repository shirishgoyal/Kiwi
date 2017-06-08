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

#include <flip/BackEndBinary.h>
#include <flip/detail/PortFactoryListener.h>
#include <flip/detail/PortListener.h>
#include <flip/contrib/transport_tcp/PortTransportServerTcp.h>
#include <flip/DocumentServer.h>

#include <KiwiModel/KiwiModel_PatcherUser.h>
#include <KiwiModel/KiwiModel_PatcherValidator.h>

#include <juce_core/juce_core.h>

#include <atomic>


namespace kiwi
{
    namespace server
    {
        // ================================================================================ //
        //                                      SERVER                                      //
        // ================================================================================ //
        
        //! @brief Converts and int64_t into a string representing it.
        std::string hexadecimal_convert(uint64_t hexa_decimal);
        
        //! @brief The Server class.
        class Server : public flip::PortFactoryListener, public flip::PortListener
        {
        public: // classes
            
            class Session;
            
        public: // methods
            
            //! @brief Constructor.
            Server(uint16_t port, std::string const& backend_directory);
            
            //! @brief Destructor.
            //! @details Disconnect all users and clean sessions.
            ~Server();
            
            //! @brief Process the socket hence process all sessions.
            void process();
            
            //! @brief Run the server.
            //! @details This will run a loop that call ServerBase::process regularly.
            //! @see stop
            void run();
            
            //! @brief Stops the server.
            //! @see Run, isRunning
            void stop();
            
            //! @brief Returns true if the server is running.
            bool isRunning() const noexcept;
            
        private: // methods
            
            //! @brief Creates a new session for server.
            void createSession(uint64_t session_id);
            
            //! @brief Called when a user connects to a document
            void onConnected(flip::PortBase & port);
            
            //! @brief Called when a user has been disconnected from a document
            void onDisconnected(flip::PortBase & port);
            
            //! @brief Get the path for a given session.
            juce::File getSessionFile(uint64_t session_id);
            
        private: // methods
            
            // PortFactoryListener
            void port_factory_add(flip::PortBase & port) override final;
            
            void port_factory_remove(flip::PortBase & port) override final;
            
            // PortListener
            void port_greet(flip::PortBase& port) override final;
            
            void port_commit(flip::PortBase& port, flip::Transaction const & tx) override final;
            
            void port_squash(flip::PortBase& port,
                             flip::TxIdRange const& range,
                             flip::Transaction const& tx) override final;
            
            void port_push(flip::PortBase& port, flip::Transaction const& tx) override final;
            
            void port_signal(flip::PortBase& port, const flip::SignalData & data) override final;
            
        private: // variables
            
            std::atomic_bool                m_running;
            juce::File                      m_backend_directory;
            std::map <uint64_t, Session>    m_sessions;
            flip::PortTransportServerTcp    m_socket;
            std::set <flip::PortBase *>     m_ports;
            
            static const char*  kiwi_file_extension;
            
        private: // deleted methods
            
            Server(Server const& other) = delete;
            Server(Server &&other) = delete;
            Server &operator=(Server const& other) = delete;
            Server &operator=(Server &&other) = delete;
        };
        
        // ================================================================================ //
        //                                   SERVER SESSION                                 //
        // ================================================================================ //
        
        class Server::Session final
        {
        public: // methods
            
            //! @brief Constructor.
            //! @details Create a Session by moving another Session.
            Session(Session && rhs);
            
            //! @brief Constructor
            //! @detailss Initialized with an empty document.
            Session(uint64_t identifier, juce::File const& backend_file);
            
            //! @brief Destructor.
            //! @details Unbinds all documents and ports.
            ~Session();
            
            uint64_t getId() const;
            
            //! @brief Returns a backend holding full presentation of the document hold by this session.
            void save() const;
            
            //! @brief Loads the session's document from the designated Backend entity.
            void load();
            
            //! @brief Binds user to session.
            void bind(flip::PortBase & port);
            
            //! @brief Unbind user to session.
            void unbind(flip::PortBase & port);
            
            //! @brief Returns wether a certain user is connected to session.
            bool hasUser(uint64_t user_id) const;
            
            //! @brief Returns a list of connected users.
            std::vector<uint64_t> getConnectedUsers() const;
            
        private: // methods
            
            //! @brief Checks if user has access to this particuliar session.
            bool authenticateUser(uint64_t user_id, std::string metadata) const;
            
            //! @brief Replies to a client with a list of connected users.
            void sendConnectedUsers() const;
            
        private: // variables
            
            const uint64_t                              m_identifier;
            std::unique_ptr<model::PatcherValidator>    m_validator;
            std::unique_ptr<flip::DocumentServer>       m_document;
            std::vector<flip::SignalConnection>         m_signal_connections;
            juce::File                                  m_backend_file;
            
        private: // variables
            
            Session() = delete;
            Session(const Session & rhs) = delete;
            Session& operator = (const Session & rhs) = delete;
            Session& operator = (Session && rhs) = delete;
        };
    }
}

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

#include <functional>

#include "flip/contrib/transport_tcp/CarrierTransportSocketTcp.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  CARRIER SOCKET                                  //
    // ================================================================================ //
    
    //! @brief Class that encapsulate a TCP socket
    class CarrierSocket
    {
    public: // methods
        
        //! @brief Constructor
        CarrierSocket(flip::DocumentBase& document, std::string const& host, uint16_t port, uint64_t session_id);
        
        // @brief Connects the socket to a remote socket
        void connect(std::string const& host, uint16_t port);
        
        //! @brief Stop the socket from processing and disconnect
        void disconnect();
        
        //! @brief Returns true if the socket is connected, false otherwise
        bool isConnected() const;
        
        //! @brief Process the socket once
        void process();
        
        //! @brief Add a callback to be called once disconnected
        void listenDisconnected(std::function<void ()> func);
        
        //! @brief Add a callback to be called on connected
        void listenConnected(std::function<void ()> func);
        
        //! @brief Add a callback to be called once first load terminated
        void listenLoaded(std::function<void ()> func);
        
        //! @brief Stops processing
        ~CarrierSocket();
        
    private: // methods
        
        //! @brief Binds the encapsulated socket callbacks
        void bindCallBacks();
        
        //! @brief Called when connection transition happen
        void listenStateTransition(flip::CarrierBase::Transition state, flip::CarrierBase::Error error);
        
        //! @brief Called during loading process
        void listenTransferBackEnd(size_t cur, size_t total);
        
        //! @brief Called when receiving a transaction
        void listenTransferTransaction(size_t cur, size_t total);
        
        //! @brief Called when receiving a signal
        void listenTransferSignal(size_t cur, size_t total);
        
    private: // members
        
        flip::CarrierTransportSocketTcp m_transport_socket;
        
        std::function<void(void)> m_func_disonnected;
        std::function<void(void)> m_func_connected;
        std::function<void(void)> m_func_loaded;
        
    private: // deleted methods
        
        CarrierSocket(CarrierSocket const& other) = delete;
        CarrierSocket(CarrierSocket && other) = delete;
        CarrierSocket& operator=(CarrierSocket const& other) = delete;
        CarrierSocket& operator=(CarrierSocket && other) = delete;
    };
}

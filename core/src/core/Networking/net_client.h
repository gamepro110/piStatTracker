#pragma once

//#include "core/log.h"
#include "net_common.h"

namespace net {
    template <typename T>
    class client_interface {
    public:
        client_interface() : m_socket(m_context) {
            // initialize the socket with the io context, so it can do stuff
        }

        virtual ~client_interface() {
            // if the client is destroyed, always try and disconnect from server
            Disconnect();
        }

    public:
        // connect to server with hostname/ip-address and port
        bool Connect(const std::string& host, const uint16_t port) {
            try {
                // Resolve hostname/ip-address into tangiable physical address
                asio::ip::tcp::resolver _resolver(m_context);                   //TODO find out why the context is lost upon entering
                asio::ip::tcp::resolver::results_type _endpoints = _resolver.resolve(host, std::to_string(port));

                // create connection
                m_connection = std::make_unique<connection<T>>(connection<T>::owner::Client, m_context, asio::ip::tcp::socket(m_context), m_qMessagesIn);

                // tell the connection object to connect to server
                m_connection->ConnectToServer(_endpoints);

                thrContext = std::thread([this]() { m_context.run(); });
            }
            catch (const std::exception& e) {
                STATS_Core_ERROR("[Client Exception]: {}", e.what());
                return false;
            }

            return true;
        }

        // disconnect from server
        void Disconnect() {
            // if connection exists, and its connected the..
            if (IsConnected()) {
                // ...disconnect from server gracefully
                m_connection->Disconnect();
            }

            // either way, we`re also done with the asio context...
            m_context.stop();
            // ...and its thread
            if (thrContext.joinable()) {
                thrContext.join();
            }

            // destroy the connection object
            m_connection.release();
        }

        // check if client is actually connected to a server
        bool IsConnected() {
            if (m_connection) {
                return m_connection->IsConnected();
            } else {
                return false;
            }
        }

    public:
        // send message to server
        void SendMsg(const message<T>& msg) {
            if (IsConnected()) {
                m_connection->SendMsg(msg);
            }
        }

        // retrieve queue of messages from server
        tsqueue<owned_message<T>>& Incomming() {
            return m_qMessagesIn;
        }

    protected:
        // asio context handles the data transfer...
        asio::io_context m_context;
        // ...but needs a thread of ies own to execute its work commands
        std::thread thrContext;
        // this is the hardware socket that is connected to the server
        asio::ip::tcp::socket  m_socket;
        // the client has a single instance of a "connection" object, which handles data transfer
        std::unique_ptr<connection<T>> m_connection;

    private:
        // this is the thread safe queue of incomming messages from the server
        tsqueue<owned_message<T>> m_qMessagesIn;
    };
}

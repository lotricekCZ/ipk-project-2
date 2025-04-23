/**
 * @file TCP_transceiver.hpp
 * @brief Implementation of TCPTransceiver class
 *
 * This class is a transceiver implementation using TCP sockets.
 */

#ifndef TCP_TRANSCEIVER_HPP
#define TCP_TRANSCEIVER_HPP

#include <stdexcept>

#include "transceiver.hpp"
#include "../receivers/TCP_receiver.hpp"
#include "../senders/TCP_sender.hpp"

/**
 * @class TCPTransceiver
 * @brief A transceiver implementation using TCP sockets
 *
 * This class is a transceiver implementation using TCP sockets. It provides
 * methods for sending and receiving data over a TCP connection.
 */
class TCPTransceiver : public Transceiver,
                       public TCPReceiver,
                       public TCPSender
{
    using Transceiver::close;
    using Transceiver::open;

public:
    TCPTransceiver();
    ~TCPTransceiver();
    std::string receive() override;
    void init() override;
    void send(std::string message) override;
    void send(uint8_t *data, uint16_t size) override
    {
        (void)data;
        (void)size;
        throw std::runtime_error("Not implemented");
    };
    using Transceiver::isOpen;
};

#endif

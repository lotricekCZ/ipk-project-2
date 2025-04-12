/**
 * @file UDP_transceiver.hpp
 * @brief Implementation of UDPTransceiver class
 *
 * This class is a transceiver implementation using UDP sockets.
 */

#ifndef UDP_TRANSCEIVER_HPP
#define UDP_TRANSCEIVER_HPP

#include <stdexcept>

#include "transceiver.hpp"
#include "../receivers/UDP_receiver.hpp"
#include "../senders/UDP_sender.hpp"

/**
 * @class UDPTransceiver
 * @brief A transceiver implementation using UDP sockets
 *
 * This class is a transceiver implementation using UDP sockets. It provides
 * methods for sending and receiving data over a UDP connection.
 */
class UDPTransceiver : public Transceiver,
                       public UDPReceiver,
                       public UDPSender
{
    using Transceiver::close;
    using Transceiver::open;

public:
    UDPTransceiver();
    ~UDPTransceiver();
    std::string receive() override;
    void init() override;
    void send(std::string message) override { throw std::runtime_error("Not implemented"); };
    void send(uint8_t *data, uint16_t size) override;
    using Transceiver::isOpen;
};

#endif

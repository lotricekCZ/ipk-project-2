#include <stdexcept>

#include "transceiver.hpp"
#include "../receivers/TCP_receiver.hpp"
#include "../senders/TCP_sender.hpp"

#ifndef TCP_TRANSCEIVER_HPP
#define TCP_TRANSCEIVER_HPP

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
    void send(uint8_t *data, uint16_t size) override { throw std::runtime_error("Not implemented"); };
    using Transceiver::isOpen;
};

#endif
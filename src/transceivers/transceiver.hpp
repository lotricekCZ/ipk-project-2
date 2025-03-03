/**
 * @class Transceiver
 * @brief An abstract base class for transceiver functionality.
 * 
 * This class provides an interface for transceivers that can send and receive
 * data. It includes methods for opening and closing the transceiver, as well
 * as for sending and receiving data.
 */

#ifndef Transceiver_HPP
#define Transceiver_HPP
#include <string>
#include <cstdint>

class Transceiver {
    bool _open = false;
protected:
    void open();
    void close();
public:
    bool isOpen();
    virtual void init() = 0;
    virtual void send(std::string message) = 0;
    virtual void send(std::uint8_t *data, std::uint16_t size) = 0;
    virtual std::string receive() = 0;
};

#endif
#include "transceiver.hpp"

bool Transceiver::isOpen()
{
    return _open;
}

void Transceiver::open()
{
    _open = true;
}

void Transceiver::close()
{
    _open = false;
}
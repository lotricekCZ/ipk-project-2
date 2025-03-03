/**
 * @file transceiver.cpp
 * @brief Implementation of Transceiver class
 */
#include "transceiver.hpp"

/**
 * @brief Check if the transceiver is open
 * 
 * @return true if the transceiver is open, false otherwise
 */
bool Transceiver::isOpen()
{
    return _open;
}

/**
 * @brief Open the transceiver
 * 
 */
void Transceiver::open()
{
    _open = true;
}

/**
 * @brief Close the transceiver
 * 
 */
void Transceiver::close()
{
    _open = false;
}

/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * Arduino_CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "Arduino_CAN.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_CAN::Arduino_CAN(PinName const can_tx_pin, PinName const can_rx_pin)
: _can(can_rx_pin, can_tx_pin)
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

bool Arduino_CAN::begin(CanBitRate const can_bitrate)
{
  int const rc = _can.frequency(static_cast<int>(can_bitrate));
  return (rc == 1);
}

void Arduino_CAN::end()
{
  /* Nothing to do. */
}

int Arduino_CAN::write(CanMsg const & msg)
{
  mbed::CANMessage const can_msg(
    msg.id,
    msg.data,
    msg.data_length,
    CANData,
    CANStandard);

  return _can.write(can_msg);
}

size_t Arduino_CAN::available()
{
  mbed::CANMessage can_msg;
  bool const msg_read = _can.read(can_msg) > 0;

  if (msg_read)
  {
    CanMsg const msg(
      can_msg.id,
      can_msg.len,
      can_msg.data);

    _rx_msg_buf.enqueue(msg);
  }

  return _rx_msg_buf.available();
}

CanMsg Arduino_CAN::read()
{
  return _rx_msg_buf.dequeue();
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

/**************************************************************************************
 * OBJECT INSTANTIATION
 **************************************************************************************/

#if CAN_HOWMANY > 0
arduino::Arduino_CAN CAN(PIN_CAN0_TX, PIN_CAN0_RX);
#endif

#if CAN_HOWMANY > 1
arduino::Arduino_CAN CAN1(PIN_CAN1_TX, PIN_CAN1_RX);
#endif

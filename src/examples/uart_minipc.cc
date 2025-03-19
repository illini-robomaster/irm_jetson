/****************************************************************************
 *                                                                          *
 *  Copyright (C) 2025 RoboMaster.                                          *
 *  Illini RoboMaster @ University of Illinois at Urbana-Champaign          *
 *                                                                          *
 *  This program is free software: you can redistribute it and/or modify    *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  This program is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                          *
 ****************************************************************************/

#include "board/main.h"
#include "board/minipc_protocol.h"
#include "board/uart.h"
#include <iostream>

const communication::status_data_t *status_data;

communication::arm_data_t arm_data;
communication::color_data_t color_data;
communication::gimbal_data_t gimbal_data;
communication::chassis_data_t chassis_data;
communication::selfcheck_data_t selfcheck_data;

auto minipc_session = communication::MinipcPort();
uint8_t txdat[minipc_session.MAX_PACKET_LENGTH];
uint8_t *rxdat;

int main() {
  UARTSERIAL::UART *uart = UARTSERIAL::UART::from_prefix(PREF_CBOARD, 115200);

  uint8_t length;
  uint8_t rxid;

  selfcheck_data.mode = 1;
  selfcheck_data.debug_int = 9;

  std::cout << "Creating packet." << std::endl;
  minipc_session.Pack(txdat, (void *)&selfcheck_data,
                      communication::GIMBAL_CMD_ID);

  std::cout << "Sending packet." << std::endl;
  uart->write(txdat, minipc_session.GetPacketLen(communication::GIMBAL_CMD_ID));

  std::cout << "Reading packet." << std::endl;
  length = uart->read(rxdat);
  minipc_session.ParseUartBuffer(rxdat, length);
  rxid = minipc_session.GetCmdId();
  status_data = minipc_session.GetStatus();
  std::cout << "Recieved id: " << rxid << std::endl;

  return 0;
}

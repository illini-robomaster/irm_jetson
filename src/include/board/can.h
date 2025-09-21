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

#include <atomic>
#include <iostream>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <map>
#include <net/if.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#pragma once

#define MAX_CAN_DEVICES 12

namespace CANRAW {

typedef void (*can_rx_callback_t)(const uint8_t data[], void *args);

class CAN {
public:
  CAN(const char *name = "can0");
  ~CAN();
  /**
   * @brief Transmits a CAN message
   * @param can_id The CAN ID to transmit to
   * @param dat Pointer to the data to transmit
   * @param len Length of the data in bytes
   */
  void Transmit(canid_t can_id, uint8_t *dat, int len);

  /**
   * @brief Receives a single CAN message
   * @note This is a blocking call
   */
  void Receive();

  /**
   * @brief Closes the CAN socket and cleans up resources
   */
  void Close();

  /**
   * @brief Registers a callback for a specific CAN ID
   * @param can_id The CAN ID to register for
   * @param callback The callback function to invoke when message received
   * @param args Optional arguments to pass to the callback
   * @return 0 on success, -1 on failure
   */
  int RegisterCanDevice(canid_t can_id, can_rx_callback_t callback,
                        void *args = nullptr);

  /**
   * @brief Deregisters a callback for a specific CAN ID
   * @param can_id The CAN ID to deregister
   * @return 0 on success, -1 if CAN ID not found
   */
  int DeregisterCanDevice(canid_t can_id);
  struct can_frame frx;

  /**
   * @brief Starts a thread to continuously receive CAN messages
   * @param stop_flag Pointer to atomic bool to control thread execution
   * @param interval_ms Time between receive attempts in microseconds
   */
  std::atomic<bool> *StartReceiveThread(int interval_us = 10);
  std::atomic<bool> *stop_flag_;

private:
  int s;
  struct sockaddr_can addr;
  struct ifreq ifr;
  struct can_frame ftx;
  std::map<canid_t, std::pair<can_rx_callback_t, void *>> callback_map;
  std::atomic<bool> *receive_thread_present_;
};

} // namespace CANRAW

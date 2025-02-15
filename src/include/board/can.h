/****************************************************************************
 *                                                                          *
 *  Copyright (C) 2024 RoboMaster.                                          *
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

#include <cstring>
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

template <class T>
constexpr const T &clamp(const T &v, const T &lo, const T &hi) {
  return v < lo ? lo : hi < v ? hi : v;
}

typedef void (*can_rx_callback_t)(const uint8_t data[], void *args);

class CAN {
public:
  CAN(const char *name = "can0");
  void Transmit(canid_t can_id, uint8_t *dat, int len);
  void Receive();
  void Close();
  int RegisterCanDevice(canid_t can_id, can_rx_callback_t callback,
                        void *args = nullptr);
  int DeregisterCanDevice(canid_t can_id);
  struct can_frame frx;

private:
  int s;
  struct sockaddr_can addr;
  struct ifreq ifr;
  struct can_frame ftx;
  std::map<canid_t, std::pair<can_rx_callback_t, void *>> callback_map;
};

} // namespace CANRAW

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
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <map>

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

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
    CAN(const char* name = "can0");
    void Transmit(canid_t can_id, uint8_t *dat, int len);
    void Receive();
    void Close();
    int RegisterRxCallback(canid_t can_id, can_rx_callback_t callback, void *args);
    void RxCallback();
    struct can_frame frx;
  private:
    int s;
    can_rx_callback_t rx_callbacks_[MAX_CAN_DEVICES] = {0x0};
    void *rx_args_[MAX_CAN_DEVICES] = {NULL};
    std::map<uint16_t, uint8_t> id_to_index_;
    uint8_t callback_count_ = 0;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame ftx;
};

}

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
#include "can.h"

namespace CANRAW {

CAN::CAN(const char *name) {
  s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (s < 1) {
    std::cerr << "Error while opening socket" << std::endl;
  }

  strcpy(ifr.ifr_name, name);
  ioctl(s, SIOCGIFINDEX, &ifr);

  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    std::cerr << "Error while binding address" << std::endl;
  }
}

void CAN::Transmit(canid_t can_id, uint8_t *dat, int len) {
  ftx.can_id = can_id;
  ftx.can_dlc = clamp(len, 0, CAN_MAX_DLEN);
  memcpy(ftx.data, dat, sizeof(uint8_t) * ftx.can_dlc);
  if (write(s, &ftx, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
    std::cerr << "Error while sending CAN frame" << std::endl;
  }
}

void CAN::Receive() {
  if (read(s, &frx, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
    std::cerr << "Error while receiving CAN frame" << std::endl;
  }
}

int CAN::RegisterRxCallback(canid_t can_id, can_rx_callback_t callback,
                            void *args) {
  if (callback_count_ >= MAX_CAN_DEVICES) {
    return -1;
  }
  rx_callbacks_[callback_count_] = callback;
  rx_args_[callback_count_] = args;
  id_to_index_[can_id] = callback_count_;
  callback_count_++;

  return 0;
}

void CAN::RxCallback() {
  const auto index = id_to_index_.find(frx.can_id);
  if (index != id_to_index_.end() && rx_callbacks_[index->second] != 0x0) {
    rx_callbacks_[index->second](frx.data, rx_args_[index->second]);
  }
}

void CAN::Close() { close(s); }

} // namespace CANRAW

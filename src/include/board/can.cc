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
#include "utils.h"
#include <chrono>
#include <cstring>
#include <thread>

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

  stop_flag_->store(false);
  recieve_thread_present_->store(false);
}

CAN::~CAN() { this->Close(); }

void CAN::Transmit(canid_t can_id, uint8_t *dat, int len) {
  ftx.can_id = can_id;
  ftx.can_dlc = clip(len, 0, CAN_MAX_DLEN);
  memcpy(ftx.data, dat, sizeof(uint8_t) * ftx.can_dlc);
  if (write(s, &ftx, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
    std::cerr << "Error while sending CAN frame" << std::endl;
  }
}

void CAN::Receive() {
  if (read(s, &frx, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
    std::cerr << "Error while receiving CAN frame" << std::endl;
    return;
  }
  auto it = callback_map.find(frx.can_id);
  if (it != callback_map.end()) {
    it->second.first(frx.data, it->second.second);
  }
}

int CAN::RegisterCanDevice(canid_t can_id, can_rx_callback_t callback,
                           void *args) {
  if (callback_map.size() >= MAX_CAN_DEVICES) {
    std::cerr << "Maximum number of CAN devices reached" << std::endl;
    return -1;
  }
  callback_map[can_id] = std::make_pair(callback, args);
  return 0;
}

int CAN::DeregisterCanDevice(canid_t can_id) {
  auto it = callback_map.find(can_id);
  if (it != callback_map.end()) {
    callback_map.erase(it);
    return 0;
  }
  std::cerr << "Can ID " << can_id << " not registered" << std::endl;
  return -1;
}

std::atomic<bool> *CAN::StartReceiveThread(int interval_us) {
  if (recieve_thread_present_->load()) {
    std::cerr << "Error: Receive thread already running" << std::endl;
    return nullptr;
  }

  stop_flag_->store(false);
  recieve_thread_present_->store(true);
  std::thread([this, interval_us]() {
    while (!stop_flag_->load()) {
      this->Receive();
      std::this_thread::sleep_for(std::chrono::microseconds(interval_us));
    }
    recieve_thread_present_->store(false);
  }).detach();

  return stop_flag_;
}

void CAN::Close() {
  // Signal receive thread to stop
  stop_flag_->store(true);
  recieve_thread_present_->store(false);
  // Close socket
  close(s);
}

} // namespace CANRAW

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

#include "board/can.h"

void sendtest() {
  int i;
  int len = 8;
  uint8_t dat[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00};

  CANRAW::CAN *can0 = new CANRAW::CAN("can0");

  for (i = 0; i < 10; i++) {
    can0->Transmit(0x200, dat, len);
  }
  can0->Close();
}

int main() {
  std::cout << "Start can send test" << std::endl;
  sendtest();
  std::cout << "End can send test" << std::endl;

  return 0;
}

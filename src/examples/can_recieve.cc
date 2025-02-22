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
#include "stdint.h"
#include "stdio.h"
#include <unistd.h>

void receive(CANRAW::CAN *can0) {
  // CANRAW::CAN *can0 = new CANRAW::CAN("can0");
  // read(, &can0->frx, sizeof(struct can_frame));
  can0->Receive();
  for (int i = 0; i < 8; i++) {
    printf("%02x ", (int)can0->frx.data[i]);
  }
  printf("\n");
}

int main() {
  CANRAW::CAN *can0 = new CANRAW::CAN("can0");
  printf("Start can receive test\n");
  while (true) {
    receive(can0);
  }
  return 0;
}

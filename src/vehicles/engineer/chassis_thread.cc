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

#include "chassis_thread.h"
#include "board/utils.h"
#include "motor/motor.h"

static control::MotorCANBase *motor1 = nullptr;
static control::MotorCANBase *motor2 = nullptr;
static control::MotorCANBase *motor3 = nullptr;
static control::MotorCANBase *motor4 = nullptr;
static control::MotorCANBase *motor5 = nullptr;
static control::MotorCANBase *motor6 = nullptr;
static control::MotorCANBase *motor7 = nullptr;
static control::MotorCANBase *motor8 = nullptr;

CANRAW::CAN *can1;

static bool killed;

void chassis_thread(void *arg) {
  UNUSED(arg);

  can1 = new CANRAW::CAN("can1");

  motor1 = new control::Motor6020(can1, 0x205);
  motor2 = new control::Motor6020(can1, 0x206);
  motor3 = new control::Motor6020(can1, 0x207);
  motor4 = new control::Motor6020(can1, 0x208);

  motor5 = new control::Motor3508(can1, 0x201);
  motor6 = new control::Motor3508(can1, 0x202);
  motor7 = new control::Motor3508(can1, 0x203);
  motor8 = new control::Motor3508(can1, 0x204);

  control::MotorCANBase *wheel_motors[] = {motor5, motor6, motor7, motor8};
  control::MotorCANBase *steer_motors[] = {motor2, motor3, motor4, motor1};

  // dummy
  std::cout << "Hello World" << std::endl;
}

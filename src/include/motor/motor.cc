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

#include "motor.h"
#include "board/can.h"
#include "error_handler.h"
#include "math.h"
#include "utils.h"

namespace control {

int16_t ClipMotorRange(float output) {
  constexpr int MIN =
      -MOTOR_RANGE; /* Minimum that a 16-bit number can represent */
  constexpr int MAX =
      MOTOR_RANGE; /* Maximum that a 16-bit number can represent */
  return (int16_t)clip<int>((int)output, MIN, MAX);
}

//==================================================================================================
// MotorCanBase(DJI Base)
//==================================================================================================

/**
 * @brief standard can motor callback, used to update motor data
 *
 * @param data data that come from motor
 * @param args pointer to a MotorCANBase instance
 */
static void can_motor_callback(const uint8_t data[], void *args) {
  MotorCANBase *motor = reinterpret_cast<MotorCANBase *>(args);
  motor->UpdateData(data);
}

MotorCANBase::MotorCANBase(CANRAW::CAN *can, uint16_t rx_id)
    : theta_(0), omega_(0), can_(can), rx_id_(rx_id) {
  constexpr uint16_t GROUP_SIZE = 4;
  constexpr uint16_t RX1_ID_START = 0x201;
  constexpr uint16_t RX2_ID_START = 0x205;
  constexpr uint16_t RX3_ID_START = 0x209;
  constexpr uint16_t TX1_ID = 0x200;
  constexpr uint16_t TX2_ID = 0x1ff;
  constexpr uint16_t TX3_ID = 0x2ff;

  RM_ASSERT_GE(rx_id, RX1_ID_START, "Invalid rx id");
  RM_ASSERT_LT(rx_id, RX3_ID_START + GROUP_SIZE, "Invalid rx id");
  if (rx_id >= RX3_ID_START)
    tx_id_ = TX3_ID;
  else if (rx_id >= RX2_ID_START)
    tx_id_ = TX2_ID;
  else
    tx_id_ = TX1_ID;

  can_->RegisterCanDevice(rx_id_, can_motor_callback, this);
}

MotorCANBase::MotorCANBase(CANRAW::CAN *can, uint16_t rx_id, uint16_t type)
    : theta_(0), omega_(0), can_(can), rx_id_(rx_id) {
  UNUSED(type);
}

MotorCANBase::~MotorCANBase() { can_->DeregisterCanDevice(rx_id_); }

void MotorCANBase::TransmitOutput(MotorCANBase *motors[], uint8_t num_motors) {
  uint8_t data[8] = {0};

  RM_ASSERT_GT(num_motors, 0, "Meaningless empty can motor transmission");
  RM_ASSERT_LE(num_motors, 4,
               "Exceeding maximum of 4 motor commands per CAN message");
  for (uint8_t i = 0; i < num_motors; ++i) {
    RM_ASSERT_EQ(motors[i]->tx_id_, motors[0]->tx_id_, "tx id mismatch");
    RM_ASSERT_EQ(motors[i]->can_, motors[0]->can_, "can line mismatch");
    const uint8_t motor_idx = (motors[i]->rx_id_ - 1) % 4;
    const int16_t output = motors[i]->output_;

    data[2 * motor_idx] = output >> 8;
    data[2 * motor_idx + 1] = output & 0xff;
  }

  motors[0]->can_->Transmit(motors[0]->tx_id_, data, 8);
}

float MotorCANBase::GetTheta() const { return theta_; }

float MotorCANBase::GetThetaDelta(float target) const {
  return wrap<float>(target - theta_, -M_PI, M_PI);
}

float MotorCANBase::GetOmega() const { return omega_; }

float MotorCANBase::GetOmegaDelta(float target) const {
  return target - omega_;
}

int16_t MotorCANBase::GetCurr() const { return 0; }

uint16_t MotorCANBase::GetTemp() const { return 0; }

float MotorCANBase::GetTorque() const { return 0.0f; }

//==================================================================================================
// Motor3508
//==================================================================================================

Motor3508::Motor3508(CANRAW::CAN *can, uint16_t rx_id)
    : MotorCANBase(can, rx_id) {}

void Motor3508::UpdateData(const uint8_t data[]) {
  const int16_t raw_theta = data[0] << 8 | data[1];
  const int16_t raw_omega = data[2] << 8 | data[3];
  raw_current_get_ = data[4] << 8 | data[5];
  raw_temperature_ = data[6];

  constexpr float THETA_SCALE = 2 * M_PI / 8192; // digital -> rad
  constexpr float OMEGA_SCALE = 2 * M_PI / 60;   // rpm -> rad / sec
  theta_ = raw_theta * THETA_SCALE;
  omega_ = raw_omega * OMEGA_SCALE;

  connection_flag_ = true;
}

void Motor3508::PrintData() const {
  printf("theta: % .4f ", GetTheta());
  printf("omega: % .4f ", GetOmega());
  printf("raw temperature: %3d ", raw_temperature_);
  printf("raw current get: % d \r\n", raw_current_get_);
}

void Motor3508::SetOutput(int16_t val) {
  constexpr int16_t MAX_ABS_CURRENT = 12288; // ~20A
  output_ = clip<int16_t>(val, -MAX_ABS_CURRENT, MAX_ABS_CURRENT);
}

int16_t Motor3508::GetCurr() const { return raw_current_get_; }

uint16_t Motor3508::GetTemp() const { return raw_temperature_; }

//==================================================================================================
// ServoMotor (as Motor3508 packed as a servo)
//==================================================================================================

} // namespace control

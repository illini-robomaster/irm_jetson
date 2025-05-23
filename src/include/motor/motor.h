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

#pragma once

#include <linux/can.h>
#define UNUSED(X) (void)X /* To avoid gcc/g++ warnings */

#include "board/can.h"

namespace control {
constexpr int MOTOR_RANGE = 30000; // TODO: 32767 or 30000?

/**
 * @brief two modes for GetTheta()
 *  absolute_mode: -inf to +inf (Default option)
 *  relative_mode: 0 - 2pi
 **/
enum GetThetaMode { absolute_mode, relative_mode };

int16_t ClipMotorRange(float output);

// mode_t is the mode for Motor DM4310
typedef enum {
  MIT = 0,
  POS_VEL = 1,
  VEL = 2,
} mode_t;

//==================================================================================================
// MotorBase
//==================================================================================================

/**
 * @brief base class for motor representation
 */
class MotorBase {
public:
  MotorBase() : output_(0) {}
  virtual ~MotorBase() {}

  virtual void SetOutput(int16_t val) { output_ = val; }

protected:
  int16_t output_;
};

//==================================================================================================
// MotorCanBase(DJI Base)
//==================================================================================================

/**
 *
 * @brief base class for DJI based CAN Motors
 *
 */

class MotorCANBase : public MotorBase {
public:
  /**
   * @brief base constructor for DJI based CAN Motors
   *
   * @param can    CAN instance
   * @param rx_id  CAN rx id
   *
   */
  MotorCANBase(CANRAW::CAN *can, uint16_t rx_id);

  /**
   * @brief base constructor for other based CAN Motors
   *
   * @param can    CAN instance
   * @param rx_id  CAN rx id
   * @param type   Motor type
   *
   */
  MotorCANBase(CANRAW::CAN *can, uint16_t rx_id, uint16_t type);

  ~MotorCANBase();

  /**
   * @brief update motor feedback data
   * @note only used in CAN callback, do not call elsewhere
   *
   * @param data[]  raw data bytes
   */
  virtual void UpdateData(const uint8_t data[]) = 0;

  /**
   * @brief print out motor data
   */
  virtual void PrintData() const = 0;

  /**
   * @brief get rotor (the cap spinning on the back of the motor) angle, in
   * [rad]
   *
   * @return radian angle, range between [0, 2PI]
   */
  virtual float GetTheta() const;

  /**
   * @brief get angle difference (target - actual), in [rad]
   *
   * @param target  target angle, in [rad]
   *
   * @return angle difference, range between [-PI, PI]
   */
  virtual float GetThetaDelta(const float target) const;

  /**
   * @brief get angular velocity, in [rad / s]
   *
   * @return angular velocity
   */
  virtual float GetOmega() const;

  /**
   * @brief get angular velocity difference (target - actual), in [rad / s]
   *
   * @param target  target angular velocity, in [rad / s]
   *
   * @return difference angular velocity
   */
  virtual float GetOmegaDelta(const float target) const;

  virtual int16_t GetCurr() const;

  virtual uint16_t GetTemp() const;

  virtual float GetTorque() const;

  /**
   * @brief transmit CAN message for setting motor outputs
   *
   * @param motors[]    array of CAN motor pointers
   * @param num_motors  number of motors to transmit
   */
  static void TransmitOutput(MotorCANBase *motors[], uint8_t num_motors);
  /**
   * @brief set ServoMotor as friend of MotorCANBase since they need to use
   *        many of the private parameters of MotorCANBase.
   */
  friend class ServoMotor;

  volatile bool connection_flag_ = false;

protected:
  volatile float theta_;
  volatile float omega_;

private:
  CANRAW::CAN *can_;
  uint16_t rx_id_;
  uint16_t tx_id_;
};

//==================================================================================================
// Motor2006
//==================================================================================================

/**
 * @brief DJI 2006 motor class
 */
class Motor2006 : public MotorCANBase {
public:
  /* constructor wrapper over MotorCANBase */
  Motor2006(CANRAW::CAN *can, uint16_t rx_id);
  /* implements data update callback */
  void UpdateData(const uint8_t data[]) override final;
  /* implements data printout */
  void PrintData() const override final;
  /* override base implementation with max current protection */
  void SetOutput(int16_t val) override final;

  int16_t GetCurr() const override final;

private:
  volatile int16_t raw_current_get_ = 0;
};

//==================================================================================================
// Motor3508
//==================================================================================================

/**
 * @brief DJI 3508 motor class
 */
class Motor3508 : public MotorCANBase {
public:
  /* constructor wrapper over MotorCANBase */
  Motor3508(CANRAW::CAN *can, uint16_t rx_id);
  /* implements data update callback */
  void UpdateData(const uint8_t data[]) override final;
  /* implements data printout */
  void PrintData() const override final;
  /* override base implementation with max current protection */
  void SetOutput(int16_t val) override final;

  int16_t GetCurr() const override final;

  uint16_t GetTemp() const override final;

private:
  volatile int16_t raw_current_get_ = 0;
  volatile uint8_t raw_temperature_ = 0;
};

//==================================================================================================
// Motor6020
//==================================================================================================

/**
 * @brief DJI 6020 motor class
 */
class Motor6020 : public MotorCANBase {
public:
  /* constructor wrapper over MotorCANBase */
  Motor6020(CANRAW::CAN *can, uint16_t rx_id);
  /* implements data update callback */
  void UpdateData(const uint8_t data[]) override final;
  /* implements data printout */
  void PrintData() const override final;
  /* override base implementation with max current protection */
  void SetOutput(int16_t val) override final;

  int16_t GetCurr() const override final;

  uint16_t GetTemp() const override final;

private:
  volatile int16_t raw_current_get_ = 0;
  volatile uint8_t raw_temperature_ = 0;
};

} // namespace control

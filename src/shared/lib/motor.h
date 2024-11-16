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

#ifndef MOTOR_H
#define MOTOR_H

#pragma once
#include "can.h"

namespace control
{
  //==================================================================================================
  // MotorBase
  //==================================================================================================

  /**
   * @brief base class for motor representation
   */
  class MotorBase
  {
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

  class MotorDJICanBase : public MotorBase
  {
  public:
    /**
     * @brief base constructor
     *
     * @param can    CAN instance
     * @param rx_id  CAN rx id
     */
    MotorDJICanBase(CANRAW::Can *can, uint16_t rx_id);

  

  };
}
#endif MOTOR_H
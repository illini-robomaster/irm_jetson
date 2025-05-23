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
#ifndef MAIN_H
#define MAIN_H

#define UNUSED(X) (void)X // To avoid gcc/g++ warnings

#define SERIAL_SSIZE_READMAX _POSIX_SSIZE_MAX
#define SERIAL_SSIZE_WRITEMAX _POSIX_SSIZE_MAX

#define SERIAL_DIRNAME "/dev/serial/by-id/"
#define INPUT_DIRNAME "/dev/input/by-id/"

#define PREF_CBOARD "usb-STMicroelectronics_STM32_STLink_"

#endif

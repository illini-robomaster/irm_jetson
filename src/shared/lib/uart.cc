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
#include <iostream>
#include <wiringSerial.h>

int main()
{
  int fd;

  // Open the serial port
  fd = serialOpen("/dev/ttyACM0", 115200); // Adjust the port and baud rate as needed

  if (fd < 0)
  {
    std::cerr << "Unable to open serial port" << std::endl;
    return 1;
  }

  // Write data to the serial port
  serialPuts(fd, "Hello from Jetson Orin Nano!\n");

  // Read data from the serial port
  while (serialDataAvail(fd))
  {
    char c = serialGetchar(fd);
    std::cout << c;
  }

  // Close the serial port
  serialClose(fd);

  return 0;
}
# ---------------------------------------------------------------------- #
#                                                                        #
#  Copyright (C) 2024                                                    #
#  Illini RoboMaster @ University of Illinois at Urbana-Champaign.       #
#                                                                        #
#  This program is free software: you can redistribute it and/or modify  #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation, either version 3 of the License, or     #
#  (at your option) any later version.                                   #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#  GNU General Public License for more details.                          #
#                                                                        #
#  You should have received a copy of the GNU General Public License     #
#  along with this program. If not, see <http://www.gnu.org/licenses/>.  #
#                                                                        #
# ---------------------------------------------------------------------- #

project(examples CXX)
set(INC ${CMAKE_SOURCE_DIR}/src/include)

FILE(GLOB BOARD ${INC}/board/*.cc)
FILE(GLOB MOTOR ${INC}/motor/*.cc)

# can_send
irm_add_executable(${PROJECT_NAME}_can_send
  SOURCES "can_send.cc" ${BOARD}
)

# can_recieve
irm_add_executable(${PROJECT_NAME}_can_recieve
  SOURCES "can_recieve.cc" ${BOARD} ${MOTOR}
)

# motor_m3508
irm_add_executable(${PROJECT_NAME}_motor_m3508
  SOURCES "motor_m3508.cc" ${BOARD} ${MOTOR}
)

# uart_minipc
irm_add_executable(${PROJECT_NAME}_uart_minipc
  SOURCES "uart_minipc.cc" ${BOARD}
)

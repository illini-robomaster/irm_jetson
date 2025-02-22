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

# test_can
add_custom_executable(${PROJECT_NAME}_can
  SOURCES "can.cc" ${BOARD}
)

# test_motor
add_custom_executable(${PROJECT_NAME}_motor_m3508
  SOURCES "motor_m3508.cc" ${BOARD} ${MOTOR}
)

# test_can_feedback
add_custom_executable(${PROJECT_NAME}_can_feedback
  SOURCES "feedback_frame.cc" ${BOARD} ${MOTOR}
)

# test_minipc_packet
add_custom_executable(${PROJECT_NAME}_minipc_packet
  SOURCES "create_minipc_packet.cc" ${BOARD}
)

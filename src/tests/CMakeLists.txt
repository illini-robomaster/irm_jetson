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

project(test CXX)
set(INC ${CMAKE_SOURCE_DIR}/src/include)

FILE(GLOB BOARD ${INC}/board/*.cc)
FILE(GLOB MOTOR ${INC}/motor/*.cc)

# test_can
add_executable(${PROJECT_NAME}_can)
target_sources(${PROJECT_NAME}_can PRIVATE
    "can.cc"
    ${BOARD}
)
target_include_directories(${PROJECT_NAME}_can PRIVATE ${INC})

# test_motor
add_executable(${PROJECT_NAME}_motor_m3508)
target_sources(${PROJECT_NAME}_motor_m3508 PRIVATE
    "motor_m3508.cc"
    ${BOARD}
    ${MOTOR}
)
target_include_directories(${PROJECT_NAME}_motor_m3508 PRIVATE ${INC})

# test_can_feedback
add_executable(${PROJECT_NAME}_can_feedback)
target_sources(${PROJECT_NAME}_can_feedback PRIVATE
    "feedback_frame.cc"
    ${BOARD}
    ${MOTOR}
)
target_include_directories(${PROJECT_NAME}_can_feedback PRIVATE ${INC})
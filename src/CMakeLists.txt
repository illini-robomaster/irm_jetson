# ---------------------------------------------------------------------- #
#                                                                        #
#  Copyright (C) 2025                                                    #
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

set(INC ${CMAKE_SOURCE_DIR}/src/include)

file(GLOB BOARD ${INC}/board/*.cc)
file(GLOB MOTOR ${INC}/motor/*.cc)
file(GLOB CAMERA ${INC}/camera/*.cc)

add_subdirectory(examples)
add_subdirectory(vehicles)

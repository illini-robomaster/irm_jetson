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

cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER "/bin/gcc")
set(CMAKE_CXX_COMPILER "/bin/g++")

project(iRM_Jetson CXX)

include(cmake/builder.cmake)
include(cmake/scripts.cmake)  # Hack

add_subdirectory(src)
add_subdirectory(scripts)

file(COPY "scripts" DESTINATION ".")
file(COPY "python" DESTINATION ".")

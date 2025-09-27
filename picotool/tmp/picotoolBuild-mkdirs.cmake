# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/_deps/picotool-src"
  "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/_deps/picotool-build"
  "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/_deps"
  "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/picotool/tmp"
  "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/picotool/src/picotoolBuild-stamp"
  "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/picotool/src"
  "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/khoi-anh-nguyen/Projects/PSR/Pico_CANLib/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()

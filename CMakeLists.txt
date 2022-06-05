# global config
cmake_minimum_required(VERSION 3.16)

project(piStatTracker)

# project wide defines
## config defines
set(currentPreset ${CMAKE_BUILD_TYPE})
if(currentPreset STREQUAL "Debug")
  add_definitions(-DSTATS_DEBUG)
elseif(currentPreset STREQUAL "Release")
  add_definitions(-DSTATS_RELEASE)
elseif(currentPreset STREQUAL "MinSizeRel")
  add_definitions(-DSTATS_DIST)
endif()

## os defines
if(WIN32)
  add_definitions(-DSTATS_WINDOWS)
elseif(UNIX)
  if(APPLE)
    add_definitions(-DSTATS_MACOS)
  else()
    add_definitions(-DSTATS_LINUX)
  endif()
endif()
# project wide defines

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

set(vendorPath
    "${PROJECT_SOURCE_DIR}/vendor"
)

set(Networking_DIR
    "${PROJECT_SOURCE_DIR}/networking"
)

add_subdirectory(vendor)
add_subdirectory(core)
#add_subdirectory(networking)
add_subdirectory(statTest)
add_subdirectory(sender)
add_subdirectory(reciever)

# https://github.com/pi-hole/AdminLTE/blob/v5.11/scripts/pi-hole/php/header.php

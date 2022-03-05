# global config
cmake_minimum_required(VERSION 3.16)

project(piStatTracker)

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
add_subdirectory(networking)
add_subdirectory(statTest)
add_subdirectory(sender)
add_subdirectory(reciever)

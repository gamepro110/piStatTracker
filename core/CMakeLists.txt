# core lib config

file(
  GLOB_RECURSE
  Core_SRC
  "src/*.cpp"
  "src/*.h"
)

add_library(
  CoreLib
  STATIC
  ${Core_SRC}
)

set_target_properties(
  CoreLib
  PROPERTIES
  VERSION 0.1
  POSITION_INDEPENDENT_CODE ON
  CXX_STANDARD 20
  FOLDER "Pi Stat Tracker"
)

target_link_libraries(
  CoreLib
  PUBLIC
  yaml-cpp
)

target_include_directories(
  CoreLib
  PUBLIC
  "${PROJECT_SOURCE_DIR}/core/src/"
  "${vendorPath}/spdlog/include/"
  "${vendorPath}/asio/asio/include/"
  "${vendorPath}/yaml-cpp/include/"
)

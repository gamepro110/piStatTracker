# vendor config

# assio
file(
  GLOB_RECURSE
  ASIO_SRC
  "${vendorPath}/asio/asio/include/*.hpp"
)

add_library(
  Asio
  STATIC
  "${ASIO_SRC}"
)

set_target_properties(
  Asio
  PROPERTIES
  LINKER_LANGUAGE CXX
  FOLDER "NetLib"
)

# ImGuiWindow
add_subdirectory(imguiWindow)

# yaml-cpp
add_subdirectory(yaml-cpp)

set_target_properties(
  yaml-cpp
  yaml-cpp-parse
  yaml-cpp-read
  yaml-cpp-sandbox
  PROPERTIES
  FOLDER "YAML-CPP"
)

# spdlog
add_subdirectory(spdlog)

set_target_properties(
  spdlog
  PROPERTIES
  FOLDER "spdlog"
)

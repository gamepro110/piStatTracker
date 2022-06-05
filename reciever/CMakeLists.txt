# reciever app config

set(
  ImGuiStyle
  "Dark"
  CACHE
  STRING
  ""
  FORCE
)

file(
    GLOB
    RECIEVER_SRC
    "src/*.cpp"
    "src/*.h"
)

add_executable(
    piStatReciever
    "${RECIEVER_SRC}"
)

set_target_properties(
    piStatReciever
    PROPERTIES
    VERSION 0.1
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 20
    FOLDER "Pi Stat Tracker"
)

target_link_libraries(
    piStatReciever
    PUBLIC
    CoreLib
    ImGUIWindow_Lib
)

target_include_directories(
    piStatReciever
    PUBLIC
    "${PROJECT_SOURCE_DIR}/core/src/"
    "${vendorPath}/imguiWindow/Lib/src/"
)

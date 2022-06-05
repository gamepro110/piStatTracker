# sender app config
file(
 GLOB
    SENDER_SRC
    "src/*.cpp"
    "src/*.h"
)

add_executable(
    piStatSender
    "${SENDER_SRC}"
)

set_target_properties(
    piStatSender
    PROPERTIES
    VERSION 0.1
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 20
    FOLDER "Pi Stat Tracker"
)

target_link_libraries(
    piStatSender
    PUBLIC
    CoreLib
)

target_include_directories(
    piStatSender
    PUBLIC
    "${PROJECT_SOURCE_DIR}/core/src/"
)

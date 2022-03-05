# reciever app config
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
    Networking
    ncurses
)

target_include_directories(
    piStatReciever
    PUBLIC
    "${Networking_DIR}/"
)
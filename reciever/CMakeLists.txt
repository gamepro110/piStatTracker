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
    "${CURSES_INCLUDE_DIRS}"
)
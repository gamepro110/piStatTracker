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

target_link_libraries(
    piStatSender
    PUBLIC
    Networking
)

target_include_directories(
    piStatSender
    PUBLIC
    "${Networking_DIR}/"
)
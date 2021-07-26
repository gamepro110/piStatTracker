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
)

target_link_directories(
    piStatSender
    PUBLIC
)
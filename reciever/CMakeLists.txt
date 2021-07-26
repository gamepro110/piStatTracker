file(
    GLOB
    RECIEVER
    "src/*.cpp"
    "src/*.h"
)

add_executable(
    piStatReciever
    "${RECIEVER}"
)

target_link_libraries(
    piStatReciever
    PUBLIC
)

target_link_directories(
    piStatReciever
    PUBLIC
)
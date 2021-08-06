# reciever app config
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
    Networking
)
    
target_include_directories(
    piStatReciever
    PUBLIC
    "${Networking_DIR}/"
)
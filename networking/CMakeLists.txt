# networking lib config
file(
    GLOB
    Networking_SRC
    "Networking/*.cpp"
    "Networking/*.h"
)

add_library(
    Networking
    STATIC
    "${Networking_SRC}"
)

set_target_properties(
    Networking
    PROPERTIES
    VERSION 0.1
    CXX_STANDARD 20
    FOLDER "NetLib"
)

target_link_libraries(
    Networking
    PUBLIC
    Threads::Threads
)

target_include_directories(
    Networking
    PUBLIC
    "${vendorPath}/asio/asio/include/"
)
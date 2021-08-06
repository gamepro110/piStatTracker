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

target_link_libraries(
    Networking
    PUBLIC
    Asio
    Threads::Threads
)

target_include_directories(
    Networking
    PUBLIC
    "${vendorPath}/asio/asio/include/"
)
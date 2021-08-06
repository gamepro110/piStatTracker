# vendor config

#assio
file(
    GLOB_RECURSE
    ASIO_SRC
    "${vendorPath}/asio/asio/include/*.hpp"
)

add_library(
    Asio
    STATIC
    "${ASIO_SRC}"
)

set_target_properties(
    Asio
    PROPERTIES
    LINKER_LANGUAGE
    CXX
)
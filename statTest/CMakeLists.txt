# statTest config

file(
  GLOB_RECURSE
  STATTEST_SRC
  "src/*.cpp"
  "src/*.h"
)

# Rework this into a cleaned up library
add_executable(
  statTest
  "${STATTEST_SRC}"
)

set_target_properties(
    statTest
    PROPERTIES
    VERSION 0.1
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 20
    FOLDER "Pi Stat Tests"
)

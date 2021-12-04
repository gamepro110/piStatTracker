# statTest config
file(
	GLOB
	STATTEST_SRC
	"src/*.cpp"
	"src/*.h"
)

add_executable(
	statTest
	"${STATTEST_SRC}"
)

# Rework this into a cleaned up library
# Add all source files
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS src/*.cpp)

# Add source to this project's executable.
add_library(twio STATIC ${SRC_LIST} "include/twio/core/AdvancedReader.h" "include/twio/stream/RedirectProtocol.h")
target_include_directories(twio PUBLIC include)
target_compile_definitions(twio PUBLIC ENABLE_ASSERT)

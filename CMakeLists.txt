cmake_minimum_required(VERSION 3.8)
project(TinyWrappedIO)

set(CMAKE_CXX_STANDARD 17)

# Add all source files
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS src/*.cpp)

# Add source to this project's executable.
add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})

add_subdirectory(twio)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE twio)

cmake_minimum_required(VERSION 3.24)
project(ArrowMock)

set(CMAKE_CXX_STANDARD 17)

# Find the installed Arrow package
find_package(Arrow REQUIRED)

# Define our executable
add_executable(arrow_mock src/main.cpp)

# Link Arrow to our executable
target_link_libraries(arrow_mock PRIVATE Arrow::arrow_shared)
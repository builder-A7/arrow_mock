cmake_minimum_required(VERSION 3.24)
project(ArrowMock)

set(CMAKE_CXX_STANDARD 17)

# 1. Find the main packages (No components asked for here to avoid the error)
find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)

# 2. MANUAL OVERRIDE: Find the compute library directly
# We know it exists in /opt/homebrew/lib, so we tell CMake to go get it.
find_library(ARROW_COMPUTE_LIB 
    NAMES arrow_compute 
    PATHS /opt/homebrew/lib
    REQUIRED
)

add_executable(arrow_mock src/main.cpp)

# 3. Link everything
target_link_libraries(arrow_mock PRIVATE 
    Arrow::arrow_shared
    ${ARROW_COMPUTE_LIB}      # <--- We link the specific file we found
    Parquet::parquet_shared
)
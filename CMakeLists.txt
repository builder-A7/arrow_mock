cmake_minimum_required(VERSION 3.24)
project(ArrowMock)

set(CMAKE_CXX_STANDARD 17)

# 1. Find the libraries
find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)  # <--- NEW: Find the Parquet package

add_executable(arrow_mock src/main.cpp)

# 2. Link the libraries
target_link_libraries(arrow_mock PRIVATE 
    Arrow::arrow_shared
    Parquet::parquet_shared     # <--- NEW: Link the Parquet library
)
cmake_minimum_required(VERSION 3.24)
project(ArrowConverter)  # New project name!

set(CMAKE_CXX_STANDARD 17)

find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)

add_executable(csv_converter src/main.cpp)

target_link_libraries(csv_converter PRIVATE 
    Arrow::arrow_shared
    Parquet::parquet_shared
)
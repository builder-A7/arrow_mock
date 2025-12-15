cmake_minimum_required(VERSION 3.24)
project(ArrowMock)

set(CMAKE_CXX_STANDARD 17)

find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)

add_executable(arrow_mock src/main.cpp)

# Link the libraries (Spaces only, correct namespaces)
target_link_libraries(arrow_mock PRIVATE 
    Arrow::arrow_shared
    Arrow::arrow_compute_shared
    Parquet::parquet_shared
)
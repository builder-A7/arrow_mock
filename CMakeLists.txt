cmake_minimum_required(VERSION 3.24)
project(LogAnalyzer)

set(CMAKE_CXX_STANDARD 17)

find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)

add_executable(log_analyzer src/main.cpp)

target_link_libraries(log_analyzer PRIVATE 
    Arrow::arrow_shared
    Parquet::parquet_shared
)
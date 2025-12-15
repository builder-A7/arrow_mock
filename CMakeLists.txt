cmake_minimum_required(VERSION 3.24)
project(StockMarket)

set(CMAKE_CXX_STANDARD 17)

find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)

# Name the output executable "stock_market"
add_executable(stock_market src/main.cpp)

target_link_libraries(stock_market PRIVATE 
    Arrow::arrow_shared
    Parquet::parquet_shared
)
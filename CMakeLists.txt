cmake_minimum_required(VERSION 3.26)
project(LibraryCheckout)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LibraryCheckout
        main.cpp
        Item.cpp
        LibraryStorage.cpp
)

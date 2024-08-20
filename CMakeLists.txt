cmake_minimum_required(VERSION 3.27)
project(NestNetwork)

set(CMAKE_CXX_STANDARD 20)
option(ENET_STATIC "Enet static" ON)

add_subdirectory(Vendor/enet)

add_executable(App Server.hpp Server.cpp BaseNetwork.hpp Client.hpp Client.cpp main.cpp)
target_link_libraries(App enet_static)
target_include_directories(App PRIVATE Vendor/enet/include)

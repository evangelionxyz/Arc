cmake_minimum_required(VERSION 3.29)
project(Arcade)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_DIR ${ROOT_DIR}/third_party)

add_subdirectory(third_party/raylib)
add_subdirectory(arc)
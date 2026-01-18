cmake_minimum_required(VERSION 3.30)
project(HytaleImGui LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(cmake/FetchImgui.cmake)
include(cmake/FetchFunchook.cmake)

add_library(HytaleImGui SHARED
  src/utils/Console.cc
  src/HytaleImGui.cpp
  src/utils/Memory.cc
  src/sdk/GDI32.cc
  src/Hooks.cc
  src/utils/ProcessHelper.cc
)

target_include_directories(HytaleImGui PRIVATE ./src)

target_link_libraries(HytaleImGui
  PRIVATE
  funchook-static
  distorm
  psapi
  opengl32
  gdi32
  imgui
)

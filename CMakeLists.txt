cmake_minimum_required(VERSION 3.10)
project(HytaleImGui LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

if (MSVC)
  add_compile_options($<$<CONFIG:Debug>:/Z7> $<$<CONFIG:Debug>:/FS>)
endif()

FetchContent_Declare(
  funchook
  GIT_REPOSITORY https://github.com/kubo/funchook.git
  GIT_TAG v1.1.3
)

FetchContent_MakeAvailable(funchook)

set(FUNCHOOK_BUILD_SHARED OFF CACHE BOOL "Disable building shared library for funchook" FORCE)
set(FUNCHOOK_BUILD_TESTS  OFF CACHE BOOL "Disable building tests for funchook" FORCE)
set(FUNCHOOK_INSTALL      OFF CACHE BOOL "Disable installation for funchook" FORCE)

file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/common/imgui/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/common/imgui/backends/*.cpp"
)

add_library(HytaleImGui SHARED
  ${IMGUI_SOURCES}

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
)
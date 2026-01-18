project(client)

include(FetchContent)
include(cmake/FetchImgui.cmake)
include(cmake/FetchFunchook.cmake)

add_library(client SHARED
    Dllmain.cc
    Hooks.cc

    utils/Console.cc
    utils/Memory.cc
    utils/ProcessHelper.cc

    sdk/GDI32.cc

    modules/ImGuiModule.cc
)


target_link_libraries(client PRIVATE
  funchook-static
  distorm
  psapi
  opengl32
  gdi32
  imgui
)
target_include_directories(client PRIVATE ./)

FetchContent_Declare(
  funchook
  GIT_REPOSITORY https://github.com/kubo/funchook.git
  GIT_TAG v1.1.3
)

FetchContent_MakeAvailable(funchook)

set(FUNCHOOK_BUILD_SHARED OFF CACHE BOOL "Disable building shared library for funchook" FORCE)
set(FUNCHOOK_BUILD_TESTS  OFF CACHE BOOL "Disable building tests for funchook" FORCE)
set(FUNCHOOK_INSTALL      OFF CACHE BOOL "Disable installation for funchook" FORCE)
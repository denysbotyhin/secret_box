cmake_minimum_required(VERSION 3.31.3)

project(IndigoTestTask VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(gtest)

set(EXECUTABLE_NAME secret_box)
set(LIB_SECRET_BOX secret_box_lib.a)
set(LIB_SECRET_BOX_HACK secret_box_hack.a)

add_subdirectory(tests)
add_subdirectory(src)
add_subdirectory(app)

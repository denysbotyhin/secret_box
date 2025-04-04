cmake_minimum_required(VERSION 3.31.3)

project(SecureBoxTestTask VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_LTO "Enable to add Link Time Optimization." ON)
option(ENABLE_WARNINGS "Enable to add warnings to a target." OFF)
option(ENABLE_SANITIZE_ADDR "Enable to add warnings to a target." ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(ConfigSafeGuards)
include(LTO)
include(CTest)
include(Sanitizer)

add_sanitizer_flags()
enable_testing()

include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(gtest)

if(ENABLE_WARNINGS)
    include(Warnings)
endif()

set(EXECUTABLE_NAME secret_box)
set(LIB_SECRET_BOX secret_box_lib.a)
set(LIB_SECRET_BOX_HACK secret_box_hack.a)

add_subdirectory(tests)
add_subdirectory(src)
add_subdirectory(app)

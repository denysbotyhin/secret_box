include(CTest)
include(GoogleTest)
enable_testing()

set(TESTS_EXECUTABLE_NAME secret_box_tests)

add_executable(${TESTS_EXECUTABLE_NAME} secret_box.cpp)

set_target_properties(${TESTS_EXECUTABLE_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/archive/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${TESTS_EXECUTABLE_NAME} PRIVATE BUILD_TYPE_DEBUG=1)
endif()
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_compile_definitions(${TESTS_EXECUTABLE_NAME} PRIVATE BUILD_TYPE_RELEASE=1)
endif()

target_link_libraries(${TESTS_EXECUTABLE_NAME} PUBLIC
    ${LIB_SECRET_BOX}
    ${LIB_SECRET_BOX_HACK}
    GTest::gtest_main)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${TESTS_EXECUTABLE_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

gtest_add_tests(
    TARGET ${TESTS_EXECUTABLE_NAME}
    SOURCES secret_box.cpp)
gtest_discover_tests(${TESTS_EXECUTABLE_NAME} DISCOVERY_MODE PRE_TEST)

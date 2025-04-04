set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/BoxHack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/helpers.cpp")
set(LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/BoxHack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/DynamicBitset.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/helpers.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/types.h")
set(LIBRARY_INCLUDES "./includes" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(${LIB_SECRET_BOX_HACK} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

set_target_properties(${LIB_SECRET_BOX}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/archive/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

target_include_directories(${LIB_SECRET_BOX_HACK} PUBLIC ${LIBRARY_INCLUDES})

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${LIB_SECRET_BOX_HACK}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

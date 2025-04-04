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
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/archive"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(${LIB_SECRET_BOX_HACK} PUBLIC ${LIBRARY_INCLUDES})

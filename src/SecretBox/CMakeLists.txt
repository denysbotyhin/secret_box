set(LIBRARY_SOURCES "")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/includes/SecureBox.h")
set(LIBRARY_INCLUDES "./includes" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(${LIB_SECRET_BOX} INTERFACE ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${LIB_SECRET_BOX} INTERFACE ${LIBRARY_INCLUDES})

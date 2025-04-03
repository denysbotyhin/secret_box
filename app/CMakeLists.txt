set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC 
    ${LIB_SECRET_BOX}
    ${LIB_SECRET_BOX_HACK})

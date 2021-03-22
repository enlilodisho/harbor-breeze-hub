include_directories(${PROJECT_SOURCE_DIR}/include)

set(Headers
    
)
set(Sources
    main.cpp
)

# Main library
add_library(${LIBRARY_NAME} STATIC ${Sources} ${Headers})

# Main executable
add_executable(${EXECUTABLE_NAME} ${Sources} ${Headers})
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
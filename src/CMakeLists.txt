set(IncludeDir ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
include_directories(${IncludeDir})

set(Headers
    ${IncludeDir}/FanManager.h
)
set(Sources
    FanManager.cpp
    main.cpp
)

# Main library
add_library(${LIBRARY_NAME} STATIC ${Sources} ${Headers})
target_link_libraries(${LIBRARY_NAME} PUBLIC ComponentEventSystem)

# Main executable
add_executable(${EXECUTABLE_NAME} ${Sources} ${Headers})
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
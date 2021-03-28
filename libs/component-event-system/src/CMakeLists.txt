set(IncludeDir ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
include_directories(${IncludeDir})

set(Headers
    ${IncludeDir}/Component.h
    ${IncludeDir}/ComponentMaster.h
    ${IncludeDir}/Result.h
)
set(Sources
    ComponentMaster.cpp
)

# Require multithreading package
find_package(Threads REQUIRED)

# Main library
add_library(${LIBRARY_NAME} STATIC ${Sources} ${Headers})
target_link_libraries(${LIBRARY_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
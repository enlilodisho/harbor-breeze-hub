include_directories(${PROJECT_SOURCE_DIR}/include)

set(Headers
    ../include/Component.h
    ../include/ComponentMaster.h
    ../include/Result.h
)
set(Sources
    ComponentMaster.cpp
)

# Main library
add_library(${LIBRARY_NAME} STATIC ${Sources} ${Headers})
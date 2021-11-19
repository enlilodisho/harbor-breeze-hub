set(IncludeDir ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
include_directories(${IncludeDir})

set(Headers
    ${IncludeDir}/FanManager.h
    ${IncludeDir}/InterruptsController.h
    ${IncludeDir}/RFDataReceivedEvent.h
    ${IncludeDir}/RFDataTransmittedEvent.h
    ${IncludeDir}/RFReceiver.h
    ${IncludeDir}/RFTransmitter.h
    ${IncludeDir}/SSLLParser.h
)
set(Sources
    FanManager.cpp
    InterruptsController.cpp
    RFReceiver.cpp
    RFTransmitter.cpp
    SSLLParser.cpp
    main.cpp
)

# Require & Include WiringPi package
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")
find_package(WiringPi REQUIRED)
include_directories(${WIRINGPI_INCLUDE_DIRS})

# Main library
add_library(${LIBRARY_NAME} STATIC ${Sources} ${Headers})
target_link_libraries(${LIBRARY_NAME} PUBLIC ComponentEventSystem ${WIRINGPI_LIBRARIES})

# Main executable
add_executable(${EXECUTABLE_NAME} ${Sources} ${Headers})
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
set(IncludeDir ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
include_directories(${IncludeDir})

set(Headers
    ${IncludeDir}/FanController.h
    ${IncludeDir}/FanState.h
    ${IncludeDir}/HttpsApiServer.h
    ${IncludeDir}/HubConfig.h
    ${IncludeDir}/HubFactory.h
    ${IncludeDir}/InterruptsController.h
    ${IncludeDir}/RFDataReceivedEvent.h
    ${IncludeDir}/RFDataTransmittedEvent.h
    ${IncludeDir}/RFReceiver.h
    ${IncludeDir}/RFTransmitter.h
    ${IncludeDir}/SSLLDecoder.h
    ${IncludeDir}/SSLLEncoder.h
    ${IncludeDir}/SSLLMessageBeginReceiveEvent.h
    ${IncludeDir}/SSLLMessageEndReceiveEvent.h
    ${IncludeDir}/TransmitRFDataBeginEvent.h
    ${IncludeDir}/TransmitRFDataEndEvent.h
    ${IncludeDir}/TransmitRFDataRequestEvent.h
    ${IncludeDir}/UpdateFanLightRequestEvent.h
    ${IncludeDir}/UpdateFanPowerRequestEvent.h
    ${IncludeDir}/UpdateFanRotationRequestEvent.h
    ${IncludeDir}/UpdateFanSpeedRequestEvent.h
)
set(Sources
    FanController.cpp
    HttpsApiServer.cpp
    InterruptsController.cpp
    RFReceiver.cpp
    RFTransmitter.cpp
    SSLLDecoder.cpp
    SSLLEncoder.cpp
    main.cpp
)

# Require & Include WiringPi package
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")
find_package(WiringPi REQUIRED)
include_directories(${WIRINGPI_INCLUDE_DIRS})

# Main library
add_library(${LIBRARY_NAME} STATIC ${Sources} ${Headers})
target_link_libraries(${LIBRARY_NAME} PUBLIC ComponentEventSystem httplib::httplib tinyxml2 ${WIRINGPI_LIBRARIES})

# Main executable
add_executable(${EXECUTABLE_NAME} ${Sources} ${Headers})
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
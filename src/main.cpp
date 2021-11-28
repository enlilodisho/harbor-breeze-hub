#include "core.h"
#include "ComponentEventSystem/ComponentMaster.h"
#include "HubConfig.h"
#include "HubFactory.h"

#include <HarborBreezeHubConfig.h>
#include <csignal>
#include <iostream>
#include <wiringPi.h>

#define HUB_CONFIG_FILE     "hub_config.xml"

std::unique_ptr<ComponentMaster> componentMaster_;

void interruptSignalHandler(int s)
{
    std::cout << "Interrupt signal received" << std::endl;
    // Destroy component master.
    // Hub will automatically stop once ComponentMaster is destroyed (see end of main function).
    if (componentMaster_)
    {
        componentMaster_.reset(nullptr);
    }
}

int main(int argc, char** argv)
{
    std::cout << "Harbor Breeze Hub - v" << HARBOR_BREEZE_HUB_VERSION_MAJOR
              << "." << HARBOR_BREEZE_HUB_VERSION_MINOR
              << "." << HARBOR_BREEZE_HUB_VERSION_PATCH << std::endl;

    // Set up program interrupt signal handler
    struct sigaction interruptHandler{};
    interruptHandler.sa_handler = interruptSignalHandler;
    sigemptyset(&interruptHandler.sa_mask);
    interruptHandler.sa_flags = 0;
    sigaction(SIGINT, &interruptHandler, nullptr);

    // Initialize wiringPi to use BCM pin numbers
    wiringPiSetupGpio();

    // Construct the component master
    componentMaster_ = std::make_unique<ComponentMaster>();

    // Parse hub config xml file
    HubConfig hubConfig;
    Result r = HubFactory::parseHubConfigXml(HUB_CONFIG_FILE, &hubConfig);
    if (!r.success)
    {
        std::cerr << r.message << "\n";
        return -1;
    }
    // Add all hub components to ComponentMaster based on the xml config file
    HubFactory::addHubComponentsFromConfig(*componentMaster_, hubConfig);

    // Start all components
    componentMaster_->start();

    while (componentMaster_ != nullptr); // Keep main thread alive while ComponentMaster is available
    std::cout << "Harbor Breeze Hub Stopped" << std::endl;
    return 0;
}
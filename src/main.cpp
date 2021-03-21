#include <HarborBreezeHubConfig.h>
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Harbor Breeze Hub - v" << HARBOR_BREEZE_HUB_VERSION_MAJOR
              << "." << HARBOR_BREEZE_HUB_VERSION_MINOR
              << "." << HARBOR_BREEZE_HUB_VERSION_PATCH << std::endl;
    return 0;
}
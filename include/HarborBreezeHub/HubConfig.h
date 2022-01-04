//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_HUBCONFIG_H
#define HARBORBREEZEHUB_HUBCONFIG_H

#include <string>
#include <unordered_map>

struct HttpsApiServerConfig
{
    int port_;
    std::string certificate_;
    std::string privateKey_;
};
struct RFReceiverConfig
{
    int pin_;
};
struct RFTransmitterConfig
{
    int pin_;
};
struct FanConfig
{
    std::string remoteId_;
};
struct HubConfig
{
    std::unordered_map<std::string, HttpsApiServerConfig> httpsApiServers_;
    std::unordered_map<std::string, RFReceiverConfig> rfReceivers_;
    std::unordered_map<std::string, RFTransmitterConfig> rfTransmitters_;
    std::unordered_map<std::string, FanConfig> fans_;
};

#endif //HARBORBREEZEHUB_HUBCONFIG_H

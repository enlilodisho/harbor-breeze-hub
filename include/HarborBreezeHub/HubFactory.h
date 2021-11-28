//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_HUBFACTORY_H
#define HARBORBREEZEHUB_HUBFACTORY_H

#define HUB_CONFIG_ROOT_NAME    "HarborBreezeHub"

#include "core.h"
#include "HubConfig.h"
#include "RFReceiver.h"
#include "SSLLParser.h"
#include "FanController.h"
#include "RFTransmitter.h"

#include <ComponentEventSystem/ComponentMaster.h>
#include <boost/filesystem.hpp>
#include <tinyxml2.h>
#include <unordered_map>

#include <iostream>

class HubFactory
{
public:
    static Result parseHubConfigXml(const std::string& configXmlFilePath, HubConfig* config)
    {
        if (!boost::filesystem::exists(configXmlFilePath))
        {
            return Result(false, "Xml file path (%s) does not exist.",
                          configXmlFilePath.c_str());
        }
        tinyxml2::XMLDocument configXml;
        tinyxml2::XMLError xmlResult = configXml.LoadFile(configXmlFilePath.c_str());
        if (xmlResult != tinyxml2::XML_SUCCESS)
        {
            return Result(false, "Xml load error");
        }

        auto hubRootElement = configXml.FirstChildElement(HUB_CONFIG_ROOT_NAME);
        if (hubRootElement)
        {
            for (auto xmlElement = hubRootElement->FirstChildElement(); xmlElement != nullptr;
                 xmlElement = xmlElement->NextSiblingElement())
            {
                if (strcmp(xmlElement->Name(), "RFReceiver") == 0)
                {
                    RFReceiverConfig rfReceiverConfig{};
                    // Get name of rf receiver
                    auto name_c_str = xmlElement->Attribute("name");
                    if (name_c_str == nullptr)
                    {
                        return Result(false, "Error parsing RFReceiver in config. Cannot find name attribute.");
                    }
                    if (config->rfReceivers_.find(name_c_str) != config->rfReceivers_.end())
                    {
                        return Result(false, "Config already contains RFReceiver with name %s", name_c_str);
                    }
                    // Get pin of rf receiver
                    auto pin_c_str = xmlElement->Attribute("pin");
                    try
                    {
                        rfReceiverConfig.pin_ = std::stoi(pin_c_str);
                    }
                    catch (std::exception& ex)
                    {
                        return Result(false, "Error parsing RFReceiver in config: %s", ex.what());
                    }
                    // Add rf receiver to config
                    config->rfReceivers_[name_c_str] = rfReceiverConfig;
                }
                else if (strcmp(xmlElement->Name(), "RFTransmitter") == 0)
                {
                    RFTransmitterConfig rfTransmitterConfig{};
                    // Get name of rf receiver
                    auto name_c_str = xmlElement->Attribute("name");
                    if (name_c_str == nullptr)
                    {
                        return Result(false, "Error parsing RFTransmitter in config. Cannot find name attribute.");
                    }
                    if (config->rfTransmitters_.find(name_c_str) != config->rfTransmitters_.end())
                    {
                        return Result(false, "Config already contains RFTransmitter with name %s", name_c_str);
                    }
                    // Get pin of rf transmitter
                    auto pin_c_str = xmlElement->Attribute("pin");
                    try
                    {
                        rfTransmitterConfig.pin_ = std::stoi(pin_c_str);
                    }
                    catch (std::exception& ex)
                    {
                        return Result(false, "Error parsing RFTransmitter in config: %s", ex.what());
                    }
                    // Add rf transmitters to config
                    config->rfTransmitters_[name_c_str] = rfTransmitterConfig;
                }
                else if (strcmp(xmlElement->Name(), "fans") == 0)
                {
                    for (auto fanElement = xmlElement->FirstChildElement(); fanElement != nullptr;
                         fanElement = fanElement->NextSiblingElement())
                    {
                        if (strcmp(fanElement->Name(), "fan") != 0)
                        {
                            return Result(false, "Fans element contains invalid child element: %s", fanElement->Name());
                        }
                        FanConfig fanConfig{};
                        // Get name of fan
                        auto name_c_str = fanElement->Attribute("name");
                        if (name_c_str == nullptr)
                        {
                            return Result(false, "Error parsing fan in config. Cannot find name attribute.");
                        }
                        if (config->fans_.find(name_c_str) != config->fans_.end())
                        {
                            return Result(false, "Config already contains fan with name %s", name_c_str);
                        }
                        // Get remote id of fan
                        auto remote_id_c_str = fanElement->Attribute("remoteId");
                        if (remote_id_c_str == nullptr)
                        {
                            return Result(false, "Error parsing fan in config. Cannot find remoteId attribute.");
                        }
                        fanConfig.remoteId_ = remote_id_c_str;
                        // Add fan to config
                        config->fans_[name_c_str] = fanConfig;
                    }
                }
            }
        }
        else
        {
            return Result(false, "Could not find config root element (%s) in xml", HUB_CONFIG_ROOT_NAME);
        }

        return Result(true);
    }

    static Result addHubComponentsFromConfig(ComponentMaster& componentMaster, const HubConfig& config)
    {
        // Create FanController component to manage all fans
        std::unique_ptr<FanController> fanController = std::make_unique<FanController>("Hub-FanController", config.fans_);

        // Register all RFReceivers
        for (const auto& it : config.rfReceivers_)
        {
            std::unique_ptr<RFReceiver> rfReceiver = std::make_unique<RFReceiver>(it.first, it.second.pin_);

            // TODO create ssllParser using arguments from xml
            std::unique_ptr<SSLLParser> ssllParser = std::make_unique<SSLLParser>("SSLLParser-" + it.first,
                                                                                  60, 400, 500, 850, 950, 10400);
            componentMaster.getEventDispatcher().subscribe(ssllParser.get(), "RFDataReceivedEvent", rfReceiver.get());
            componentMaster.getEventDispatcher().subscribe(fanController.get(), "SSLLMessageBeginReceiveEvent", ssllParser.get());
            componentMaster.getEventDispatcher().subscribe(fanController.get(), "SSLLMessageEndReceiveEvent", ssllParser.get());

            // Add RFReceiver and relevant components to ComponentMaster
            if (!componentMaster.addComponent(std::move(rfReceiver)).success)
            {
                return Result(false, "Failed to add RFReceiver to component master");
            }
            if (!componentMaster.addComponent(std::move(ssllParser)).success)
            {
                return Result(false, "Failed to add SSLLParser to component master");
            }
        }

        // Register all RFTransmitters
        for (const auto& it : config.rfTransmitters_)
        {
            std::unique_ptr<RFTransmitter> rfTransmitter = std::make_unique<RFTransmitter>(it.first, it.second.pin_);
            componentMaster.getEventDispatcher().subscribe(rfTransmitter.get(), "TransmitRFDataRequestEvent", fanController.get());
            componentMaster.getEventDispatcher().subscribe(fanController.get(), "TransmitRFDataBeginEvent", rfTransmitter.get());
            componentMaster.getEventDispatcher().subscribe(fanController.get(), "TransmitRFDataEndEvent", rfTransmitter.get());

            // Add RFTransmitter to ComponentMaster
            if (!componentMaster.addComponent(std::move(rfTransmitter)).success)
            {
                return Result(false, "Failed to add RFTransmitter to component master");
            }
        }

        if (!componentMaster.addComponent(std::move(fanController)).success)
        {
            return Result(false, "Failed to add FanController to component master");
        }
        return Result(true);
    }
};

#endif //HARBORBREEZEHUB_HUBFACTORY_H

//
// Created by enlil on 11/27/21.
//

#include "HttpsApiServer.h"
#include "FanState.h"
#include "UpdateFanLightRequestEvent.h"
#include "UpdateFanPowerRequestEvent.h"
#include "UpdateFanRotationRequestEvent.h"
#include "UpdateFanSpeedRequestEvent.h"

#include <boost/algorithm/string.hpp>
#include <thread>

using namespace httplib;

HttpsApiServer::HttpsApiServer(const std::string &instanceName, int port)
    : HttpsApiServer(instanceName, port, "", "") {}

HttpsApiServer::HttpsApiServer(const std::string& instanceName, int port,
                               const std::string& certificatePath, const std::string& keyPath)
    : port_(port), Component(instanceName)
{
    if (port_ <= 0 || port > 65535)
    {
        throw std::logic_error("Invalid port specified.");
    }
    if (certificatePath.empty() && keyPath.empty())
    {
        server_ = std::make_unique<Server>();
    }
    else
    {
        server_ = std::make_unique<SSLServer>(certificatePath.c_str(), keyPath.c_str());
    }

    server_->Get("/api/", [&](const Request& request, Response& response)
    {
        response.set_content("Harbor Breeze Hub: " + instanceName, "text/plain");
    });

    // Turn ON/OFF fan/light
    server_->Put("/api/fans/([A-Za-z0-9]+)/([a-z]+)/power", [&](const Request& request, Response& response)
    {
        auto& fanName = request.matches[1];
        auto& fanModule = request.matches[2];

        Power powerRequested;
        if (boost::algorithm::iequals(request.body, "ON"))
        {
            // Turn on request
            powerRequested = Power::ON;
        }
        else if (boost::algorithm::iequals(request.body, "OFF"))
        {
            // Turn off request
            powerRequested = Power::OFF;
        }
        else
        {
            response.set_content(R"({ "success": false, "reason": "Invalid power value. Expected 'ON' or 'OFF'." })",
                                 "application/json");
            return;
        }

        if (fanModule == "fan")
        {
            // Switch fan power
            eventDispatcher_->post(this, std::make_unique<UpdateFanPowerRequestEvent>(fanName, powerRequested));
        }
        else if (fanModule == "light")
        {
            // Switch light power
            eventDispatcher_->post(this, std::make_unique<UpdateFanLightRequestEvent>(fanName, powerRequested));
        }
        else
        {
            response.set_content(R"({ "success": false, "reason": "Invalid fan module. Expected 'fan' or 'light'." })",
                                 "application/json");
            return;
        }

        response.set_content(R"({ "success": true })", "application/json");
    });

    // Change fan speed
    server_->Put("/api/fans/([A-Za-z0-9]+)/fan/speed", [&](const Request& request, Response& response)
    {
        auto& fanName = request.matches[1];
        int fanSpeedRequested;
        try
        {
            fanSpeedRequested = std::stoi(request.body);
        }
        catch (...)
        {
            response.set_content(R"({ "success": false, "reason": "Invalid fan speed value." })",
                                 "application/json");
            return;
        }
        if (fanSpeedRequested < 1 || fanSpeedRequested > UINT8_MAX)
        {
            response.set_content(R"({ "success": false, "reason": "Fan speed value out-of-range." })",
                                 "application/json");
            return;
        }
        eventDispatcher_->post(this, std::make_unique<UpdateFanSpeedRequestEvent>(fanName, fanSpeedRequested));
        response.set_content(R"({ "success": true })", "application/json");
    });

    // Change fan rotation direction
    server_->Put("/api/fans/([A-Za-z0-9]+)/fan/rotation", [&](const Request& request, Response& response)
    {
        auto& fanName = request.matches[1];
        Rotation fanRotationRequested;
        if (boost::algorithm::iequals(request.body, "CCW"))
        {
            // Rotate fan CCW
            fanRotationRequested = Rotation::CCW;
        }
        else if (boost::algorithm::iequals(request.body, "CW"))
        {
            // Rotate fan CW
            fanRotationRequested = Rotation::CW;
        }
        else
        {
            response.set_content(R"({ "success": false, "reason": "Invalid rotation value. Expected 'CCW' or 'CW'." })",
                                 "application/json");
            return;
        }
        eventDispatcher_->post(this, std::make_unique<UpdateFanRotationRequestEvent>(fanName, fanRotationRequested));
        response.set_content(R"({ "success": true })", "application/json");
    });
}

HttpsApiServer::~HttpsApiServer() = default;

void HttpsApiServer::onStart()
{
    std::thread([&]()
    {
        server_->listen("0.0.0.0", port_);
    }).detach();
}

void HttpsApiServer::onStop()
{
    server_->stop();
}
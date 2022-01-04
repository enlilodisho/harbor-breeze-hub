//
// Created by enlil on 11/22/21.
//

#include "FanController.h"
#include "SSLLMessageBeginReceiveEvent.h"
#include "SSLLMessageEndReceiveEvent.h"
#include <iostream>
#include <UpdateFanLightRequestEvent.h>
#include <UpdateFanPowerRequestEvent.h>
#include <UpdateFanRotationRequestEvent.h>
#include <UpdateFanSpeedRequestEvent.h>
#include <TransmitRFDataRequestEvent.h>

#define REMOTE_ID_LENGTH    30
#define TRANSMIT_COUNT      10
#define CMD_LIGHT_POWER     "SLSLSSLSLLSSLLSSLLS"
#define CMD_LIGHT_DIM       "SLSSLLSSLLSSLLSSLLS"
#define CMD_FAN_POWER       "SLSSLSLSLSLSLLSSLLS"
#define CMD_FAN_SPEED_1     "SLSLSSLSLSLSLLSSLLS"
#define CMD_FAN_SPEED_2     "SLSSLLSSLSLSLLSSLLS"
#define CMD_FAN_SPEED_3     "SLSLSLSSLSLSLLSSLLS"
#define CMD_FAN_SPEED_4     "SLSSLSLLSSLSLLSSLLS"
#define CMD_FAN_SPEED_5     "SLSLSSLLSSLSLLSSLLS"
#define CMD_FAN_SPEED_6     "SLSSLLSLSSLSLLSSLLS"
#define CMD_ROTATE_CCW      "SLSSLSLSLLSSLLSSLLS"
#define CMD_ROTATE_CW       "SLSSLSLSLLSLSLSSLLS"

FanController::FanController(const std::string &instanceName, std::unique_ptr<SSLLEncoder> ssllEncoder)
    : ssllEncoder_(std::move(ssllEncoder)), Component(instanceName)
{

}

FanController::FanController(const std::string& instanceName, std::unique_ptr<SSLLEncoder> ssllEncoder,
                             const std::unordered_map<std::string, FanConfig>& fans)
    : ssllEncoder_(std::move(ssllEncoder)), Component(instanceName)
{
    for (auto& fanConfig : fans)
    {
        Fan fan{};
        fan.name_ = fanConfig.first;
        fan.remoteId_ = fanConfig.second.remoteId_;
        fans_[fanConfig.first] = fan;
    }
}

FanController::~FanController() = default;

void FanController::onEvent(Component* sender, std::shared_ptr<Event> event)
{
    if (event->type() == "SSLLMessageBeginReceiveEvent")
    {
        my_clock::time_point curTime = my_clock::now();
        std::shared_ptr<SSLLMessageBeginReceiveEvent> ssllMessageBeginEvent
                = std::static_pointer_cast<SSLLMessageBeginReceiveEvent>(event);
        std::string remoteId = getRemoteId(ssllMessageBeginEvent->getDataString());
        //std::cout <<"cmd:"<<getCommand(ssllMessageBeginEvent->getDataString())<<"\n";
        if (remoteId.size() != REMOTE_ID_LENGTH)
        {
            return;
        }

        // find fan for this remote id
        struct Fan* fan = nullptr;
        for (auto& it : fans_)
        {
            if (it.second.remoteId_ == remoteId)
            {
                fan = &it.second;
            }
        }
        if (fan == nullptr || fan->receivingMessage_)
        {
            return;
        }

        // TODO ignore if time diff small
        fan->receivingMessage_ = true;
        fan->lastMessageTime_ = curTime;

        std::cout << "Fan name:" << fan->name_ << ", command:";
        if (!updateFanStateFromCommand(fan->state_, getCommand(ssllMessageBeginEvent->getDataString())).success)
        {
            std::cout << "ERROR\n";
        }
    }
    else if (event->type() == "SSLLMessageEndReceiveEvent")
    {
        my_clock::time_point curTime = my_clock::now();
        std::shared_ptr<SSLLMessageEndReceiveEvent> ssllMessageEndEvent
                = std::static_pointer_cast<SSLLMessageEndReceiveEvent>(event);
        std::string remoteId = getRemoteId(ssllMessageEndEvent->getDataString());
        if (remoteId.size() != REMOTE_ID_LENGTH)
        {
            return;
        }

        // find fan for this remote id
        struct Fan* fan = nullptr;
        for (auto& it : fans_)
        {
            if (it.second.remoteId_ == remoteId)
            {
                fan = &it.second;
            }
        }
        if (fan == nullptr || !fan->receivingMessage_)
        {
            return;
        }

        // TODO handle light brightness
        fan->receivingMessage_ = false;
        fan->lastMessageTime_ = curTime;
    }
    else if (event->type() == "UpdateFanLightRequestEvent")
    {
        if (eventDispatcher_ == nullptr) return;
        std::shared_ptr<UpdateFanLightRequestEvent> updateFanLightEvent
                = std::static_pointer_cast<UpdateFanLightRequestEvent>(event);
        auto fan_it = fans_.find(updateFanLightEvent->getFanName());
        if (fan_it == fans_.end() || fan_it->second.state_.lightPower_ == updateFanLightEvent->getLightPower())
        {
            return;
        }
        fan_it->second.state_.lightPower_ = updateFanLightEvent->getLightPower();
        std::unique_ptr<std::vector<unsigned int>> timings = std::make_unique<std::vector<unsigned int>>();
        if (ssllEncoder_->getTimingsFromDataString(fan_it->second.remoteId_ + CMD_LIGHT_POWER, *timings).success)
        {
            std::unique_ptr<TransmitRFDataRequestEvent> transmissionRequestEvent =
                    std::make_unique<TransmitRFDataRequestEvent>(std::move(timings), ssllEncoder_->REST, TRANSMIT_COUNT);
            eventDispatcher_->post(this, std::move(transmissionRequestEvent));
        }
    }
    else if (event->type() == "UpdateFanPowerRequestEvent")
    {
        if (eventDispatcher_ == nullptr) return;
        std::shared_ptr<UpdateFanPowerRequestEvent> updateFanPowerEvent
                = std::static_pointer_cast<UpdateFanPowerRequestEvent>(event);
        auto fan_it = fans_.find(updateFanPowerEvent->getFanName());
        if (fan_it == fans_.end() || fan_it->second.state_.fanPower_ == updateFanPowerEvent->getFanPower())
        {
            return; // fan not in FanController
        }
        fan_it->second.state_.fanPower_ = updateFanPowerEvent->getFanPower();
        std::unique_ptr<std::vector<unsigned int>> timings = std::make_unique<std::vector<unsigned int>>();
        if (ssllEncoder_->getTimingsFromDataString(fan_it->second.remoteId_ + CMD_FAN_POWER, *timings).success)
        {
            std::unique_ptr<TransmitRFDataRequestEvent> transmissionRequestEvent =
                    std::make_unique<TransmitRFDataRequestEvent>(std::move(timings), ssllEncoder_->REST, TRANSMIT_COUNT);
            eventDispatcher_->post(this, std::move(transmissionRequestEvent));
        }
    }
    else if (event->type() == "UpdateFanRotationRequestEvent")
    {
        if (eventDispatcher_ == nullptr) return;
        std::shared_ptr<UpdateFanRotationRequestEvent> updateFanRotationEvent
                = std::static_pointer_cast<UpdateFanRotationRequestEvent>(event);
        auto fan_it = fans_.find(updateFanRotationEvent->getFanName());
        if (fan_it == fans_.end() || fan_it->second.state_.fanRotation_ == updateFanRotationEvent->getFanRotation())
        {
            return; // fan not in FanController
        }
        fan_it->second.state_.fanRotation_ = updateFanRotationEvent->getFanRotation();
        std::unique_ptr<std::vector<unsigned int>> timings = std::make_unique<std::vector<unsigned int>>();
        if (ssllEncoder_->getTimingsFromDataString(fan_it->second.remoteId_ +
        ((updateFanRotationEvent->getFanRotation() == CCW) ? CMD_ROTATE_CCW : CMD_ROTATE_CW), *timings).success)
        {
            std::unique_ptr<TransmitRFDataRequestEvent> transmissionRequestEvent =
                    std::make_unique<TransmitRFDataRequestEvent>(std::move(timings), ssllEncoder_->REST, TRANSMIT_COUNT);
            eventDispatcher_->post(this, std::move(transmissionRequestEvent));
        }
    }
    else if (event->type() == "UpdateFanSpeedRequestEvent")
    {
        if (eventDispatcher_ == nullptr) return;
        std::shared_ptr<UpdateFanSpeedRequestEvent> updateFanSpeedEvent
                = std::static_pointer_cast<UpdateFanSpeedRequestEvent>(event);
        auto fan_it = fans_.find(updateFanSpeedEvent->getFanName());
        if (fan_it == fans_.end() || fan_it->second.state_.fanSpeed_ == updateFanSpeedEvent->getFanSpeed())
        {
            return; // fan not in FanController
        }
        fan_it->second.state_.fanPower_ = ON;
        fan_it->second.state_.fanSpeed_ = updateFanSpeedEvent->getFanSpeed();
        std::string dataStr = fan_it->second.remoteId_;
        switch (updateFanSpeedEvent->getFanSpeed())
        {
            case 1:
                dataStr += CMD_FAN_SPEED_1;
                break;
            case 2:
                dataStr += CMD_FAN_SPEED_2;
                break;
            case 3:
                dataStr += CMD_FAN_SPEED_3;
                break;
            case 4:
                dataStr += CMD_FAN_SPEED_4;
                break;
            case 5:
                dataStr += CMD_FAN_SPEED_5;
                break;
            case 6:
                dataStr += CMD_FAN_SPEED_6;
                break;
            default:
                return; // invalid speed
        }
        std::unique_ptr<std::vector<unsigned int>> timings = std::make_unique<std::vector<unsigned int>>();
        if (ssllEncoder_->getTimingsFromDataString(dataStr, *timings).success)
        {
            std::unique_ptr<TransmitRFDataRequestEvent> transmissionRequestEvent =
                    std::make_unique<TransmitRFDataRequestEvent>(std::move(timings), ssllEncoder_->REST, TRANSMIT_COUNT);
            eventDispatcher_->post(this, std::move(transmissionRequestEvent));
        }
    }
    else if (event->type() == "TransmitRFDataBeginEvent")
    {

    }
}

Result FanController::updateFanStateFromCommand(struct FanState& fanState, const std::string& commandStr)
{
    if (commandStr == CMD_LIGHT_POWER) // light power
    {
        std::cout << "light power\n";
        fanState.lightPower_ = (fanState.lightPower_ == OFF) ? ON : OFF;
    }
    else if (commandStr == CMD_FAN_POWER) // fan power
    {
        std::cout << "fan power\n";
        fanState.fanPower_ = (fanState.fanPower_ == OFF) ? ON : OFF;
    }
    else if (commandStr == CMD_FAN_SPEED_1) // fan speed 1
    {
        std::cout << "fan speed 1\n";
        fanState.fanSpeed_ = 1;
    }
    else if (commandStr == CMD_FAN_SPEED_2) // fan speed 2
    {
        std::cout << "fan speed 2\n";
        fanState.fanSpeed_ = 2;
    }
    else if (commandStr == CMD_FAN_SPEED_3) // fan speed 3
    {
        std::cout << "fan speed 3\n";
        fanState.fanSpeed_ = 3;
    }
    else if (commandStr == CMD_FAN_SPEED_4) // fan speed 4
    {
        std::cout << "fan speed 4\n";
        fanState.fanSpeed_ = 4;
    }
    else if (commandStr == CMD_FAN_SPEED_5) // fan speed 5
    {
        std::cout << "fan speed 5\n";
        fanState.fanSpeed_ = 5;
    }
    else if (commandStr == CMD_FAN_SPEED_6) // fan speed 6
    {
        std::cout << "fan speed 6\n";
        fanState.fanSpeed_ = 6;
    }
    else if (commandStr == CMD_ROTATE_CCW) // rotate ccw
    {
        std::cout << "rotate ccw\n";
        fanState.fanRotation_ = CCW;
    }
    else if (commandStr == CMD_ROTATE_CW) // rotate cw
    {
        std::cout << "rotate cw\n";
        fanState.fanRotation_ = CW;
    }
    else if (commandStr == CMD_LIGHT_DIM) // light dim
    {
        std::cout << "light dim\n";
    }
    else
    {
        return Result(false, "Unknown command received for fan");
    }
    return Result(true);
}

std::string FanController::getRemoteId(const std::string& dataStr)
{
    return dataStr.substr(0, REMOTE_ID_LENGTH);
}

std::string FanController::getCommand(const std::string& dataStr)
{
    return dataStr.substr(REMOTE_ID_LENGTH);
}
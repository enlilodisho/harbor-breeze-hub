//
// Created by enlil on 11/22/21.
//

#ifndef HARBORBREEZEHUB_FANCONTROLLER_H
#define HARBORBREEZEHUB_FANCONTROLLER_H

#include "core.h"
#include "HubConfig.h"

#include <chrono>
#include <unordered_map>

class FanController : public Component
{
public:
    enum Power
    {
        OFF,
        ON
    };
    enum Rotation
    {
        CCW,
        CW
    };
    struct FanState
    {
        Power fanPower_ = OFF;
        uint8_t fanSpeed_ = 0;
        Rotation fanRotation_ = CCW;
        Power lightPower_ = OFF;
        uint8_t lightBrightness_ = 0;
    };

    explicit FanController(const std::string& instanceName);
    FanController(const std::string& instanceName, const std::unordered_map<std::string, FanConfig>& fans);
    ~FanController() override;

    [[nodiscard]] ComponentType type() const override
    {
        return "FanController";
    }

protected:
    typedef std::chrono::steady_clock my_clock;
    struct Fan
    {
        std::string name_;
        std::string remoteId_;
        my_clock::time_point lastMessageTime_;
        bool receivingMessage_ = false;
        FanState state_;
    };

    std::unordered_map<std::string, struct Fan> fans_;

    void onEvent(Component* sender, std::shared_ptr<Event> event) override;
    static Result updateFanStateFromCommand(struct FanState& fan, const std::string& commandStr);
    static std::string getRemoteId(const std::string& dataStr);
    static std::string getCommand(const std::string& dataStr);

};

#endif //HARBORBREEZEHUB_FANCONTROLLER_H

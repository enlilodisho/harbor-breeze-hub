//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_HTTPSAPISERVER_H
#define HARBORBREEZEHUB_HTTPSAPISERVER_H

#include "core.h"

#include <httplib.h>

class HttpsApiServer : public Component
{
private:
    const int port_;

protected:
    std::unique_ptr<httplib::Server> server_;

    void onStart() override;
    void onStop() override;

public:
    HttpsApiServer(const std::string &instanceName, int port);
    HttpsApiServer(const std::string &instanceName, int port,
                   const std::string& certificatePath, const std::string& keyPath);
    ~HttpsApiServer() override;

    [[nodiscard]] unsigned int getPort() const
    {
        return port_;
    }

    [[nodiscard]] ComponentType type() const override {
        return "HttpsApiServer";
    }
};

#endif //HARBORBREEZEHUB_HTTPSAPISERVER_H

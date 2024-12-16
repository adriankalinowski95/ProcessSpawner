#pragma once

#include <shared/src/application/services/PathMacro.h>
#include <shared/src/domain/models/PingMessage.h>
#include <shared/src/application/utils/ModelsJsonConverter.h>

namespace child_process::api::controllers {

class ProcessManagerCommunicationController : public BaseController {
public:
    ProcessManagerCommunicationController() = default;

    PATH("/init", init)
    void init(boost::asio::ip::tcp::socket& socket, const http::request<http::string_body>& req) {
        const std::string body = req.body().c_str();
        auto message = shared::application::utils::ModelsJsonConverter{}.fromJson<shared::domain::models::PingMessage>(body);
        
        // @Todo change this to templates like in core!
        if (!message) {
            auto baseResponse = this->getBaseResponse(req);
            baseResponse.body() = "Failed to parse message!";
            http::write(socket, baseResponse);

            return;
        }

        // add validation 
        message->uniqueNumber = message->uniqueNumber + 1;

        auto baseResponse = this->getBaseResponse(req);
        baseResponse.body() = shared::application::utils::ModelsJsonConverter{}.toJson(*message);
        baseResponse.content_length(baseResponse.body().size() + 1);
        baseResponse.prepare_payload();

        http::write(socket, baseResponse);
    }
};

}
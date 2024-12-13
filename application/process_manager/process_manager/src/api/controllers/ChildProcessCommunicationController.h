#pragma once

#include <shared/src/application/services/PathMacro.h>

namespace process_manager::api::controllers {

class ChildProcessCommunicationController : public BaseController {
public:
    ChildProcessCommunicationController() = default;

    PATH("/fun1", handleFun1)
    void handleFun1(boost::asio::ip::tcp::socket& socket, const http::request<http::string_body>& req) {
        auto baseResponse = this->getBaseResponse();
        res.body() = "Response from /fun2";

        http::write(socket, res);
    }
};

}
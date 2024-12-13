#pragma once

#include "BaseController.h"

// Makro PATH
//#define PATH(route, func_name) \
//    void func_name(boost::asio::ip::tcp::socket& socket, const http::request<http::string_body>& req); \
//    HandlerRegistrar registrar_##func_name{*this, route, std::bind(&std::remove_reference<decltype(*this)>::type::func_name, this, std::placeholders::_1, std::placeholders::_2)};

#define PATH(route, func_name) \
    HandlerRegistrar registrar_##func_name{*this, route, std::bind(&std::remove_reference<decltype(*this)>::type::func_name, this, std::placeholders::_1, std::placeholders::_2)};
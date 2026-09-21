// Поля запроса протокола; при отсутствии token парсер оставляет пустую строку.
#pragma once

#include <boost/json.hpp>
#include <string>

struct ProtocolRequest
{
    std::string event;
    std::string token;
    boost::json::object data;
};

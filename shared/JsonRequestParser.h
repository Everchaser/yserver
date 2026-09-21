// Разбор JSON-запроса: обязательные event и data, необязательный token.
#pragma once

#include <string>
#include "ProtocolRequest.h"

class JsonRequestParser
{
public:
    ProtocolRequest parseRequest(const std::string& payload);
};

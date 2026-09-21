// Разбор JSON-запроса: обязательные event и data, необязательный token.
#pragma once
#include <string>
#include "ProtocolRequest.h"

class Parser {
public:

    Request parseRequest(const std::string& payload);
};

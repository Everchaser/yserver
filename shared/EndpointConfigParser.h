// Разбор обязательных полей host и port из JSON-конфигурации.
#pragma once
#include <string>
#include "EndpointConfig.h"

class ParserConfig {
public:
    EndpointConfig parse(const std::string& filePayload);

private:

};

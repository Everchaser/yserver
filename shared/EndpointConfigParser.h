// Разбор обязательных полей host и port из JSON-конфигурации.
#pragma once

#include <string>
#include "EndpointConfig.h"

class EndpointConfigParser
{
public:
    EndpointConfig parse(const std::string& configText);
};

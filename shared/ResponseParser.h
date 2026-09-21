#pragma once

#include "ProtocolResponse.h"
#include <string>

class ResponseParser
{
public:
    ProtocolResponse parseResponse(const std::string& payload) const;
    std::string serialize(const ProtocolResponse& response) const;
};

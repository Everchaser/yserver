#pragma once

#include <string>

struct ProtocolResponse
{
    enum class Status { OK, ERR };

    Status status = Status::ERR;
    std::string message;

    ProtocolResponse() = default;
    ProtocolResponse(Status responseStatus, const std::string& responseMessage)
        : status(responseStatus), message(responseMessage) {}
};

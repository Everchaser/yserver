#pragma once

#include <string>

struct ProtocolResponse
{
    enum class Status { OK, ERR };

    Status status = Status::ERR;
    std::string message;
};

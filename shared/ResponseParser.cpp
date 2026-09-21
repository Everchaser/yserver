#include "ResponseParser.h"

#include <boost/json.hpp>
#include <stdexcept>

ProtocolResponse ResponseParser::parseResponse(const std::string& payload) const
{
    const auto value = boost::json::parse(payload);
    const auto& object = value.as_object();
    const auto& status = object.at("status").as_string();

    ProtocolResponse response;
    if (status == "OK")
        response.status = ProtocolResponse::Status::OK;
    else if (status == "ERR")
        response.status = ProtocolResponse::Status::ERR;
    else
        throw std::invalid_argument("Unknown response status");

    response.message = boost::json::value_to<std::string>(object.at("message"));
    return response;
}

std::string ResponseParser::serialize(const ProtocolResponse& response) const
{
    boost::json::object object;
    switch (response.status)
    {
    case ProtocolResponse::Status::OK:
        object["status"] = "OK";
        break;
    case ProtocolResponse::Status::ERR:
        object["status"] = "ERR";
        break;
    default:
        throw std::invalid_argument("Unknown response status");
    }
    object["message"] = response.message;
    return boost::json::serialize(object);
}

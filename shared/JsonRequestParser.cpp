#include "JsonRequestParser.h"
#include <boost/json.hpp>

ProtocolRequest JsonRequestParser::parseRequest(const std::string& payload)
{
    const auto parsedValue = boost::json::parse(payload);
    const auto& jsonObject = parsedValue.as_object();

    ProtocolRequest request;

    request.event = std::string(jsonObject.at("event").as_string());

    request.data = jsonObject.at("data").as_object();

    auto tokenIterator = jsonObject.find("token");

    if (tokenIterator != jsonObject.end())
    {
        request.token = std::string(tokenIterator->value().as_string());
    }

    return request;
}

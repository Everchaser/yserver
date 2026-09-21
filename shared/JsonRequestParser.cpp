#include "JsonRequestParser.h"
#include <boost/json.hpp>

ProtocolRequest JsonRequestParser::parseRequest(const std::string& payload)
{
    const auto parsedValue = boost::json::parse(payload);
    const auto& jsonObject = parsedValue.as_object();

    ProtocolRequest request;

    request.event = boost::json::value_to<std::string>(jsonObject.at("event"));

    request.data = jsonObject.at("data").as_object();

    auto tokenIterator = jsonObject.find("token");

    if (tokenIterator != jsonObject.end())
    {
        request.token = boost::json::value_to<std::string>(tokenIterator->value());
    }

    return request;
}

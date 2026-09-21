#include "JsonRequestParser.h"
#include <boost/json.hpp>

Request Parser::parseRequest(const std::string& payload)
{
    boost::json::value parsedValue = boost::json::parse(payload);
    boost::json::object jsonObject = parsedValue.as_object();

    Request request;

    request.event =
        std::string(jsonObject.at("event").as_string());

    request.data =
        jsonObject.at("data").as_object();

    auto tokenIterator = jsonObject.find("token");

    if (tokenIterator != jsonObject.end())
    {
        request.token =
            std::string(tokenIterator->value().as_string());
    }

    return request;
}

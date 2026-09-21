#include "EndpointConfigParser.h"
#include <stdexcept>
#include <boost/json.hpp>

EndpointConfig ParserConfig::parse(const std::string& filePayload) {

    boost::json::value parsedValue;
    parsedValue = boost::json::parse(filePayload);

    boost::json::object jsonObject;
    jsonObject = parsedValue.as_object();

    boost::json::string hostBoost = jsonObject.at("host").as_string();
    const auto portValue = jsonObject.at("port").as_int64();
    if (portValue < 1 || portValue > 65535)
    throw std::invalid_argument("Invalid port");
    int port = static_cast<int>(portValue);

    std::string host = hostBoost.c_str();

    EndpointConfig config(
    host, port
    );

    return config;

}

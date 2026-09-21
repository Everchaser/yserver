#include "EndpointConfigParser.h"
#include <stdexcept>
#include <boost/json.hpp>

EndpointConfig EndpointConfigParser::parse(const std::string& configText)
{
    const auto parsedValue = boost::json::parse(configText);
    const auto& jsonObject = parsedValue.as_object();
    const auto& hostJson = jsonObject.at("host").as_string();
    const auto portValue = jsonObject.at("port").as_int64();
    if (portValue < 1 || portValue > 65535)
        throw std::invalid_argument("Invalid port");

    return EndpointConfig(hostJson.c_str(), static_cast<int>(portValue));
}

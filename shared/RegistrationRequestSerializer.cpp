#include "RegistrationRequestSerializer.h"
#include <boost/json.hpp>

std::string RegistrationRequestSerializer::serialize(
    const std::string& login,
    const std::string& password)
{
    boost::json::object data;
    data["login"] = login;
    data["password"] = password;

    boost::json::object request;
    request["event"] = "REGISTER";
    request["data"] = data;

    return boost::json::serialize(request);
}

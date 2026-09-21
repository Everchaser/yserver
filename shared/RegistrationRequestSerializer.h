// Формирование JSON-запроса REGISTER с логином и паролем в объекте data.
#pragma once

#include <string>

class RegistrationRequestSerializer
{
public:
    std::string serialize(const std::string& login, const std::string& password);
};

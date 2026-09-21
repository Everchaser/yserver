// Незавершённая заготовка поиска пользователя в каталоге; не включена в сборку.
#pragma once
#include <string>

class UserDatabase
{
public:
    bool hasLogin(const std::string& login);

private:
    const std::string directoryPath_ =
        "/home/everchaser/dio";
};

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

struct User
{
    std::string uuid;
    std::string login;
    std::string passwordHash;
    std::string tokenHash;
};

// Один JSON-файл на пользователя: <root>/users/<uuid>.json.
// Вызывается из единственного потока ioContext сервера.
class UserDirectoryDatabase
{
public:
    explicit UserDirectoryDatabase(const std::filesystem::path& root);
    bool hasLogin(const std::string& login) const;
    bool registerUser(const std::string& login, const std::string& password);
    std::size_t size() const { return usersByLogin_.size(); }

private:
    std::filesystem::path usersDirectory_;
    std::unordered_map<std::string, User> usersByLogin_;
};

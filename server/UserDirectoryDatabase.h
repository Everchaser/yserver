#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

struct User
{
    std::string uuid;
    std::string login;
    std::string password_hash;
    std::string token_hash;
};

// Один JSON-файл на пользователя: <root>/users/<uuid>.json.
// Вызывается из единственного потока io_context сервера.
class UserDatabase
{
public:
    explicit UserDatabase(const std::filesystem::path& root);
    bool hasLogin(const std::string& login) const;
    bool registerUser(const std::string& login, const std::string& password);
    std::size_t size() const { return users_.size(); }

private:
    std::filesystem::path directoryPath_;
    std::unordered_map<std::string, User> users_;
};

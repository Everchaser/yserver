#include "UserDirectoryDatabase.h"
#include "TextFileReader.h"
#include <boost/json.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <array>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace
{
template<std::size_t N>
std::string toHex(const std::array<unsigned char, N>& bytes)
{
    constexpr char digits[] = "0123456789abcdef";
    std::string result;
    for (unsigned char byte : bytes)
    {
        result += digits[byte >> 4];
        result += digits[byte & 15];
    }
    return result;
}

std::string hashPassword(const std::string& password)
{
    constexpr int iterations = 600000;
    std::array<unsigned char, 16> salt;
    std::array<unsigned char, 32> hash;
    if (password.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::invalid_argument("Password too long");
    if (RAND_bytes(salt.data(), salt.size()) != 1 ||
        PKCS5_PBKDF2_HMAC(password.data(), static_cast<int>(password.size()),
                          salt.data(), salt.size(), iterations, EVP_sha256(),
                          hash.size(), hash.data()) != 1)
        throw std::runtime_error("Password hashing failed");
    return "pbkdf2-sha256$" + std::to_string(iterations) + "$" + toHex(salt) + "$" + toHex(hash);
}
}

UserDirectoryDatabase::UserDirectoryDatabase(const boost::filesystem::path& root)
    : usersDirectory_(root / "users")
{
    // Отсутствующая или повреждённая база — ошибка запуска, а не пустая база.
    for (const auto& entry : boost::filesystem::directory_iterator(usersDirectory_))
    {
        if (!boost::filesystem::is_regular_file(entry.path()) || entry.path().extension() != ".json")
            continue;
        try
        {
            const auto value = boost::json::parse(TextFileReader(entry.path().string()).readFile());
            const auto& object = value.as_object();
            User user{boost::json::value_to<std::string>(object.at("uuid")),
                      boost::json::value_to<std::string>(object.at("login")),
                      boost::json::value_to<std::string>(object.at("password_hash")),
                      boost::json::value_to<std::string>(object.at("token_hash"))};
            const auto uuid = boost::uuids::string_generator()(user.uuid);
            if (uuid.is_nil() || boost::uuids::to_string(uuid) != user.uuid ||
                entry.path().stem().string() != user.uuid ||
                user.login.empty() || user.passwordHash.empty())
                throw std::runtime_error("Invalid user fields or filename");
            if (!usersByLogin_.emplace(user.login, user).second)
                throw std::runtime_error("Duplicate login");
        }
        catch (const std::exception& error)
        {
            throw std::runtime_error(entry.path().string() + ": " + error.what());
        }
    }
}

bool UserDirectoryDatabase::hasLogin(const std::string& login) const
{
    return usersByLogin_.find(login) != usersByLogin_.end();
}

bool UserDirectoryDatabase::registerUser(const std::string& login, const std::string& password)
{
    if (login.empty() || password.empty())
        throw std::invalid_argument("Login and password must not be empty");
    if (hasLogin(login))
        return false;

    User user{boost::uuids::to_string(boost::uuids::random_generator()()),
              login, hashPassword(password), ""};
    const auto path = usersDirectory_ / (user.uuid + ".json");
    const auto temporaryPath = usersDirectory_ / (user.uuid + ".tmp");
    boost::json::object object{{"uuid", user.uuid}, {"login", user.login},
                               {"password_hash", user.passwordHash},
                               {"token_hash", user.tokenHash}};
    // Сначала сохраняем полный файл, затем делаем его видимым для загрузчика.
    try
    {
        std::ofstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        file.open(temporaryPath.string());
        file << boost::json::serialize(object) << '\n';
        file.close();
        boost::filesystem::rename(temporaryPath, path);
    }
    catch (...)
    {
        boost::system::error_code ignored;
        boost::filesystem::remove(temporaryPath, ignored);
        throw;
    }
    usersByLogin_.emplace(login, std::move(user));
    return true;
}

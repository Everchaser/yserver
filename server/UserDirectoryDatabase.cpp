#include "UserDirectoryDatabase.h"
#include "TextFileReader.h"
#include <boost/json.hpp>
#include <filesystem>

bool UserDatabase::hasLogin(const std::string &login)
{
    std::filesystem::directory_iterator file(directoryPath_);

    while (file != std::filesystem::directory_iterator{})
    {

        std::filesystem::path path = (*file).path();

        // Заготовка не завершена: reader не объявлен, результат поиска не возвращается.
        std::string fileText = reader.readFile();
        ++file;
    }
}

// Чтение содержимого текстового файла по заданному пути.
#pragma once

#include <string>

class TextFileReader
{
public:
    TextFileReader(std::string filePath);
    std::string readFile();

private:
    std::string filePath_;
};

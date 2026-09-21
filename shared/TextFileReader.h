// Чтение содержимого текстового файла по заданному пути.
#pragma once
#include <string>

class FileReader {
public:
    FileReader(std::string filePath);
    std::string readFile();

private:
    std::string filePath_;

};

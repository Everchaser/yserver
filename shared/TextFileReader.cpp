#include "TextFileReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

TextFileReader::TextFileReader(std::string filePath) : filePath_(filePath) {}

std::string TextFileReader::readFile()
{
    std::ifstream file(filePath_);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file");

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

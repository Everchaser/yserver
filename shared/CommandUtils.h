#pragma once

#include <algorithm>
#include <cctype>
#include <string>

inline bool commandEquals(std::string command, std::string expected)
{
    const auto normalize = [](std::string& text)
    {
        text.erase(std::remove(text.begin(), text.end(), '/'), text.end());
        for (char& ch : text)
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    };

    normalize(command);
    normalize(expected);
    return command == expected;
}

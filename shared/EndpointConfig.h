// Адрес подключения; конструктор проверяет диапазон TCP-порта.
#pragma once
#include <set>
#include <string>

class EndpointConfig {
public:

    EndpointConfig(std::string host,
    int port);

    const std::string& host() const;

    int port() const;

private:
    std::string host_;
    int port_;

    static std::set<std::string> allowed_protocols_;
};

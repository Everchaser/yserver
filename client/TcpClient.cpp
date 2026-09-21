#include "TcpClient.h"

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include "ResponseParser.h"
#include "RegistrationRequestSerializer.h"
#include "CommandUtils.h"

TcpClient::TcpClient(boost::asio::io_context& ioContext, Endpoints endpoints)
    : socket_(ioContext),
      reconnectTimer_(ioContext),
      endpoints_(std::move(endpoints))
{
}

void TcpClient::tryConnect()
{
    boost::asio::async_connect(
        socket_, endpoints_,
        std::bind(&TcpClient::onConnect,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

void TcpClient::onConnect(const boost::system::error_code& error,
                        const tcp::endpoint& endpoint)
{
    if (!error)
    {
        std::cout << "Подключились к " << endpoint << '\n';
        processConsoleCommand();
        return;
    }

    std::cerr << "Не удалось подключиться: " << error.message()
              << "\nПовтор через 3 секунды...\n";

    // После ошибки подключения повторяем попытку через три секунды.
    reconnectTimer_.expires_after(boost::asio::chrono::seconds(3));
    reconnectTimer_.async_wait(
        std::bind(&TcpClient::tryConnect,
                  this));
}

void TcpClient::processConsoleCommand()
{
    // Консольный ввод и обмен данными здесь выполняются синхронно.
    std::string command;
    std::cout << "Введите команду: ";
    std::getline(std::cin, command);

    RegistrationRequestSerializer serializer;
    std::string requestText;

    if (commandEquals(command, "register"))
    {
        std::string login;
        std::string password;

        std::cout << "Login: ";
        std::getline(std::cin, login);

        std::cout << "Password: ";
        std::getline(std::cin, password);

        requestText = serializer.serialize(login, password);
    }
    else if (commandEquals(command, "healthcheck"))
    {
        requestText = R"({"event":"HEALTHCHECK","data":{}})";
    }
    else
    {
        std::cout << "Неизвестная команда\n";
        return;
    }
    requestText += '\n';
    boost::asio::write(socket_, boost::asio::buffer(requestText));

    boost::asio::streambuf responseBuffer;
    boost::asio::read_until(socket_, responseBuffer, '\n');
    std::istream responseStream(&responseBuffer);
    std::string responseText;
    std::getline(responseStream, responseText);

    const auto response = ResponseParser().parseResponse(responseText);
    std::cout << (response.status == ProtocolResponse::Status::OK ? "OK" : "ERR")
              << ": " << response.message << '\n';
}

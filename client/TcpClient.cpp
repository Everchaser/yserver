#include "TcpClient.h"

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include "JsonRequestParser.h"
#include "RegistrationRequestSerializer.h"

Client::Client(boost::asio::io_context &io_context, Endpoints endpoints)
    : socket_(io_context),
      timer_(io_context),
      endpoints_(std::move(endpoints))
{
}

void Client::try_connect()
{

    boost::asio::async_connect(
        socket_, endpoints_,
        std::bind(&Client::on_connect,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

void Client::on_connect(const boost::system::error_code &error,
                        const tcp::endpoint &endpoint)
{
    if (!error)
    {
        std::cout << "Подключились к " << endpoint << '\n';
        communicate();
        return;
    }

    std::cerr << "Не удалось подключиться: " << error.message()
              << "\nПовтор через 3 секунды...\n";

    // После ошибки подключения повторяем попытку через три секунды.
    timer_.expires_after(boost::asio::chrono::seconds(3));
    timer_.async_wait(
        std::bind(&Client::try_connect,
                  this));
}

void Client::communicate()
{
    // Консольный ввод и обмен данными здесь выполняются синхронно.
    std::string command;
    std::cout << "Введите команду: ";
    std::getline(std::cin, command);

    Serializer serializer;
    std::string requestText;

    if (command == "/REGISTER")
    {
        std::string login;
        std::string password;

        std::cout << "Login: ";
        std::getline(std::cin, login);

        std::cout << "Password: ";
        std::getline(std::cin, password);

        requestText = serializer.serialize(login, password);
    }
    else
    {
        std::cout << "Неизвестная команда\n";
        return;
    }
    boost::asio::write(socket_, boost::asio::buffer(requestText));

    char arr[100];
    const std::size_t bytes = socket_.read_some(boost::asio::buffer(arr));
    std::string responseText(arr, bytes);

    std::cout << responseText << '\n';
}

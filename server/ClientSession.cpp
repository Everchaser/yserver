#include "ClientSession.h"
#include <iostream>
#include <functional>
#include "RegistrationRequestSerializer.h"
#include "JsonRequestParser.h"

std::string make_daytime_string()
{
    std::time_t now = std::time(nullptr);

    return ctime(&now);
}

void Session::on_read(const boost::system::error_code &error,
                      std::size_t bytes)

{
    if (error)
    {
        std::cerr << "Ошибка чтения: " << error.message() << '\n';
        return;
    }

    std::string requestText(arr_, bytes);

    Parser parser;
    Request request = parser.parseRequest(requestText);

    std::cout << "Получена команда: " << request.event << '\n';

    // Незавершённое объявление ответа в исходном коде; требует отдельного исправления.
    //boost::json::object;

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(responseText_),
        std::bind(&Session::on_write,
                  shared_from_this(),
                  std::placeholders::_1,
                  std::placeholders::_2));
}
void Session::on_write(const boost::system::error_code &error,
                       std::size_t bytes)
{
    if (error)
        std::cerr << "Ошибка отправки: " << error.message() << '\n';
}
void Session::start()
{

    // Обработчик удерживает сессию до завершения асинхронного чтения.
    std::shared_ptr<Session> self = shared_from_this();
    socket_.async_read_some(
        boost::asio::buffer(arr_),
        std::bind(&Session::on_read,
                  self,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

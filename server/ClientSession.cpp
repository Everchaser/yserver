#include "ClientSession.h"
#include <iostream>
#include <functional>
#include "ResponseParser.h"
#include "JsonRequestParser.h"
#include "CommandUtils.h"

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

    std::istream requestStream(&requestBuffer_);
    std::string requestText;
    std::getline(requestStream, requestText);

    ProtocolResponse response;
    try
    {
        const Request request = Parser().parseRequest(requestText);
        std::cout << "Получена команда: " << request.event << '\n';
        if (commandEquals(request.event, "healthcheck"))
        {
            response = {ProtocolResponse::Status::OK, "Сервер работает"};
        }
        else
        {
            response = {ProtocolResponse::Status::ERR, "Неизвестная команда"};
        }
    }
    catch (const std::exception&)
    {
        response = {ProtocolResponse::Status::ERR, "Некорректный запрос"};
    }
    responseText_ = ResponseParser().serialize(response) + '\n';

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
    boost::asio::async_read_until(
        socket_, requestBuffer_, '\n',
        std::bind(&Session::on_read,
                  self,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

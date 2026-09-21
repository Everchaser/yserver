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
        else if (commandEquals(request.event, "REGISTER"))
        {
            const std::string login(request.data.at("login").as_string());
            const std::string password(request.data.at("password").as_string());
            if (login.empty() || password.empty())
            {
                response = {ProtocolResponse::Status::ERR, "Логин и пароль не должны быть пустыми"};
            }
            else
            {
                try
                {
                    response = database_.registerUser(login, password)
                        ? ProtocolResponse{ProtocolResponse::Status::OK, "Пользователь зарегистрирован"}
                        : ProtocolResponse{ProtocolResponse::Status::ERR, "Логин уже занят"};
                }
                catch (const std::exception& error)
                {
                    std::cerr << "Ошибка базы данных: " << error.what() << '\n';
                    response = {ProtocolResponse::Status::ERR, "Не удалось сохранить пользователя"};
                }
            }
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

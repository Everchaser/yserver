#include "ClientSession.h"
#include <iostream>
#include <functional>
#include "ResponseParser.h"
#include "JsonRequestParser.h"
#include "CommandUtils.h"

void ClientSession::onRead(const boost::system::error_code& error,
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
        const ProtocolRequest request = JsonRequestParser().parseRequest(requestText);
        std::cout << "Получена команда: " << request.event << '\n';
        response = handleRequest(request);
    }
    catch (const std::exception&)
    {
        response = {ProtocolResponse::Status::ERR, "Некорректный запрос"};
    }
    responseText_ = ResponseParser().serialize(response) + '\n';

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(responseText_),
        std::bind(&ClientSession::onWrite,
                  shared_from_this(),
                  std::placeholders::_1,
                  std::placeholders::_2));
}

ProtocolResponse ClientSession::handleRequest(const ProtocolRequest& request)
{
    if (commandEquals(request.event, "healthcheck"))
        return {ProtocolResponse::Status::OK, "Сервер работает"};
    if (commandEquals(request.event, "REGISTER"))
        return handleRegistration(request);
    return {ProtocolResponse::Status::ERR, "Неизвестная команда"};
}

ProtocolResponse ClientSession::handleRegistration(const ProtocolRequest& request)
{
    const std::string login(request.data.at("login").as_string());
    const std::string password(request.data.at("password").as_string());
    if (login.empty() || password.empty())
        return {ProtocolResponse::Status::ERR, "Логин и пароль не должны быть пустыми"};

    try
    {
        return database_.registerUser(login, password)
            ? ProtocolResponse{ProtocolResponse::Status::OK, "Пользователь зарегистрирован"}
            : ProtocolResponse{ProtocolResponse::Status::ERR, "Логин уже занят"};
    }
    catch (const std::exception& error)
    {
        std::cerr << "Ошибка базы данных: " << error.what() << '\n';
        return {ProtocolResponse::Status::ERR, "Не удалось сохранить пользователя"};
    }
}

void ClientSession::onWrite(const boost::system::error_code& error,
                            std::size_t bytes)
{
    if (error)
        std::cerr << "Ошибка отправки: " << error.message() << '\n';
}

void ClientSession::start()
{
    // Обработчик удерживает сессию до завершения асинхронного чтения.
    std::shared_ptr<ClientSession> self = shared_from_this();
    boost::asio::async_read_until(
        socket_, requestBuffer_, '\n',
        std::bind(&ClientSession::onRead,
                  self,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

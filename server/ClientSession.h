// Сессия одного клиента: однократное чтение запроса и отправка ответа.
#pragma once

#include "ProtocolRequest.h"
#include "ProtocolResponse.h"
#include "UserDirectoryDatabase.h"
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <utility>

using boost::asio::ip::tcp;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(tcp::socket socket, UserDirectoryDatabase& database)
        : socket_(std::move(socket)), database_(database) {}

    void start();

private:
    tcp::socket socket_;
    UserDirectoryDatabase& database_;
    boost::asio::streambuf requestBuffer_;
    std::string responseText_;

    void onRead(const boost::system::error_code& error, std::size_t bytes);
    void onWrite(const boost::system::error_code& error, std::size_t bytes);
    ProtocolResponse handleRequest(const ProtocolRequest& request);
    ProtocolResponse handleRegistration(const ProtocolRequest& request);
};

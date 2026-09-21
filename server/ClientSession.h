// Сессия одного клиента: однократное чтение запроса и отправка ответа.
#pragma once
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <utility>
#include <ctime>
#include "UserDirectoryDatabase.h"

using boost::asio::ip::tcp;
void read(const boost::system::error_code &error,
std::size_t bytes);

class Session : public std::enable_shared_from_this<Session> {
public:

    Session(tcp::socket socket, UserDatabase& database)
    : socket_ (std::move(socket)), database_(database) {}
    void start();

    tcp::socket& socket() {
        return socket_;
    }

private:
    tcp::socket socket_;
    UserDatabase& database_;
    boost::asio::streambuf requestBuffer_;
    std::string responseText_;

    void on_read(const boost::system::error_code& error,
    std::size_t bytes);
    void on_write(const boost::system::error_code& error, std::size_t bytes);

};

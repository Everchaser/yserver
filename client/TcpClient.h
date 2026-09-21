// Клиент TCP: подключение с повторными попытками и консольная команда регистрации.
#pragma once

#include <boost/asio.hpp>

class TcpClient
{
public:
    using tcp = boost::asio::ip::tcp;
    using Endpoints = tcp::resolver::results_type;

    TcpClient(boost::asio::io_context& ioContext, Endpoints endpoints);

    void tryConnect();

private:
    tcp::socket socket_;
    boost::asio::steady_timer reconnectTimer_;
    Endpoints endpoints_;

    void onConnect(const boost::system::error_code& error,
                   const tcp::endpoint& endpoint);

    void processConsoleCommand();
};

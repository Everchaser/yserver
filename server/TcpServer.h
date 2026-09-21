// Сервер TCP: приём подключений и периодический вывод статистики.
#pragma once
#include <boost/asio.hpp>
#include "EndpointConfig.h"
#include "EndpointConfigParser.h"
#include "UserDirectoryDatabase.h"

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_context &context, const EndpointConfig &config,
           UserDatabase& database);

private:
    UserDatabase& database_;
    tcp::acceptor acceptor_;
    void do_accept();
    void on_accept(const boost::system::error_code &error,
                   tcp::socket socket);
    void on_write(const boost::system::error_code &error, std::size_t bytes);
    boost::asio::steady_timer statisticsTimer_;
    void start_statistics_timer();
    void on_statistics_timer();
    // Общее число принятых подключений, включая уже закрытые.
    int acceptedClients_ = 0;
    boost::asio::steady_timer statusTimer_;
    void start_status_timer();
    void on_status_timer();
};

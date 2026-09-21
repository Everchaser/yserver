// Сервер TCP: приём подключений и периодический вывод статистики.
#pragma once

#include <boost/asio.hpp>
#include "EndpointConfig.h"
#include "UserDirectoryDatabase.h"

using boost::asio::ip::tcp;

class TcpServer
{
public:
    TcpServer(boost::asio::io_context& context, const EndpointConfig& config,
              UserDirectoryDatabase& database);

private:
    UserDirectoryDatabase& database_;
    tcp::acceptor acceptor_;
    void acceptNextConnection();
    void onAccept(const boost::system::error_code& error,
                  tcp::socket socket);
    boost::asio::steady_timer statisticsTimer_;
    void startStatisticsTimer();
    void onStatisticsTimer();
    // Общее число принятых подключений, включая уже закрытые.
    int totalAcceptedConnections_ = 0;
    boost::asio::steady_timer statusTimer_;
    void startStatusTimer();
    void onStatusTimer();
};

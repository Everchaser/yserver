#include "TcpServer.h"
#include <functional>
#include <memory>
#include <utility>
#include <iostream>
#include "ClientSession.h"

TcpServer::TcpServer(boost::asio::io_context& context, const EndpointConfig& config,
                     UserDirectoryDatabase& database)
    : database_(database),
      acceptor_(context, tcp::endpoint(boost::asio::ip::make_address(config.host()),
                                     config.port())),
      statisticsTimer_(context),
      statusTimer_(context)
{
    acceptNextConnection();
    startStatisticsTimer();
    startStatusTimer();
}

void TcpServer::acceptNextConnection()
{
    acceptor_.async_accept(
        std::bind(&TcpServer::onAccept,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

void TcpServer::startStatisticsTimer()
{
    statisticsTimer_.expires_after(boost::asio::chrono::seconds(5));
    statisticsTimer_.async_wait(std::bind(
        &TcpServer::onStatisticsTimer, this));
}

void TcpServer::onStatisticsTimer()
{
    std::cout << " Всего подключено" << '\n' << totalAcceptedConnections_ << '\n' << "клиентов" << '\n';
    startStatisticsTimer();
}

void TcpServer::startStatusTimer()
{
    statusTimer_.expires_after(
        boost::asio::chrono::seconds(10));

    statusTimer_.async_wait(
        std::bind(&TcpServer::onStatusTimer, this));
}

void TcpServer::onStatusTimer()
{
    std::cout << "Сервер работает" << '\n';

    startStatusTimer();
}

void TcpServer::onAccept(const boost::system::error_code& error,
                       tcp::socket socket)
{
    if (error)
    {
        std::cerr << error.message() << '\n';
        return;
    }
    ++totalAcceptedConnections_;
    std::shared_ptr<ClientSession> session =
        std::make_shared<ClientSession>(std::move(socket), database_);
    session->start();
    acceptNextConnection();
}

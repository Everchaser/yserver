// Клиент TCP: подключение с повторными попытками и консольная команда регистрации.
#pragma once
#include <boost/asio.hpp>

class Client
{
public:
    using tcp = boost::asio::ip::tcp;
    using Endpoints = tcp::resolver::results_type;

    Client(boost::asio::io_context& io_context, Endpoints endpoints);

    void try_connect();

private:
    tcp::socket socket_;
    boost::asio::steady_timer timer_;
    Endpoints endpoints_;

    void on_connect(const boost::system::error_code& error,
                    const tcp::endpoint& endpoint);

    void communicate();
    };

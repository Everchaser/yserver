// Загрузка конфигурации, разрешение адреса сервера и запуск клиента.
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <utility>
#include "TcpClient.h"
#include "EndpointConfigParser.h"
#include "TextFileReader.h"

using boost::asio::ip::tcp;

int main(int ac, char **av)
{
    try
    {
        if (ac != 2)
        {
            std::cerr << "Usage: client <config.json>" << std::endl;
            return 1;
        }
        FileReader reader(av[1]);
        std::string filePayload = reader.readFile();
        ParserConfig configParser;
        EndpointConfig config = configParser.parse(filePayload);
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);

        Client::Endpoints endpoints =
            resolver.resolve(config.host(), std::to_string(config.port()));

        Client client(io_context, std::move(endpoints));
        client.try_connect();
        io_context.run();
    }

    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

// Загрузка конфигурации, разрешение адреса сервера и запуск клиента.
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <utility>
#include "TcpClient.h"
#include "EndpointConfigParser.h"
#include "TextFileReader.h"

using boost::asio::ip::tcp;

int main(int argc, char** argv)
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <config.json>" << std::endl;
            return 1;
        }
        TextFileReader reader(argv[1]);
        std::string configText = reader.readFile();
        EndpointConfigParser configParser;
        EndpointConfig config = configParser.parse(configText);
        boost::asio::io_context ioContext;
        tcp::resolver resolver(ioContext);

        TcpClient::Endpoints endpoints =
            resolver.resolve(config.host(), std::to_string(config.port()));

        TcpClient client(ioContext, std::move(endpoints));
        client.tryConnect();
        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

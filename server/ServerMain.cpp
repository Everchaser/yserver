// Загрузка конфигурации и запуск цикла обработки событий сервера.
#include "TextFileReader.h"
#include "EndpointConfigParser.h"
#include "TcpServer.h"
#include <boost/asio.hpp>
#include <iostream>

int main(int ac, char **av)
{
    try
    {
        if (ac != 2)
        {
            std::cerr << "Usage: server <config.json>\n";
            return 1;
        }

        FileReader reader(av[1]);
        std::string filePayload = reader.readFile();
        ParserConfig parser;
        EndpointConfig config = parser.parse(filePayload);

        std::cout << "host: " << config.host() << '\n';
        std::cout << "port: " << config.port() << '\n';

        boost::asio::io_context io_context;
        Server server(io_context, config);
        io_context.run();
        std::cout << "Listening on " << config.host()
                  << " port " << config.port() << std::endl;

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}

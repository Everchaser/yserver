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
        if (ac != 2 && ac != 3)
        {
            std::cerr << "Usage: server <config.json> [database-directory]\n";
            return 1;
        }

        FileReader reader(av[1]);
        std::string filePayload = reader.readFile();
        ParserConfig parser;
        EndpointConfig config = parser.parse(filePayload);

        std::cout << "host: " << config.host() << '\n';
        std::cout << "port: " << config.port() << '\n';

        boost::asio::io_context io_context;
        const std::filesystem::path databasePath = ac == 3
            ? std::filesystem::path(av[2])
            : std::filesystem::path(av[1]).parent_path() / "mock_db";
        UserDatabase database(databasePath);
        std::cout << "База данных: " << databasePath << ", пользователей: " << database.size() << '\n';
        Server server(io_context, config, database);
        std::cout << "Listening on " << config.host()
                  << " port " << config.port() << std::endl;
        io_context.run();

    }
    catch (const std::exception &e)
    {
        std::cerr << "Ошибка сервера: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

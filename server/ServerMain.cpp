// Загрузка конфигурации и запуск цикла обработки событий сервера.
#include "TextFileReader.h"
#include "EndpointConfigParser.h"
#include "TcpServer.h"
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        if (argc != 2 && argc != 3)
        {
            std::cerr << "Usage: server <config.json> [database-directory]\n";
            return 1;
        }

        TextFileReader reader(argv[1]);
        std::string configText = reader.readFile();
        EndpointConfigParser configParser;
        EndpointConfig config = configParser.parse(configText);

        std::cout << "host: " << config.host() << '\n';
        std::cout << "port: " << config.port() << '\n';

        boost::asio::io_context ioContext;
        const boost::filesystem::path databasePath = argc == 3
            ? boost::filesystem::path(argv[2])
            : boost::filesystem::path(argv[1]).parent_path() / "mock_db";
        UserDirectoryDatabase database(databasePath);
        std::cout << "База данных: " << databasePath << ", пользователей: " << database.size() << '\n';
        TcpServer server(ioContext, config, database);
        std::cout << "Listening on " << config.host()
                  << " port " << config.port() << std::endl;
        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка сервера: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

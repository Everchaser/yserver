cmake_minimum_required(VERSION 3.16)

project(mentorshipproject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost REQUIRED COMPONENTS json)
find_package(Threads REQUIRED)

# Отдельный каталог исключает конфликт имён программ с client/ и server/.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Настройки копируются в bin рядом с исполняемыми файлами.
configure_file(EndpointConfig.json "${CMAKE_BINARY_DIR}/bin/EndpointConfig.json" COPYONLY)

# Конфигурация, чтение файлов и JSON-протокол используются обоими приложениями.
add_library(tcp_common STATIC
    shared/EndpointConfig.cpp
    shared/TextFileReader.cpp
    shared/EndpointConfigParser.cpp
    shared/ResponseParser.cpp
    shared/JsonRequestParser.cpp
    shared/RegistrationRequestSerializer.cpp
)
target_include_directories(tcp_common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/shared")
target_link_libraries(tcp_common PUBLIC Boost::json)

add_executable(client
    client/ClientMain.cpp
    client/TcpClient.cpp
)
target_link_libraries(client PRIVATE tcp_common Threads::Threads)

add_executable(server
    server/ServerMain.cpp
    server/TcpServer.cpp
    server/ClientSession.cpp
)
target_link_libraries(server PRIVATE tcp_common Threads::Threads)

# server/UserDirectoryDatabase.cpp остаётся вне сборки: это незавершённая заготовка.

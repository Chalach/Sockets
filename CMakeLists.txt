cmake_minimum_required(VERSION 3.7)
project(Sockets)

set(CMAKE_C_STANDARD 99)

add_executable(Client Client/Client.c)
add_executable(Server Server/Server.c)
add_executable(ChatClient ChatClient/ChatClient.c)
add_executable(ChatServer ChatServer/ChatServer.c)
add_executable(Sockets test.c)
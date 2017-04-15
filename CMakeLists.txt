cmake_minimum_required(VERSION 3.7)
project(Sockets)

set(CMAKE_C_STANDARD 99)

add_executable(Client Client/Client.c)
add_executable(Server Server/Server.c)

add_executable(ChatClient ChatClient/ChatClient.c)
target_link_libraries(ChatClient -lpthread)
add_executable(ChatServer ChatServer/ChatServer.c)
target_link_libraries(ChatServer -lpthread)

add_executable(Test test.c)
target_link_libraries(Test -lpthread)
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

// Our headers to test
#include "../Core/Events.hpp"
#include "../Handlers/MessageHandler.hpp"
#include "../Models/Server.hpp"
#include "../Utils/Colors.hpp"

//c++ -Wall -Wextra -Werror Tests/Integration.cpp Services/ChannelService.cpp Services/UserService.cpp Models/Channel.cpp Models/Server.cpp Models/User.cpp Handlers/MessageHandler.cpp Commands/*.cpp Utils/ServerMessages.cpp Utils/Utils.cpp Core/Events.cpp 

void commandsRegister(std::vector<std::string> &commands)
{
    //commands.push_back("PASS");
    //commands.push_back("NICK");
    //commands.push_back("USER");
    
    commands.push_back("JOIN");
    commands.push_back("PRIVMSG"); // todo: find out why core dumped when no args
    commands.push_back("WHO");
    commands.push_back("MODE");
    commands.push_back("INVITE");
    commands.push_back("PART");
    commands.push_back("KICK");
    commands.push_back("TOPIC");
    commands.push_back("PING");

    // remove events from poll will crash the tester cause im simulating
    //commands.push_back("QUIT");
}


int main(void)
{
    // the infamous write[0]/read[1]
    int fd[2];
    std::vector<std::string> commands;
    std::string strObj;

    commandsRegister(commands);

    // setup the fds
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == -1)
    {
        std::cerr << "Failed to simulate the fds :(" << "\n";
        return (1);
    }

    // Set the password to connect to the server
    Server::getInstance().setPassword("abc");

    // Init
    MessageHandler handler;

    // Simulates a connection to server
    handler.CreateEvent(fd[1]);

    std::cout << "---------- Not registered Test ----------" << "\n\n";
    for (std::string command : commands)
    {
        send(fd[0], command.c_str(), command.size(), 0);

        handler.HandleEvent(fd[1]);
        char buf[1024];

        int nbr = recv(fd[0], buf, sizeof(buf) - 1, MSG_DONTWAIT);
        if (nbr > 0)
        {
            buf[nbr] = '\0';
            strObj = buf;
        }

        std::cout << "Test - " << command << " - " << (strObj.empty() ? RED : GREEN) << (strObj.empty() ? "[FAILED]" : "[SUCCESS]") << RESET << "\n";
        std::cout << "Response:\n" << strObj << '\n';

        strObj.clear();
    }

    // Authentication initial command --------------------------------------------
    std::cout << "---------- Auth Test ----------" << "\n\n";
    
    std::string passCommand("PASS abc\r\nNICK davi20\r\nUSER davi20 * * *\r\n");
    send(fd[0], passCommand.c_str(), passCommand.size(), 0);

    handler.HandleEvent(fd[1]);

    char buf[1024];

    int nbr = read(fd[0], buf, sizeof(buf) - 1);
    buf[nbr] = '\0';
    strObj = buf;

    std::cout << "Auth Test - " << (strObj.empty() ? RED : GREEN) << (strObj.empty() ? "[FAILED]" : "[SUCCESS]") << RESET << "\n";
    std::cout << "Response:\n" << strObj << '\n';
    strObj.clear();

    // Not enough params test --------------------------------------------
    std::cout << "---------- Not enough params Test ----------" << "\n\n";
    for (std::string command : commands)
    {
        send(fd[0], command.c_str(), command.size(), 0);

        handler.HandleEvent(fd[1]);
        char buf[1024];

        int nbr = recv(fd[0], buf, sizeof(buf) - 1, MSG_DONTWAIT);
        if (nbr > 0)
        {
            buf[nbr] = '\0';
            strObj = buf;
        }

        std::cout << "Test - " << command << " - " << (strObj.empty() ? RED : GREEN) << (strObj.empty() ? "[FAILED]" : "[SUCCESS]") << RESET << "\n";
        std::cout << "Response:\n" << strObj << '\n';

        strObj.clear();
    }
}
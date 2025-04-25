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
    
    commands.push_back("JOIN\r\n");
    commands.push_back("PRIVMSG\r\n"); // todo: find out why core dumped when no args
    commands.push_back("WHO\r\n");
    commands.push_back("MODE\r\n");
    commands.push_back("INVITE\r\n");
    commands.push_back("PART\r\n");
    commands.push_back("KICK\r\n");
    commands.push_back("TOPIC\r\n");
    commands.push_back("PING\r\n");

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

    char buf[1024] = {0};

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
        char buf[1024] = {0};

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


        // Specific tests ----------------------------------------------------
        std::cout << "---------- Specific test ----------" << "\n\n";

        struct SpecificTest {
            std::string label;
            std::string command;
            std::string expectedReply;
        };
    
        std::vector<SpecificTest> specificTests = {
            // JOIN
            {"JOIN valid channel", "JOIN #testchannel\r\n", "JOIN"},
            {"JOIN again same channel", "JOIN #testchannel\r\n", "443"}, // ERR_USERONCHANNEL
    
            // PRIVMSG
            {"PRIVMSG no such nick", "PRIVMSG nosuchnick :hello\r\n", "401"}, // ERR_NOSUCHNICK
            {"PRIVMSG valid self", "PRIVMSG davi20 :hi\r\n", "PRIVMSG"},
            { "PRIVMSG with no recipient", "PRIVMSG :hello\r\n", "461" }, // ERR_NOTENOUGHPARAMETERS
            { "PRIVMSG with no text", "PRIVMSG davi20\r\n", "412" }, // ERR_NOTEXTTOSEND

    
            // WHO
            {"WHO test channel", "WHO #testchannel\r\n", "352"}, // RPL_WHOREPLY

            // INVITE
            {"INVITE no such nick", "INVITE nosuchnick #testchannel\r\n", "401"}, // ERR_NOSUCHNICK
    
            // PART
            {"PART not on channel", "PART #notjoined\r\n", "403"}, // ERR_NOTONCHANNEL
            {"PART valid channel", "PART #testchannel\r\n", "PART"},
            { "PART with no channel", "PART\r\n", "461" }, // ERR_NEEDMOREPARAMS
            { "PART unknown channel", "PART #ghost\r\n", "403" }, // ERR_NOSUCHCHANNEL

    
            // KICK
            {"KICK not in channel", "KICK #testchannel nosuchnick\r\n", "401"}, // ERR_USERNOTINCHANNEL
            {"KICK not operator", "JOIN #kickchan\r\n", "JOIN"}, // First join
            {"KICK not operator try", "KICK #kickchan davi20\r\n", "KICK"},
    
            // TOPIC
            {"TOPIC no such channel", "TOPIC #doesnotexist\r\n", "403"}, // ERR_NOSUCHCHANNEL
            {"TOPIC not on channel", "TOPIC #kickchan\r\n", "442"}, // ERR_NOTONCHANNEL
    
            // PING
            {"PING success", "PING 12345\r\n", "PONG"},
        };
    
        for (const SpecificTest& test : specificTests)
        {
            send(fd[0], test.command.c_str(), test.command.size(), 0);
            handler.HandleEvent(fd[1]);
    
            char buf[1024] = {0};
            std::string response;
            while (true)
            {
                int nbr = recv(fd[0], buf, sizeof(buf) - 1, MSG_DONTWAIT);
                if (nbr <= 0)
                    break;
                buf[nbr] = '\0';
                response += buf;
            }
    
            bool passed = response.find(test.expectedReply) != std::string::npos;
    
            std::cout << "Test: " << test.label << "\n"
                      << "Command: " << test.command
                      << "Expected: " << test.expectedReply << "\n"
                      << "Result: " << (passed ? GREEN "[PASS]" : RED "[FAIL]") << RESET << "\n"
                      << "Response:\n" << (response.empty() ? "(no response)" : response) << "\n\n";
        }
    
}
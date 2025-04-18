/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:03 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 19:59:18 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <map>
#include <cerrno>
#include <cstring>


#include "../Services/ChannelService.hpp"
#include "../Services/UserService.hpp"

#include "../Models/MessageContent.hpp"

#include "../Commands/Command.hpp"
#include "../Commands/PassCommand.hpp"
#include "../Commands/NickCommand.hpp"
#include "../Commands/UserCommand.hpp"
#include "../Commands/JoinCommand.hpp"
#include "../Commands/PrivMsgCommand.hpp"
#include "../Commands/QuitCommand.hpp"
#include "../Commands/WhoCommand.hpp"
#include "../Commands/ModeCommand.hpp"
#include "../Commands/InviteCommand.hpp"
#include "../Commands/PartCommand.hpp"
#include "../Commands/KickCommand.hpp"
#include "../Commands/TopicCommand.hpp"
#include "../Commands/PingCommand.hpp"


/* 
    CLASS WITH THE PURPOSE OF HANDLING EVENTS,
    WHETHER CREATING AN EVENT (CREATING A USER AND STORING FD AND USER DATA),
    HANDLING MESSAGES BY CALLING SERVICES AND LOGIC FOR PARSING AND TOKENIZATION,
    AND REGISTERING COMMANDS
*/
class MessageHandler
{
private:
    typedef std::map<std::string, Command*> cmdsMap;
    std::string getMessage(std::string& buffer, std::size_t it);
    std::string getMessage(std::string& strBegin, std::istringstream& stream);

    UserService& _userService;
    ChannelService& _channelService;
    cmdsMap _commands;

    std::vector<std::string> splitDeVariosComandos(std::string &buffer);

    void RegisterCommands();

    void FreeCommands();
public:
    MessageHandler();
    ~MessageHandler();

    MessageContent ircTokenizer(std::string &buffer);

    bool HandleEvent(int fd);
    void CreateEvent(int fd);

    void ProcessCommand(MessageContent messageContent, int clientFd);

    enum CommandType { 
        PASS,
        NICK,
        USER
    };
};
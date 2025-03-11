/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:03 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 22:27:04 by davi             ###   ########.fr       */
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

#include "../Services/ChannelService.hpp"
#include "../Services/UserService.hpp"

#include "../Models/MessageContent.hpp"

#include "../Commands/Command.hpp"
#include "../Commands/PassCommand.hpp"
#include "../Commands/NickCommand.hpp"
#include "../Commands/UserCommand.hpp"
#include "../Commands/JoinCommand.hpp"
#include "../Commands/PrivMsgCommand.hpp"

// TALVEZ DEPOIS SUBSTITUIR POR METHODOS ESTATICOS
// JA QUE NAO POSSUI VARIAVEIS PROPRIAS

class MessageHandler
{
private:
    std::string getMessage(std::string& buffer, std::size_t it);
    std::string getMessage(std::string& strBegin, std::istringstream& stream);

    bool IsOnlyTab(std::string& buffer);

    UserService _userService;
    ChannelService _channelService;
    std::map<std::string, Command*> _commands;

    std::vector<std::string> splitDeVariosComandos(std::string buffer);

    void RegisterCommands();
public:
    MessageHandler();
    ~MessageHandler();

    MessageContent ircTokenizer(std::string buffer);

    void HandleEvent(int fd);
    void CreateEvent(int fd);

    void ProcessCommand(MessageContent messageContent, int clientFd);

    enum CommandType {
        PASS,
        NICK,
        USER
    };
};
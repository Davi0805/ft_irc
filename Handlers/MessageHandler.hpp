/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/23 15:56:11 by artuda-s         ###   ########.fr       */
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

#include "../Exceptions/Exceptions.hpp"
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

/* 
    CLASSE COM OBJETIVO DE DAR HANDLE DOS EVENTOS,
    SEJA CRIAR UM EVENTO (CRIAR USER E ARMAZENAR FD E DADOS DO USUARIO),
    HANDLE DE MENSAGENS CHAMANDO SERVICES E LOGICAS DE PARSING E TOKENIZACAO
    E REGISTRO DE COMANDOS
*/
class MessageHandler
{
private:
    typedef std::map<std::string, Command*> cmdsMap;
    std::string getMessage(std::string& buffer, std::size_t it);
    std::string getMessage(std::string& strBegin, std::istringstream& stream);

    UserService _userService;
    ChannelService _channelService;
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

    enum CommandType { // todo check if used
        PASS,
        NICK,
        USER
    };
};
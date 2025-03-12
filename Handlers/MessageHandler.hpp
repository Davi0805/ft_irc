/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:03 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 12:50:22 by dmelo-ca         ###   ########.fr       */
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
#include "../Commands/QuitCommand.hpp"

/* 
    CLASSE COM OBJETIVO DE DAR HANDLE DOS EVENTOS,
    SEJA CRIAR UM EVENTO (CRIAR USER E ARMAZENAR FD E DADOS DO USUARIO),
    HANDLE DE MENSAGENS CHAMANDO SERVICES E LOGICAS DE PARSING E TOKENIZACAO
    E REGISTRO DE COMANDOS
*/
class MessageHandler
{
private:
    std::string getMessage(std::string& buffer, std::size_t it);
    std::string getMessage(std::string& strBegin, std::istringstream& stream);

    UserService _userService;
    ChannelService _channelService;
    std::map<std::string, Command*> _commands;

    std::vector<std::string> splitDeVariosComandos(std::string buffer);

    void RegisterCommands();

    void FreeCommands();
public:
    MessageHandler();
    ~MessageHandler();

    MessageContent ircTokenizer(std::string buffer);

    bool HandleEvent(int fd);
    void CreateEvent(int fd);

    void ProcessCommand(MessageContent messageContent, int clientFd);

    enum CommandType {
        PASS,
        NICK,
        USER
    };
};
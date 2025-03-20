/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:17:12 by davi              #+#    #+#             */
/*   Updated: 2025/03/20 13:55:49 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsgCommand.hpp"

PrivMsgCommand::PrivMsgCommand(UserService& userService, ChannelService& channelService)
                        :_userService(&userService), _channelService(&channelService)
{
}

PrivMsgCommand::~PrivMsgCommand()
{
}

void PrivMsgCommand::execute(MessageContent messageContent, int fd)
{
    //(void)_userService;
    //(void)_channelService;
    //(void)messageContent;
    //(void)fd;
    
    std::cout << "[DEBUG]: COMANDO PRIVMSG SENDO CHAMADO" << std::endl;

    // TODO: VERIFICACOES PARA EVITAR CRASHES
    User* sender = _userService->findUserByFd(fd);

    if (messageContent.tokens.size() == 2 && messageContent.message.find(":DCC") == 0)
    {
        std::string fullMsg = ServerMessages::ConvertMessageContentToA(messageContent);
        User* receiver = _userService->findUserByNickname(messageContent.tokens[1]);

        send(receiver->getFd(), fullMsg.c_str(), fullMsg.size(), 0);
    }
    else if (messageContent.tokens[1][0] == '#') // Se for um channel publico, comeca com # (jogo da velha no inicio)
    {
        if (_channelService->isUserPartOfChannel(fd, messageContent.tokens[1]))
        {
            std::vector<User*> users = _channelService->findChannel(messageContent.tokens[1])->getUsers();
            Channel* channel = _channelService->findChannel(messageContent.tokens[1]);

            std::string msgFormatada = ServerMessages::PrivMsgFormatter(sender, channel, messageContent.message);

            for (size_t i = 0; i < users.size(); i++)
            {
                if (users[i]->getFd() != sender->getFd())
                    send(users[i]->getFd(), msgFormatada.c_str(), msgFormatada.size(), 0);
            }
        }
    }
    else
    {
        User* receiver = _userService->findUserByNickname(messageContent.tokens[1]);
        if (receiver == NULL)
            return ; // TODO: Exception

        std::string msgFormatada = ServerMessages::PrivMsgFormatter(sender, receiver, messageContent.message);
        
        //send(receiver->getFd(), messageContent.message.c_str(), messageContent.message.size(), 0);
        send(receiver->getFd(), msgFormatada.c_str(), msgFormatada.size(), 0);
    }
}


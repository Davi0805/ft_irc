/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:17:12 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 21:25:01 by davi             ###   ########.fr       */
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

    // Se for um channel publico, comeca com # (jogo da velha no inicio)    
    if (messageContent.tokens[1][0] == '#')
    {
        if (_channelService->isUserPartOfChannel(fd, messageContent.tokens[1]))
        {
            std::vector<User*> users = _channelService->findChannel(messageContent.tokens[1])->getUsers();
            for (size_t i = 0; i < users.size(); i++)
            {
                if (users[i]->getFd() != fd)
                    send(users[i]->getFd(), messageContent.message.c_str(), messageContent.message.size(), 0);
            }
        }
    }
    else
    {
        User* user = _userService->findUserByNickname(messageContent.tokens[1]);
        if (user == NULL)
            return ; // TODO: Exception
        send(user->getFd(), messageContent.message.c_str(), messageContent.message.size(), 0);
    }
}


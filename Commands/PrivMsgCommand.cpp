/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:17:12 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 20:53:18 by davi             ###   ########.fr       */
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

    // TODO: OS CANAIS SAO CARACTERIZADOS COM UM # (JOGO DA VELHA NO INICIO DO NOME)
    
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


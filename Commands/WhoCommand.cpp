/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:53:53 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/26 12:39:42 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WhoCommand.hpp"

WhoCommand::WhoCommand(UserService& userService, ChannelService& channelService)
                    :_userService(&userService), _channelService(&channelService)
{
}

WhoCommand::~WhoCommand()
{
}

void WhoCommand::execute(MessageContent messageContent, int fd)
{
    (void)_userService;
    //(void)_channelService;
    //(void)messageContent;
    //(void)fd;

    User* sender = _userService->findUserByFd(fd);

    if (messageContent.tokens.size() > 1 && messageContent.tokens[1][0] == '#') // Se for um channel publico, comeca com # (jogo da velha no inicio)
    {
        // ! NAO SEI SE ISSO SE APLICA AO CHANNEL
        if (_channelService->isUserPartOfChannel(fd, messageContent.tokens[1]))
        {
            std::vector<User*> users = _channelService->findChannel(messageContent.tokens[1])->getUsers();
            Channel* channel = _channelService->findChannel(messageContent.tokens[1]);

            std::string msgFormatada = ServerMessages::WhoReply(sender, channel);

            send(sender->getFd(), msgFormatada.c_str(), msgFormatada.size(), 0);
            /* for (size_t i = 0; i < users.size(); i++)
            {
                if (users[i]->getFd() != sender->getFd())
                    send(users[i]->getFd(), msgFormatada.c_str(), msgFormatada.size(), 0);
            } */
        }
    }
}

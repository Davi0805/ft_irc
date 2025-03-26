/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:53:53 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/26 14:21:00 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WhoCommand.hpp"

WhoCommand::WhoCommand()
{
}

WhoCommand::~WhoCommand()
{
}

void WhoCommand::execute(MessageContent messageContent, int fd)
{

    User* sender = UserService::getInstance().findUserByFd(fd);

    if (messageContent.tokens.size() > 1 && messageContent.tokens[1][0] == '#') // Se for um channel publico, comeca com # (jogo da velha no inicio)
    {
        // ! NAO SEI SE ISSO SE APLICA AO CHANNEL
        if (ChannelService::getInstance().isUserPartOfChannel(fd, messageContent.tokens[1]))
        {
            std::vector<User*> users = ChannelService::getInstance().findChannel(messageContent.tokens[1])->getUsers();
            Channel* channel = ChannelService::getInstance().findChannel(messageContent.tokens[1]);

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

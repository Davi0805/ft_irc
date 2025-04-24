/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:53:53 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/04/24 13:12:42 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WhoCommand.hpp"

WhoCommand::WhoCommand() {}

WhoCommand::~WhoCommand() {}

void WhoCommand::execute(MessageContent messageContent, int fd)
{

    User* sender = UserService::getInstance().findUserByFd(fd);
    if (!sender)
        return ;
        
    if (!sender->isAuthenticated()){
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, sender->getNick());
        return ;
    }

    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "WHO");
        return ;
    }

    if (messageContent.tokens.size() > 1 && messageContent.tokens[1][0] == '#')
    {
        if (ChannelService::getInstance().isUserPartOfChannel(fd, messageContent.tokens[1]))
        {
            std::vector<User*> users = ChannelService::getInstance().findChannel(messageContent.tokens[1])->getUsers();
            Channel* channel = ChannelService::getInstance().findChannel(messageContent.tokens[1]);

            std::string msgFormatada = ServerMessages::WhoReply(sender, channel);

            send(sender->getFd(), msgFormatada.c_str(), msgFormatada.size(), 0);
        }
    }
}

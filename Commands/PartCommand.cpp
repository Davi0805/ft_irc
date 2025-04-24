/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:10:56 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/04/24 13:12:49 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCommand.hpp"


PartCommand::PartCommand() {}

PartCommand::~PartCommand() {}

void PartCommand::execute(MessageContent messageContent, int fd)
{
    User* user = UserService::getInstance().findUserByFd(fd);

    if (!user->isAuthenticated()){
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, user->getNick());
        return ;
    }

    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "PART");
        return ;
    }
    
    for (size_t i = 1; i < messageContent.tokens.size(); i++)
    {
        if (ChannelService::getInstance().isUserPartOfChannel(fd, messageContent.tokens[i]))
        {
            ServerMessages::PartMsg(ChannelService::getInstance().findChannel(messageContent.tokens[i]), user, messageContent.message);
            ChannelService::getInstance().findChannel(messageContent.tokens[i])->removeUser(fd);
        } else if (!ChannelService::getInstance().findChannel(messageContent.tokens[i])) {
            ServerMessages::SendErrorMessage(fd, 403, user->getNick(), "");
        } else
            ServerMessages::SendErrorMessage(fd, 442, user->getNick(), ""); 
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:10:56 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/04/18 19:42:44 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCommand.hpp"


PartCommand::PartCommand() {}

PartCommand::~PartCommand() {}

void PartCommand::execute(MessageContent messageContent, int fd)
{
    User* user = UserService::getInstance().findUserByFd(fd);
    
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
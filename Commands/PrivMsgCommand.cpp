/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:17:12 by davi              #+#    #+#             */
/*   Updated: 2025/04/25 13:44:05 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsgCommand.hpp"

PrivMsgCommand::PrivMsgCommand() {}

PrivMsgCommand::~PrivMsgCommand() {}

void PrivMsgCommand::execute(MessageContent messageContent, int fd)
{
    User* sender = UserService::getInstance().findUserByFd(fd);
    if (!sender) return ;
    
    if (!sender->isAuthenticated())
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, sender->getNick());
        return ;
    }

    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "PRIVMSG");
        return ;
    }

    if (messageContent.tokens.size() == 2 && messageContent.message.find(":DCC") == 0)
    {
        std::string fullMsg = ServerMessages::ConvertMessageContentToA(messageContent);
        User* receiver = UserService::getInstance().findUserByNickname(messageContent.tokens[1]);

        send(receiver->getFd(), fullMsg.c_str(), fullMsg.size(), 0);
    }
    else if (messageContent.tokens[1][0] == '#') // If it is a public channel starts with #
    {
        if (ChannelService::getInstance().isUserPartOfChannel(fd, messageContent.tokens[1]))
        {
            std::vector<User*> users = ChannelService::getInstance().findChannel(messageContent.tokens[1])->getUsers();
            Channel* channel = ChannelService::getInstance().findChannel(messageContent.tokens[1]);

            std::string formattedMsg = ServerMessages::PrivMsgFormatter(sender, channel, messageContent.message);

            for (size_t i = 0; i < users.size(); i++)
            {
                if (users[i]->getFd() != sender->getFd())
                    send(users[i]->getFd(), formattedMsg.c_str(), formattedMsg.size(), 0);
            }
        }
    }
    else
    {
        User* receiver = UserService::getInstance().findUserByNickname(messageContent.tokens[1]);
        if (receiver == NULL)
        {
            ServerMessages::SendErrorMessage(fd, 401, messageContent.tokens[1]);
            return ;
        }

        std::string formattedMsg = ServerMessages::PrivMsgFormatter(sender, receiver, messageContent.message);
        
        send(receiver->getFd(), formattedMsg.c_str(), formattedMsg.size(), 0);
    }
}


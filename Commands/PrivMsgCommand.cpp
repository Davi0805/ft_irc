/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:17:12 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 18:36:30 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivMsgCommand.hpp"

PrivMsgCommand::PrivMsgCommand()
{
}

PrivMsgCommand::~PrivMsgCommand()
{
}

void PrivMsgCommand::execute(MessageContent messageContent, int fd)
{

    User* sender = UserService::getInstance().findUserByFd(fd);
    if (!sender) return ;

    if (messageContent.tokens.size() == 2 && messageContent.message.find(":DCC") == 0)
    {
        std::string fullMsg = ServerMessages::ConvertMessageContentToA(messageContent);
        User* receiver = UserService::getInstance().findUserByNickname(messageContent.tokens[1]);

        send(receiver->getFd(), fullMsg.c_str(), fullMsg.size(), 0);
    }
    else if (messageContent.tokens[1][0] == '#')
    {
        if (ChannelService::getInstance().isUserPartOfChannel(fd, messageContent.tokens[1]))
        {
            std::vector<User*> users = ChannelService::getInstance().findChannel(messageContent.tokens[1])->getUsers();
            Channel* channel = ChannelService::getInstance().findChannel(messageContent.tokens[1]);

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
        User* receiver = UserService::getInstance().findUserByNickname(messageContent.tokens[1]);
        if (receiver == NULL)
            return ;

        std::string msgFormatada = ServerMessages::PrivMsgFormatter(sender, receiver, messageContent.message);
        
        send(receiver->getFd(), msgFormatada.c_str(), msgFormatada.size(), 0);
    }
}


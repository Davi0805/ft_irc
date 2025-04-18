/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:16:16 by lebarbos          #+#    #+#             */
/*   Updated: 2025/04/18 19:47:10 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"

TopicCommand::TopicCommand() {}

TopicCommand::~TopicCommand() {}

void TopicCommand::execute(MessageContent messageContent, int fd)
{
    User *user = UserService::getInstance().findUserByFd(fd);
    if (!user) return;

    // Checks if user is registred
    if (user->getStatus() < User::AUTHENTICATED)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, "TOPIC");
        return;
    }

    // Checks is command has at least the name of the channel
    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "TOPIC");
        return;
    }

    std::string channelName = messageContent.tokens[1];
    Channel *channel = ChannelService::getInstance().findChannel(channelName);

    // Channel exits?
    if (!channel)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOSUCHCHANNEL, channelName);
        return;
    }

    // Verifies if the user is in the channel
    if (!channel->isUserInChannel(fd))
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTONCHANNEL, channelName);
        return;
    }

    // if the message is empty, returns the actual topic of the channel
    if (messageContent.message.empty())
    {
        if (channel->getChannelTopic().empty())
            ServerMessages::SendErrorMessage(fd, RPL_NOTOPIC, channelName, "No topic is set");
        else
            ServerMessages::SendErrorMessage(fd, RPL_TOPIC, channelName, channel->getChannelTopic());
        return;
    }

    // If the channel has the +t mode, only operators can change the topic
    if (channel->isRestrictedTopic() && !channel->isOperator(user->getFd()))
    {
        ServerMessages::SendErrorMessage(fd, ERR_CHANOPRIVSNEEDED, channelName);
        return;
    }

    // Defines new topic
    std::string newTopic = messageContent.message;
    channel->setChannelTopic(newTopic);

    // Notifies all channel members about the change in the topic change
    std::string topicMessage = ":" + user->getNick() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    channel->broadcastMessageTemp(topicMessage, fd);
}

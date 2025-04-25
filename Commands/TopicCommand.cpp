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

    // Checks if user is registered
    if (user->getStatus() < User::AUTHENTICATED)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, user->getNick());
        return;
    }

    // Checks if command has at least the name of the channel
    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, user->getNick(), "TOPIC");
        return;
    }

    std::string channelName = messageContent.tokens[1];
    Channel *channel = ChannelService::getInstance().findChannel(channelName);

    // Channel exists?
    if (!channel)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOSUCHCHANNEL, user->getNick(), channelName);
        return;
    }

    // Verifies if the user is in the channel
    if (!channel->isUserInChannel(fd))
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTONCHANNEL, user->getNick(), channelName);
        return;
    }

    // if no topic message is provided, return the current topic
    if (messageContent.message.empty())
    {
        std::string currentTopic = channel->getChannelTopic();
        if (currentTopic.empty())
        {
            // Send "No topic is set" if the topic is empty
            std::string response = ":" + std::string(SERVER_NAME) + " 331 " + user->getNick() + " " + channelName + " :No topic is set\r\n";
            send(fd, response.c_str(), response.size(), 0);
        }
        else
        {
            // Send the current topic
            std::string response = ":" + std::string(SERVER_NAME) + " 332 " + user->getNick() + " " + channelName + " :" + currentTopic + "\r\n";
            send(fd, response.c_str(), response.size(), 0);
        }
        return;
    }

    // If the channel has the +t mode, only operators can change the topic
    if (channel->isRestrictedTopic() && !channel->isOperator(fd))
    {
        ServerMessages::SendErrorMessage(fd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
        return;
    }

    // Define new topic
    std::string newTopic = messageContent.message;
    channel->setChannelTopic(newTopic);
    // Notify all channel members about the change in the topic
    std::string topicMessage = ":" + user->getNick() + " TOPIC " + channelName + " " + newTopic + "\r\n";
    send(fd, topicMessage.c_str(), topicMessage.size(), 0);
    channel->broadcastMessageTemp(topicMessage, fd);
}

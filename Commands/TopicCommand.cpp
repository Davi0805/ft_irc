/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:16:16 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/25 18:35:47 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"

TopicCommand::TopicCommand() {}

TopicCommand::~TopicCommand() {}

void TopicCommand::execute(MessageContent messageContent, int fd)
{
    User *user = UserService::getInstance().findUserByFd(fd);
    if (!user) return;

    // Verifica se o usuário está registrado
    if (user->getStatus() < User::AUTHENTICATED)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, "TOPIC");
        return;
    }

    // Verifica se o comando contém pelo menos um argumento (nome do canal)
    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "TOPIC");
        return;
    }

    std::string channelName = messageContent.tokens[1];
    Channel *channel = ChannelService::getInstance().findChannel(channelName);

    // Verifica se o canal existe
    if (!channel)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOSUCHCHANNEL, channelName);
        return;
    }

    // Verifica se o usuário está no canal
    if (!channel->isUserInChannel(fd))
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTONCHANNEL, channelName);
        return;
    }

    // Se não houver mensagem, retorna o tópico atual do canal
    if (messageContent.message.empty())
    {
        if (channel->getChannelTopic().empty())
            ServerMessages::SendErrorMessage(fd, RPL_NOTOPIC, channelName, "No topic is set");
        else
            ServerMessages::SendErrorMessage(fd, RPL_TOPIC, channelName, channel->getChannelTopic());
        return;
    }

    // Se o canal tem modo +t, apenas operadores podem mudar o tópico
    if (channel->isRestrictedTopic() && !channel->isOperator(user->getFd()))
    {
        ServerMessages::SendErrorMessage(fd, ERR_CHANOPRIVSNEEDED, channelName);
        return;
    }

    // Define o novo tópico
    std::string newTopic = messageContent.message;
    channel->setChannelTopic(newTopic);

    // Notifica todos no canal sobre a mudança de tópico
    std::string topicMessage = ":" + user->getNick() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    channel->broadcastMessageTemp(topicMessage, fd);
}

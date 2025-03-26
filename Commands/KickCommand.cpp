/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:05:26 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/26 14:34:15 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "KickCommand.hpp"

KickCommand::KickCommand()
{}

KickCommand::~KickCommand() {}

void KickCommand::execute(MessageContent messageContent, int clientFd)
{
	User *user = UserService::getInstance().findUserByFd(clientFd);
	if (!user || !user->isAuthenticated())
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTREGISTERED, user->getNick());
		return;
	}
	std::cout << "[DEBUG] Executing KICK command" << std::endl;
	if (messageContent.tokens.size() < 3) // Pelo menos 3 parâmetros: KICK <channel> <user>
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NEEDMOREPARAMS, "KICK");
		return;
	}

	std::vector<std::string> channelNames = Utils::split(messageContent.tokens[1], ',');
	std::vector<std::string> targetNicks = Utils::split(messageContent.tokens[2], ',');

	std::string reason = "No reason";
	if (!messageContent.message.empty())
		reason = messageContent.message;

	if (channelNames.size() != targetNicks.size())
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NEEDMOREPARAMS, "KICK");
		return;
	}

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string channelName = channelNames[i];
		std::string targetNick = targetNicks[i];

		Channel *channel = ChannelService::getInstance().findChannel(channelName);
		if (!channel)
		{
			ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHCHANNEL, user->getNick(), channelName);
			continue;
		}

		User *targetUser = UserService::getInstance().findUserByNickname(targetNick);
		if (!targetUser)
		{
			ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHNICK, user->getNick(), targetNick);
			continue;
		}

		if (!channel->isUserInChannel(clientFd))
		{
			ServerMessages::SendErrorMessage(clientFd, ERR_NOTONCHANNEL, user->getNick(), channelName);
			continue;
		}

		if (!channel->isUserInChannel(targetUser->getFd()))
		{
			ServerMessages::SendErrorMessage(clientFd, ERR_USERNOTINCHANNEL, user->getNick(), targetNick, channelName);
			continue;
		}

		if (!channel->isOperator(clientFd))
		{
			ServerMessages::SendErrorMessage(clientFd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
			continue;
		}

		// Enviar mensagem de KICK para todos no canal
		std::stringstream kickMessage;
		kickMessage << ":" << user->getNick() << " KICK " << channelName << " " << targetNick << " :" << reason << "\r\n";
		send(clientFd, kickMessage.str().c_str(), kickMessage.str().size(), 0);
		channel->broadcastMessageTemp(kickMessage.str(), clientFd);

		// Remover o usuário do canal
		channel->removeUser(targetUser->getFd());
	}
}

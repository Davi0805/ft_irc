/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:51:57 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/23 14:05:19 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCommand.hpp"

KickCommand::KickCommand(UserService &_userService, ChannelService &_channelService)
	: _userService(&_userService), _channelService(&_channelService) {}

KickCommand::~KickCommand() {}

void KickCommand::execute(MessageContent messageContent, int clientFd)
{
	User *user = _userService->findUserByFd(clientFd);
	if (!user || !user->isAuthenticated())
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTREGISTERED, user->getNick());
		return;
	}
	if (messageContent.tokens.size() < 3)
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NEEDMOREPARAMS, "KICK");
		return;
	}

	std::string channelName = messageContent.tokens[1];
	std::string targetNick = messageContent.tokens[2];

	Channel *channel = _channelService->findChannel(channelName);
	if (!channel)
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHCHANNEL, user->getNick(), channelName);
		return;
	}

	User *targetUser = _userService->findUserByNickname(targetNick);
	if (!targetUser)
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHNICK, user->getNick(), targetNick);
		return;
	}

	if (!channel->isUserInChannel(clientFd))
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTONCHANNEL, user->getNick(), channelName);
		return;
	}

	if (!channel->isUserInChannel(targetUser->getFd()))
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_USERNOTINCHANNEL, user->getNick(), targetNick, channelName);
		return;
	}

	if (!channel->isOperator(clientFd) && clientFd != targetUser->getFd())
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
		return;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:05:45 by lebarbos          #+#    #+#             */
/*   Updated: 2025/04/20 19:05:48 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"

InviteCommand::InviteCommand() {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(MessageContent messageContent, int clientFd)
{
	User *user = UserService::getInstance().findUserByFd(clientFd);
	if (!user || !user->isAuthenticated())  // Check if user is authenticated
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTREGISTERED, user->getNick());
		return;
	}
	if (messageContent.tokens.size() < 3) // Check if enough parameters are given
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NEEDMOREPARAMS, "INVITE");
		return;
	}

	std::string channelName = messageContent.tokens[2];
	std::string targetNick = messageContent.tokens[1];

	Channel *channel = ChannelService::getInstance().findChannel(channelName);
	if (!channel) // Channel does not exist
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHCHANNEL, user->getNick(), channelName);
		return;
	}

	User *targetUser = UserService::getInstance().findUserByNickname(targetNick);
	if (!targetUser) // Target user does not exist
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHNICK, user->getNick(), targetNick);
		return;
	}

	if (!channel->isUserInChannel(clientFd)) // Inviter must be on the channel
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTONCHANNEL, user->getNick());
		return;
	}

	if (channel->isUserInChannel(targetUser->getFd()))
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_USERONCHANNEL, user->getNick(), targetUser->getNick(), "");
		return;
	}

	if (!channel->isOperator(clientFd)) // Check if the user has operator privileges
	{
		ServerMessages::SendErrorMessage(clientFd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
		return;
	}

	// Invite the user
	channel->inviteUser(targetUser);
	UserService::getInstance().sendMessage(targetUser->getFd(),
    ":" + user->getNick() + "!~" + user->getUser() + "@host" + 
    " INVITE " + targetNick + " " + channelName + "\r\n");
}


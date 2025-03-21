/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:05:45 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/21 12:33:45 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCommand.hpp"

InviteCommand::InviteCommand(UserService &userService, ChannelService &channelService)
	: _userService(&userService), _channelService(&channelService) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(MessageContent messageContent, int clientFd)
{
	User *user = _userService->findUserByFd(clientFd);
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

	Channel *channel = _channelService->findChannel(channelName);
	if (!channel) // Channel does not exist
	{
		// _userService->sendMessage(clientFd, ERR_NOSUCHCHANNEL(user->getNick(), channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHCHANNEL, user->getNick(), channelName);
		return;
	}

	User *targetUser = _userService->findUserByNickname(targetNick);
	if (!targetUser) // Target user does not exist
	{
		// _userService->sendMessage(clientFd, ERR_NOSUCHNICK(user->getNick(), targetNick));
		ServerMessages::SendErrorMessage(clientFd, ERR_NOSUCHNICK, user->getNick(), targetNick);
		return;
	}

	if (!channel->isUserInChannel(clientFd)) // Inviter must be on the channel
	{
		// _userService->sendMessage(clientFd, ERR_NOTONCHANNEL(user->getNick(), channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_NOTONCHANNEL, user->getNick(), channelName);
		return;
	}

	if (channel->isUserInvited(targetUser) || channel->isUserInChannel(targetUser->getFd())) // Check if already invited
	{
		// _userService->sendMessage(clientFd, ERR_USERONCHANNEL(user->getNick(), targetNick, channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_USERONCHANNEL, user->getNick(), channelName);
		return;
	}

	if (!channel->isOperator(clientFd)) // Check if the user has operator privileges
	{
		// _userService->sendMessage(clientFd, ERR_CHANOPRIVSNEEDED(user->getNick(), channelName));
		ServerMessages::SendErrorMessage(clientFd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
		return;
	}

	// Invite the user
	channel->inviteUser(targetUser);
	_userService->sendMessage(targetUser->getFd(),
    ":" + user->getNick() + "!~" + user->getUser() + "@host" + 
    " INVITE " + targetNick + " " + channelName + "\r\n");
	

	std::cout << "[DEBUG]: User " << targetUser->getNick() << " invited to channel " << channel->getChannelName() << std::endl;
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:04:49 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/21 12:33:26 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

#include <string>
#include <sstream>

class InviteCommand : public Command
{
private:
	UserService *_userService;
	ChannelService *_channelService;
	
public:
	InviteCommand(UserService &userService, ChannelService &channelService);
	~InviteCommand();
	void execute(MessageContent messageContent, int clientFd);
};
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:33:33 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/18 18:52:08 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

class ModeCommand : public Command
{
private:
	UserService *_userService;
	ChannelService *_channelService;

public:
	ModeCommand(UserService &userSerice, ChannelService &channelService);
	~ModeCommand();
	void execute(MessageContent messageContent, int clientFd);
};
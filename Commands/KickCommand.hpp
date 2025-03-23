/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:50:51 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/23 11:54:26 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

class KickCommand : public Command
{
private:
	UserService *_userService;
	ChannelService *_channelService;
public:
	KickCommand(UserService &_userService, ChannelService &_channelService);
	~KickCommand();
	void execute(MessageContent messageContent, int fd) ;
};
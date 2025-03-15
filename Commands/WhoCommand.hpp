/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:53:09 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/13 14:53:44 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include <iostream>

class WhoCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;
public:
    WhoCommand(UserService& userService, ChannelService& channelService);
    ~WhoCommand();

    void execute(MessageContent messageContent, int fd);
};
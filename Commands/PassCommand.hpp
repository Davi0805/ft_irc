/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:32:40 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/16 20:29:48 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include "../Models/Server.hpp"
#include <iostream>

class PassCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;
public:
    PassCommand(UserService& userService, ChannelService& channelService);
    ~PassCommand();

    void execute(MessageContent messageContent, int fd);
};

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:41:56 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:42:08 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include <iostream>

class JoinCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;
public:
    JoinCommand(UserService& userService, ChannelService& channelService);
    ~JoinCommand();

    void execute(MessageContent messageContent, int fd);
};

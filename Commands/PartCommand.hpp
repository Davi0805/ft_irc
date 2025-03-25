/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:11:17 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/23 17:11:35 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include <iostream>

class PartCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;
public:
    PartCommand(UserService& userService, ChannelService& channelService);
    ~PartCommand();

    void execute(MessageContent messageContent, int fd);
};
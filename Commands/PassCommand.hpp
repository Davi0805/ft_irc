/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:32:40 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 15:32:38 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
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

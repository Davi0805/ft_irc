/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:32:40 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/19 17:10:15 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include "../Exceptions/Exceptions.hpp"
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

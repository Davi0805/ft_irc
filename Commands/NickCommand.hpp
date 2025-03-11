/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:15 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 15:32:19 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "Command.hpp"

#include <iostream>

class NickCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;
public:
    NickCommand(UserService& userService, ChannelService& channelService);
    ~NickCommand();

    void execute(MessageContent messageContent, int fd);
};

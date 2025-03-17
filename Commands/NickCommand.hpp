/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:15 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/17 15:16:12 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "Command.hpp"
#include "../Utils/Utils.hpp"

#include <iostream>

class NickCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;

    static const short _nicknameMaxLen = 8; // note: the static makes so it is only stored once for every instance
public:
    NickCommand(UserService& userService, ChannelService& channelService);
    ~NickCommand();

    void execute(MessageContent messageContent, int fd);
};

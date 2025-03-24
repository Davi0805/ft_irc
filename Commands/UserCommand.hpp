/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:54:55 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/24 17:35:45 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include <iostream>

class UserCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;

    bool ValidateUserCharset(const std::string &newNick) const;
    static const short _usernameMaxLen = 9; // note: the static makes so it is only stored once for every instance


public:
    UserCommand(UserService& userService, ChannelService& channelService);
    ~UserCommand();

    void execute(MessageContent messageContent, int fd);
};

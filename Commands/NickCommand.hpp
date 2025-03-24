/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:15 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/24 16:24:19 by fang             ###   ########.fr       */
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
    
    bool ValidateNickCharset(const std::string &newNick) const;

    static const short _nicknameMaxLen = 9; // note: the static makes so it is only stored once for every instance
public:
    NickCommand(UserService& userService, ChannelService& channelService);
    ~NickCommand();

    void execute(MessageContent messageContent, int fd);
};

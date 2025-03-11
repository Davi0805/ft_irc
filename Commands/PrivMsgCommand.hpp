/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:17:36 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 20:19:00 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

class PrivMsgCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;
public:
    PrivMsgCommand(UserService& userService, ChannelService& channelService);
    ~PrivMsgCommand();

    void execute(MessageContent messageContent, int fd);
};
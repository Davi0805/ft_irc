/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:35:27 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 00:36:58 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

class QuitCommand : public Command
{
private:
    UserService* _userService;
    ChannelService* _channelService;
public:
    QuitCommand(UserService& userService, ChannelService& channelService);
    ~QuitCommand();

    void execute(MessageContent messageContent, int fd);
};
	
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:15:08 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 19:33:08 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Services/ChannelService.hpp"
#include "../Services/UserService.hpp"
#include "../Models/MessageContent.hpp"
#include "../Models/Channel.hpp"
#include "../Utils/ServerMessages.hpp"

#include <sys/socket.h>
class Command
{
public:
    Command();
    virtual ~Command();

    // Metodo abstrato para executar
    virtual void execute(MessageContent messageContent, int fd) = 0;
};

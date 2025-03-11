/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:43:32 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 22:51:17 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sstream>

#include "../Models/Channel.hpp"
#include "../Models/User.hpp"

#define SERVER_NAME "ft_irc.42Network.local"

class ServerMessages
{
private:
    /* data */
public:
    ServerMessages(/* args */);
    ~ServerMessages();

    // TODO: Substituir fd e nickname apenas por um User* para clean code
    static void MensagemAutenticado(int fd, std::string nickname);

    
    static void JoinedChannel(User* user, Channel* channel);
};
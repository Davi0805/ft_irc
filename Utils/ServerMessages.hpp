/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:43:32 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 21:54:43 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sstream>

#define SERVER_NAME "ft_irc.42Network.local"

class ServerMessages
{
private:
    /* data */
public:
    ServerMessages(/* args */);
    ~ServerMessages();

    static void MensagemAutenticado(int fd, std::string nickname);
};
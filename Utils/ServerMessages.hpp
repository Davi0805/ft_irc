/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:43:32 by davi              #+#    #+#             */
/*   Updated: 2025/03/13 15:04:53 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sstream>

#include "../Models/Channel.hpp"
#include "../Models/User.hpp"

#include "../Models/MessageContent.hpp"


// SERVER_NAME CRIADO MERAMENTE ILUSTRATIVAMENTE
// JA QUE O NOSSO SERVER RODA LOCALMENTE
// POR SER EDUCACIONAL APENAS

#define SERVER_NAME "ft_irc.42Network.local"


/* 
    CLASSE COM OBJETIVO DE ADICIONAR METODOS ESTATICOS
    COM PROPOSITO DE FORMATAR E/OU MANDAR MENSAGEMS PARA OS CLIENTES
    SEGUINDO AS NORMAS DOCUMENTADAS NO RFC DO IRC    
*/
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

    static std::string ConvertMessageContentToA(MessageContent content);

    static std::string PrivMsgFormatter(User* user, Channel* channel, std::string message);

    static std::string PrivMsgFormatter(User* sender, User* receiver, std::string message);

    static std::string WhoReply(User* user, Channel* channel);
};
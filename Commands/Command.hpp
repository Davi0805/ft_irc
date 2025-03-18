/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/23 15:55:33 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "../Services/ChannelService.hpp"
#include "../Services/UserService.hpp"
#include "../Models/MessageContent.hpp"
#include "../Models/Channel.hpp"
#include "../Utils/ServerMessages.hpp"
#include "../Exceptions/Exceptions.hpp"


#include <sys/socket.h>
class Command
{
private:
    // TODO: METHODO NAO ABSTRATO PARA CONTAGEM DE ARGUMENTOS
    // TODO: JA QUE TODOS OS COMANDOS PRECISAM CHECAR SE POSSUEM
    // TODO: NUMERO MINIMO DE ARGS
public:
    Command();
    //Command(UserService& userService, ChannelService& channelService); //todo check if used
    virtual ~Command();

    // Metodo abstrato para executar
    virtual void execute(MessageContent messageContent, int fd) = 0;

};

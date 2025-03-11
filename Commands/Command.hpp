/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:15:08 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 21:57:36 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Services/ChannelService.hpp"
#include "../Services/UserService.hpp"
#include "../Models/MessageContent.hpp"
#include "../Utils/ServerMessages.hpp"

#include <sys/socket.h>
class Command
{
private:
    // TODO: METHODO NAO ABSTRATO PARA CONTAGEM DE ARGUMENTOS
    // TODO: JA QUE TODOS OS COMANDOS PRECISAM CHECAR SE POSSUEM
    // TODO: NUMERO MINIMO DE ARGS
public:
    Command();
    //Command(UserService& userService, ChannelService& channelService);
    ~Command();

    // Metodo abstrato para executar
    virtual void execute(MessageContent messageContent, int fd) = 0;

};

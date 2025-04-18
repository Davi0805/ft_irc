/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:15:08 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 18:39:23 by lebarbos         ###   ########.fr       */
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
private:
    // TODO: METHODO NAO ABSTRATO PARA CONTAGEM DE ARGUMENTOS
    // TODO: JA QUE TODOS OS COMANDOS PRECISAM CHECAR SE POSSUEM
    // TODO: NUMERO MINIMO DE ARGS
public:
    Command();
    virtual ~Command();

    virtual void execute(MessageContent messageContent, int fd) = 0;

};

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:15:08 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 15:32:24 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Services/ChannelService.hpp"
#include "../Services/UserService.hpp"
#include "../Models/MessageContent.hpp"

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

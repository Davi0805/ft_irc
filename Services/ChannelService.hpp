/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:53 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 01:00:17 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/Channel.hpp"

// unordered map faz parte do c++ 11
// #include <unordered_map>
#include <map>
#include <iostream>

/* 
    ESSA CLASSE TEM O PROPOSITO DE GERENCIAR E IMPLEMENTAR METHODOS PARA
    TODAS OS CANAIS DO PROGRAMA, ONDE TEM COMO OBJETIVO, SEPARAR E/OU
    DESACOPLAR AS LOGICAS PARA MAIS ESCALABILIDADE, FACIL MANUTENCAO E 
    VERSATILIDADE NA HORA DE CRIAR NOVOS COMANDOS OU IMPLEMENTAR NOVAS 
    FEATURES

    USE CASES: AO CRIAR UM COMANDO, TODOS COMANDOS POSSUEM PONTEIROS PARA
    ESSA CLASSE, ONDE VOCE PODE SIMPLESMENTE REUTILIZAR OS METODOS E/OU 
    IMPLEMENTAR NOVOS QUE IRAO FACILITAR/SIMPLICAR LOGICAS FUTURAS
 */

class ChannelService
{
private:
    // channelName - Channel*
    std::map<std::string, Channel*> _channels;
public:
    ChannelService(/* args */);
    ~ChannelService();

    Channel *get_or_createChannel(std::string channelName);

    Channel* findChannel(std::string channelName);

    bool isUserPartOfChannel(std::string nickname, std::string channelName);
    bool isUserPartOfChannel(int fd, std::string channelName);
};

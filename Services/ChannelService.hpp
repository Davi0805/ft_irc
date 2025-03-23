/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:53 by davi              #+#    #+#             */
/*   Updated: 2025/03/23 16:29:47 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/Channel.hpp"
#include "../Utils/ServerMessages.hpp"

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
    
    ChannelService(/* args */);

    static ChannelService* _instance;
    
public:
    ~ChannelService();

    static ChannelService& getInstance();

    void quitFromAllChannels(User* user, std::string message);

    Channel *get_or_createChannel(std::string channelName);

    Channel* findChannel(std::string channelName);

    bool isUserPartOfChannel(std::string nickname, std::string channelName);
    bool isUserPartOfChannel(int fd, std::string channelName);
};

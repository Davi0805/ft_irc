/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:53 by davi              #+#    #+#             */
/*   Updated: 2025/03/25 15:48:44 by lebarbos         ###   ########.fr       */
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


    void handleModeChange(User* user, int fd, const std::string& channelName, const std::string& modeString, std::vector<std::string>& params);
    // void applyMode(Channel* channel, char mode, bool addMode, std::vector<std::string>& params, size_t& paramIndex);
    void applyMode(Channel* channel, char mode, bool addMode, std::vector<std::string>& params, size_t& paramIndex, int fd);
    void broadcastModeChange(Channel* channel, User* user, int fd, const std::string& modeString, std::vector<std::string>& params, size_t paramIndex);
};

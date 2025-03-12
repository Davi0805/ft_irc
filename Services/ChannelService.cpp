/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:45 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 01:06:25 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelService.hpp"

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

ChannelService::ChannelService(/* args */)
{
}

ChannelService::~ChannelService()
{
}

/* 
    CLASSE COM OBJETIVO DE RETORNAR UM CANAL EXISTENTE OU CRIAR CANAL
    QUANDO TAL CANAL NAO EXISTIR AINDA, SENDO SEMELHANTE AO COMANDO JOIN
    DO IRC
 */
Channel* ChannelService::get_or_createChannel(std::string channelName)
{
    std::map<std::string, Channel*>::iterator it;
    
    it = _channels.find(channelName);

    // Se existir retorna channel
    // Se n, cria um novo channel
    if (it != _channels.end())
        return it->second;
    else
    {
        _channels[channelName] = new Channel(channelName);

        return _channels[channelName];
    }
}

/* 
    METHODO PARA ENCONTRAR TAL CANAL SEM CRIA-LO, CASO N ENCONTRE,
    PARA USAR EM CASOS ONDE SE TRATA MAIS DE CHECAGEM DO QUE PROPRIAMENTE
    DO COMANDO JOIN
 */
Channel* ChannelService::findChannel(std::string channelName)
{
    std::map<std::string, Channel*>::iterator it;

    it = _channels.find(channelName);
    if (it != _channels.end())
        return it->second;
    else
        return NULL;
}

/* 
    METHODO PARA CHECAGEM SE TAL USUARIO, FAZ PARTE DE TAL CANAL, GERALMENTE CHECAGEM
    PARA TESTES HARDCODED PROVAVELMENTE EXECUTADOS VIA NCAT
*/
bool ChannelService::isUserPartOfChannel(std::string nickname, std::string channelName)
{
    Channel* temp = findChannel(channelName);
    if (temp == NULL)
        return false;
    
    for (size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (nickname == temp->getUsers()[i]->getNick())
            return true;
    }
    return false;
}

/* 
    METHODO PARA CHECAGEM SE TAL USUARIO, FAZ PARTE DE TAL CANAL, GERALMENTE CHECAGEM
    PARA TESTES HARDCODED PROVAVELMENTE EXECUTADOS VIA NCAT
*/
bool ChannelService::isUserPartOfChannel(int fd, std::string channelName)
{
    Channel* temp = findChannel(channelName);
    if (temp == NULL)
        return false;

    for(size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (fd == temp->getUsers()[i]->getFd())
            return true;
    }
    return false;
}
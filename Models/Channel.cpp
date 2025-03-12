/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:26 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 01:32:30 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* 
    CLASSE PARA MODELAGEM/DTO (DATA TRANSFER OBJECT),
    ONDE EXISTE MAIS COM O OBJETIVO DE GUARDAR DADOS
    DO QUE PROPRIAMENTE EXECUTAR E/OU TER LOGICAS COMPLEXAS    
*/



/* 
    SETA NOME DO CANAL AO CONSTRUIR TAL OBJETO
    E INICIA O TOPICO DO CANAL COMO DEFAULT,
    PODENDO DEPOIS SER MODIFICADO UTILIZANDO
    UM SIMPLES SETTER QUE NAO EXISTE POR ENQUANTO,
    POIS NAO EXISTE TAL COMANDO TBM
 */
Channel::Channel(std::string ChannelName)
{
    this->_channelName = ChannelName;
    this->topic = "Default topic";
}

Channel::~Channel()
{
}

/* 
    ADICIONA PONTEIRO DE USUARIO AO VETOR
    DE USUARIOS QUE FAZEM PARTE DO CANAL

    OBS: TALVEZ MODIFICAR PARA REFERENCIA
    O ARG DESSE METODO, PARA CODIGO MAIS SAFE
 */
void Channel::AddUser(User* user)
{
    _users.push_back(user);
}

std::vector<User*> Channel::getUsers() const
{
    return this->_users;
}

std::string Channel::getChannelName() const
{
    return this->_channelName;
}

std::string Channel::getChannelTopic() const
{
    return this->topic;
}


/* 
    IMPLEMENTADO PARA SER UTILIZADO NO SERVER MESSAGES
    PARA FACILMENTE RETORNAR UMA STRING COM TODOS USERS
    DE DETERMINANDO CANAL, DEIXANDO O SERVER MESSAGES
    UM POUCO MAIS CLEAN, SENDO QUE JA ESTA MUITO FEIO
    AQUELA PARTE DO CODIGO
 */
std::string Channel::getAllUserString() const
{
    std::string result;

    for(size_t i = 0; i < _users.size(); i++)
    {
        result.append(_users[i]->getNick());
        result.append(" ");
    }
    return result;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:26 by davi              #+#    #+#             */
/*   Updated: 2025/03/18 15:32:14 by lebarbos         ###   ########.fr       */
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
    if (_users.size() == 1)
        _operators.insert(user->getFd());
}


// DEVE SER POSSÍVEL REMOVER OPERADOR?
void Channel::removeUser(User* user)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == user->getFd())
        {
            _users.erase(_users.begin() + i);
            break;
        }
    }
}

bool Channel::isUserInChannel(int fd) const
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == fd)
            return true;
    }
    return false;
}

bool Channel::isOperator(int fd) const
{
    return _operators.find(fd) != _operators.end();
}

void Channel::promoteToOperator(int fd)
{
    _operators.insert(fd);
}

void Channel::demoteOperator(int fd)
{
    _operators.erase(fd);
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


void Channel::setChannelPassword(std::string password)
{
    this->_password = password;
}

void Channel::setChannelLimit(int limit)
{
    this->_userLimit = limit;
}

void Channel::setChannelTopic(std::string topic)
{
    this->topic = topic;
}


void Channel::setChannelMode(int mode)
{
    this->_mode = mode;
}
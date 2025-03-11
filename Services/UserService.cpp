/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:06:28 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 00:30:32 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserService.hpp"

UserService::UserService()
{
}

UserService::~UserService()
{
}


// Cria apenas a referencia no _userByFd
// Pq nessa altura ainda nao possui nick
// que sera definido com comandos
void UserService::CreateUserByFd(int fd)
{
    _usersByFd[fd] = new User(fd);
}

void UserService::RemoveUserByFd(int fd)
{
    std::unordered_map<int, User*>::iterator it;
    std::unordered_map<std::string, User*>::iterator itNick;
    
    it = _usersByFd.find(fd);
    
    // ! Nao sei se a norminette deixa
    // auto it = _userByFd.find(fd);

    // Apaga ambos maps e deleta User
    if (it != _usersByFd.end())
    {
        itNick = _userByNick.find(it->second->getNick());
        if (itNick != _userByNick.end())
            _userByNick.erase(itNick);
        delete it->second;
        _usersByFd.erase(it);
    }
}

// Procura User pelo Fd
// Se existir retorna ponteiro para User
// Se nao, retorna NULL
User* UserService::findUserByFd(int fd)
{
    std::unordered_map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
        return it->second;
    else
        return NULL;
}

User* UserService::findUserByNickname(std::string nickname)
{
    std::unordered_map<std::string, User*>::iterator it;
    it = _userByNick.find(nickname);

    if (it != _userByNick.end())
        return it->second;
    else
        return ; // TODO: EXCEPTION
}

void UserService::SetNickByFd(std::string nickname, int fd)
{
    std::unordered_map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    // ! Nao sei se a norminette deixa
    // auto it = _userByFd.find(fd);

    if (it != _usersByFd.end())
    {
        it->second->setNick(nickname);
        
        _userByNick[nickname] = it->second;
    }
    else
        return ; // TODO: EXCEPTION
}

void UserService::SetUserByFd(std::string username, int fd)
{
    std::unordered_map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    // ! Nao sei se a norminette deixa
    // auto it = _userByFd.find(fd);

    if (it != _usersByFd.end())
        it->second->setUser(username);
    else
        return ; // TODO: EXCEPTION
}

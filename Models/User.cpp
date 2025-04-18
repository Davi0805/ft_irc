/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:23 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:03:26 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/* 
    CLASS FOR MODELING/DTO (DATA TRANSFER OBJECT),
    MAINLY USED FOR STORING DATA RATHER THAN EXECUTING
    OR HAVING COMPLEX LOGIC    
*/


/* 
    INSTANTIATES THE USER OBJECT, SETTING THE CONNECTION FD
    AND AUTH STATUS AS CONNECTED
*/
User::User(int fd)
{
    this->_fd = fd;
    this->_status = CONNECTED;
}

User::~User() {}

/* 
    SETS THE NICKNAME FOR THE USER AND MODIFIES
    THE USER'S STATUS, MAKING IT POSSIBLE TO SET USER    
*/
void User::setNick(std::string nickname)
{
     this->_nickname = nickname;
}

void User::setStatus(Status status)
{
    this->_status = status;   
}

/* 
    SETS USERNAME AND STATUS AFTER HAVING ALREADY SET
    PASS AND NICKNAME, AND COMPLETES AUTHENTICATION   
*/
void User::setUser(std::string username)
{
    this->_username = username; 
}

void User::setRealName(std::string realName)
{
    this->_realName = realName;
}



/* 
    METODO GET PARA SER UTILIZADO, POR EXEMPLO
    QUANDO FOR EXECUTAR ALGUM COMANDO, POIS SO
    E PERMITIDO EXECUTAR COMANDOS SEM SEREM OS DE
    AUTENTICACAO(PASS, NICK, USER) QUANDO JA TIVER
    AUTENTICADO
 */
bool User::isAuthenticated() const
{
    if (this->_status == AUTHENTICATED)
        return true;
    return false;
}

User::Status User::getStatus() const
{
    return this->_status;
}

std::string User::getNick()
{
    return this->_nickname;
}

int User::getFd() const
{
    return this->_fd;
}

std::string User::getUser() const
{
    return _username;
}

std::string User::getRealName () const
{
    return _realName;
}

// Checks if the password passed as a parameter checks out with the server password
// Returns true or false accordingly
bool User::checkPassword(std::string &pass) const
{
    if (Server::getInstance().getPassword() == pass)
        return true;
    return false;
}

bool User::checkBotPassword(std::string &pass) const
{
    if (Server::getInstance().getBotPassword() == pass)
        return true;
    return false;
}

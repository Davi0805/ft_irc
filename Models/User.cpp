/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:23 by davi              #+#    #+#             */
/*   Updated: 2025/03/27 18:58:27 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/* 
    CLASSE PARA MODELAGEM/DTO (DATA TRANSFER OBJECT),
    ONDE EXISTE MAIS COM O OBJETIVO DE GUARDAR DADOS
    DO QUE PROPRIAMENTE EXECUTAR E/OU TER LOGICAS COMPLEXAS    
*/


/* 
    INSTANCIA O OBJ USUARIO, JA SETANDO O FD DA CONEXAO
    E STATUS DE AUTH COMO CONNECTED
*/
User::User(int fd)
{
    this->_fd = fd;
    this->_status = CONNECTED;
}

User::~User()
{
}

/* 
    TODO: VERIFICAR A SENHA AQUI OU EM OUTRA PARTE MAIS RELACIONADA A LOGICA
    MODIFICA O STATUS PARA PASS RECEIVED,
    ONDE JA SE PODE UTILIZAR METODOS SUBSEQUENTES
    COMO SETNICK E SETUSER
*/
void User::setPass()
{
    // TODO
    // this->_status = 
}


/* 
    SETA O NICKNAME PARA TAL USUARIO E MODIFICA
    O STATUS DO USUARIO, JA SENDO POSSIVEL SETAR USER    
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
    SETA USUARIO E STATUS APOS JA TER SETADO
    PASS E NICKNAME E FINALIZA AUTENTICACAO   
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

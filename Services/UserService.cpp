/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:06:28 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 02:19:41 by davi             ###   ########.fr       */
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
    std::map<int, User*>::iterator it;
    std::map<std::string, User*>::iterator itNick;
    
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

    // TODO: ADICONAR EXCEPTION SE EXISTIR ERRO NO RFC
}

void UserService::RemoveUserByNick(std::string nickname)
{
    std::map<std::string, User*>::iterator it;
    std::map<int, User*>::iterator itFd;

    it = _userByNick.find(nickname);

    if (it != _userByNick.end())
    {
        itFd = _usersByFd.find(it->second->getFd());
        _userByNick.erase(it);
        delete itFd->second;
        _usersByFd.erase(itFd);
    }
    // TODO: ADICONAR EXCEPTION SE EXISTIR ERRO NO RFC
}

// Procura User pelo Fd
// Se existir retorna ponteiro para User
// Se nao, retorna NULL
User* UserService::findUserByFd(int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
        return it->second;
    else
        return NULL;
}

User* UserService::findUserByNickname(std::string nickname)
{
    std::map<std::string, User*>::iterator it;
    it = _userByNick.find(nickname);

    if (it != _userByNick.end())
        return it->second;
    else
        return NULL; // TODO: EXCEPTION
}

void UserService::SetNickByFd(std::string nickname, int fd)
{
    std::map<int, User*>::iterator it;
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
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    // ! Nao sei se a norminette deixa
    // auto it = _userByFd.find(fd);

    if (it != _usersByFd.end())
        it->second->setUser(username);
    else
        return ; // TODO: EXCEPTION
}


// TESTE UNITARIO

// c++ -Wall -Wextra -Werror -std=c++98 UserService.cpp ../Models/User.cpp 


/* int main(void)
{

    // TESTE - CRIAR E ATRIBUIR NICK
    UserService service;

    service.CreateUserByFd(1);
    service.SetNickByFd("TIGRINHO", 1);
    service.SetUserByFd("TIGRINHO_USER", 1);

    service.CreateUserByFd(2);
    service.SetNickByFd("TIGRINHO2", 2);
    service.SetUserByFd("TIGRINHO2_USER", 2);

    // TESTE FIND POR FD

    std::cout << "TESTES COM RESULTADO POSITIVO-------------" << std::endl << std::endl;

    std::cout << std::endl << "TESTE FIND POR FD" << std::endl;

    User* teste1 = service.findUserByFd(1);

    User* teste2 = service.findUserByNickname("TIGRINHO2");

    std::cout << "-NICK DO USER: " << teste1->getNick() << std::endl;

    std::cout << std::endl << "TESTE FIND POR NICK" << std::endl;

    teste2 = service.findUserByNickname("TIGRINHO2");

    std::cout << "-NICK DO USER: " << teste2->getNick() << std::endl;

    std::cout << std::endl << "TESTES COM RESULTADO NEGATIVO-------------" << std::endl << std::endl;
    
    std::cout << std::endl << "TESTE DELETE USER PELO FD EXISTENTE" << std::endl;
    
    std::cout << "-User existe? " << teste1 << std::endl;
    
    service.RemoveUserByFd(1);

    teste1 = service.findUserByFd(1);

    std::cout << "-Depois de deletar ainda existe? " << teste1 << std::endl;

    std::cout << std::endl << "TESTE DELETE USER PELO NICK EXISTENTE" << std::endl;

    std::cout << "-User existe? " << teste2 << std::endl;

    service.RemoveUserByNick("TIGRINHO2");

    teste2 = service.findUserByNickname("TIGRINHO2");

    std::cout << "-Depois de deletar ainda existe? " << teste2 << std::endl;


    std::cout << std::endl << "TESTE DELETE USER INEXISTENTE" << std::endl;

    service.RemoveUserByFd(3);

    teste1 = service.findUserByFd(3);

    std::cout << "-User existe? " << teste1 << std::endl;
        
} */

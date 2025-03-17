/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:06:28 by davi              #+#    #+#             */
/*   Updated: 2025/03/17 18:09:06 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserService.hpp"

/* 
    ESSA CLASSE TEM O PROPOSITO DE GERENCIAR E IMPLEMENTAR METHODOS PARA
    TODAS OS USUARIOS DO PROGRAMA, ONDE TEM COMO OBJETIVO, SEPARAR E/OU
    DESACOPLAR AS LOGICAS PARA MAIS ESCALABILIDADE, FACIL MANUTENCAO E 
    VERSATILIDADE NA HORA DE CRIAR NOVOS COMANDOS OU IMPLEMENTAR NOVAS 
    FEATURES

    USE CASES: AO CRIAR UM COMANDO, TODOS COMANDOS POSSUEM PONTEIROS PARA
    ESSA CLASSE, ONDE VOCE PODE SIMPLESMENTE REUTILIZAR OS METODOS E/OU 
    IMPLEMENTAR NOVOS QUE IRAO FACILITAR/SIMPLICAR LOGICAS FUTURAS
 */

UserService::UserService()
{
}

UserService::~UserService()
{
}


/* 
    QUANDO UM NOVO CLIENT CONECTA, ELE APENAS ALOCA UM NOVO USUARIO
    E ATRIBUI NO MAP <FD, USER*>. ONDE SEU FD FICA REGISTRADO, POIS 
    O NICKNAME SERA ATRIBUIDO SOMENTE AO UTILIZAR COMANDO NICK
 */
void UserService::CreateUserByFd(int fd)
{
    _usersByFd[fd] = new User(fd);
}


/* 
    METHODO PARA DELETAR INSTANCIA DO USUARIO, FECHAR O FD DA CONEXAO
    E APAGAR REFERENCIA NO MAP DO FD E NO MAP DE NICKNAME, CASO EXISTA
 */
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
    close(fd); // fecha fd do user desconectado
    

    // TODO: ADICONAR EXCEPTION SE EXISTIR ERRO NO RFC
}

/* 
    METHODO PARA DELETAR INSTANCIA DO USUARIO, FECHAR O FD DA CONEXAO
    E APAGAR REFERENCIA NO MAP DO NICK NAME E SUBSEQUENTEMENTE DO MAP
    DO FD
 */
void UserService::RemoveUserByNick(std::string nickname)
{
    std::map<std::string, User*>::iterator it;
    std::map<int, User*>::iterator itFd;

    it = _userByNick.find(nickname);

    if (it != _userByNick.end())
    {
        itFd = _usersByFd.find(it->second->getFd());
        close(it->second->getFd()); // fecha o fd do user
        _userByNick.erase(it);
        delete itFd->second;
        _usersByFd.erase(itFd);
    }
    // TODO: ADICONAR EXCEPTION SE EXISTIR ERRO NO RFC
}

/* 
    METHOD DE BUSCA DE USUARIO UTILIZANDO FD.
    SE EXISTIR, RETORNA UM PONTEIRO PARA A INSTANCIA DO USUARIO
    CASO NAO, RETORNA NULL
 */
User* UserService::findUserByFd(int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
        return it->second;
    else
        return NULL;
}

/* 
    METHOD DE BUSCA DE USUARIO UTILIZANDO NICKNAME.
    SE EXISTIR, RETORNA UM PONTEIRO PARA A INSTANCIA DO USUARIO
    CASO NAO, RETORNA NULL
 */
User* UserService::findUserByNickname(std::string nickname)
{
    std::map<std::string, User*>::iterator it;
    it = _userByNick.find(nickname);

    if (it != _userByNick.end())
        return it->second;
    else 
        return NULL; // TODO: EXCEPTION
}

/* 
    METODO PARA SETAR O NICKNAME DE ALGUM USUARIO APENAS PELO FD,
 */
void UserService::SetNickByFd(std::string nickname, int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
    {
        // This bit is when we are replacing nickname. We must remove the entry of the last one
        std::map<std::string, User*>::iterator it2;
        it2 = _userByNick.find(it->second->getNick());
        if (it2 != _userByNick.end())
            _userByNick.erase(it2);
        
        it->second->setNick(nickname); // map<int, User*>
        _userByNick[nickname] = it->second; // map<string, User*>
    }
    else
        return ; // TODO: EXCEPTION
}

/* 
    METODO PARA SETAR O NICKNAME DE ALGUM USUARIO APENAS PELO FD,
 */
void UserService::SetUserByFd(std::string username, int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);


    if (it != _usersByFd.end())
        it->second->setUser(username);
    else
        return ; // TODO: EXCEPTION
}


void UserService::SetRealNameByFd(std::string realName, int fd)
{
    std::map<int, User*>::iterator it;
    it = _usersByFd.find(fd);

    if (it != _usersByFd.end())
        it->second->setRealName(realName);
    else
        return ; // TODO: Expt
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

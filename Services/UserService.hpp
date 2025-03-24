/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:07:28 by davi              #+#    #+#             */
/*   Updated: 2025/03/24 15:59:58 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/User.hpp"
#include "../Utils/Utils.hpp"

#include "unistd.h"
#include <sys/socket.h>
#include <iostream>
#include <map>

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

class UserService
{
private:
    UserService();

    // Infelizmente redundante porem otimizado para buscas
    std::map<int, User*> _usersByFd;
    std::map<std::string, User*> _userByNick;

    static UserService* _instance;

public:
    ~UserService();
    const std::map<int, User*> getFdsMap() const;
    static UserService& getInstance();
    
    User* findUserByFd(int fd);
    User* findUserByNickname(std::string nickname);

     // Nickname etc setados posteriormente com o fd usando os metodos do service
    void CreateUserByFd(int fd);
    void RemoveUserByFd(int fd);
    void RemoveUserByNick(std::string username);

    void SetNickByFd(std::string nickname, int fd);
    void SetUserByFd(std::string username, int fd);
    void SetRealNameByFd(std::string realName, int fd);

    // TEMPORARIA?
    void sendMessage(int fd, const std::string &message);

};
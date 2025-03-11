/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:41 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 17:53:04 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

#include "User.hpp"

class Channel
{
private:
    // ! SUBSTITUIR POR UNORDEREDMAP ???
    std::vector<User*> _users;
    std::string topic;
    std::string _channelName;
    
    //TODO: PENSAR EM UMA LOGICA DE OPERATOR/ADMIN
    
    //TODO: ADICIONAR METHODOS PARA GET E SET DE VARIAVEIS 

    
    enum mode
    {
        INVITE_ONLY,
        // TENHO QUE LER A DOCUMENTACAO,
        // POREM SEPARAR MODES COM O ENUM
    };
public:
    Channel(std::string ChannelName);
    ~Channel();

    void AddUser(std::string nickname);
    void AddUser(User *user);
};

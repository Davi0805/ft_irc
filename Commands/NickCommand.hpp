/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:15 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/26 14:04:38 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "Command.hpp"
#include "../Utils/Utils.hpp"

#include <iostream>



class NickCommand : public Command
{
private:
    
    bool ValidateNickCharset(const std::string &newNick) const;

    static const short _nicknameMaxLen = 9; // note: the static makes so it is only stored once for every instance
public:
    NickCommand();
    ~NickCommand();

    void execute(MessageContent messageContent, int fd);
};

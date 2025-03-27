/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:56:29 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 15:46:42 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

//Default Constructor
Bot::Bot()
{
    
}
//Destructor
Bot::~Bot()
{
    if (_botSocketFd >= 0)
        close(_botSocketFd);
    _botSocketFd = -1;
}


bool Bot::IsPortValid(char *port)
{
    
}

void Bot::Connect()
{
    
}

void Bot::Register()
{
    
}

void Bot::RecieveData()
{
    
}

void Bot::HandleData()
{
    
}

void Bot::ExecuteCommands()
{
    
}



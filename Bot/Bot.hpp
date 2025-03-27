/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:46:23 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 15:51:20 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//Includes
#include <unistd.h> // close
#include <cstdlib> // exit
#include <limits> // numeric limits
#include <iostream> // cout cerr endl
#include <cerrno> // errno
#include <cstring> // strerror

class Bot
{
    private:
        const char *_botName = "MyBot";
        const char *_botServerPass = "d082dc29b65ed2e80db4ef542ca7ee8c4d3971043cbe214090f61b238493893f";

        int _botSocketFd;
        
    public:
        //Default Constructor
        Bot();
        //Destructor
        ~Bot();

        static bool IsPortValid(const char *port);
        
        void Connect();
        void Register();
        void RecieveData();
        void HandleData();
        void ExecuteCommands();
        
        
};
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:33:37 by artuda-s          #+#    #+#             */
/*   Updated: 2025/03/31 18:59:42 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "../Services/UserService.hpp"
#include "../Models/User.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>

class PingCommand : public Command
{
private:
    std::string _token;
public:
    PingCommand();
    ~PingCommand();
    std::string getToken() const;
    
    // Will recieve a PING :<token> and respond with a PONG :<token>
    void execute(MessageContent messageContent, int fd);
};



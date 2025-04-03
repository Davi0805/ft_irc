/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:41:56 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/04/03 13:00:25 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include <iostream>

class JoinCommand : public Command
{
private:

public:
    JoinCommand();
    ~JoinCommand();

    std::vector<std::string> commaTokenizer(std::string target);
    void execute(MessageContent messageContent, int fd);
};

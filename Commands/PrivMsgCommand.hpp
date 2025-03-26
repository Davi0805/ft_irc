/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsgCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:17:36 by davi              #+#    #+#             */
/*   Updated: 2025/03/26 14:13:35 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

class PrivMsgCommand : public Command
{
private:
public:
    PrivMsgCommand();
    ~PrivMsgCommand();

    void execute(MessageContent messageContent, int fd);
};
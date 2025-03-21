/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorMessages.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:55:03 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/21 10:55:57 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "../Utils/ServerMessages.hpp"


// Declarações das funções (extern evita múltiplas definições)
std::string ERR_NOTREGISTERED(const std::string &nick);
std::string ERR_NEEDMOREPARAMS(const std::string &nick, const std::string &command);
std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &channel);
std::string ERR_NOSUCHNICK(const std::string &nick, const std::string &targetNick);
std::string ERR_NOTONCHANNEL(const std::string &nick, const std::string &channel);
std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &targetNick, const std::string &channel);
std::string ERR_CHANOPRIVSNEEDED(const std::string &nick, const std::string &channel);
std::string RPL_INVITING(const std::string &nick, const std::string &target, const std::string &channel);

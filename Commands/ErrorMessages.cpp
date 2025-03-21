/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorMessages.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:54:52 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/21 10:54:54 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorMessages.hpp"
#include <sstream>

std::string ERR_NOTREGISTERED(const std::string &nick) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 451 " << nick << " :You have not registered\r\n";
    return ss.str();
}

std::string ERR_NEEDMOREPARAMS(const std::string &nick, const std::string &command) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 461 " << nick << " " << command << " :Not enough parameters\r\n";
    return ss.str();
}

std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &channel) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 403 " << nick << " " << channel << " :No such channel\r\n";
    return ss.str();
}

std::string ERR_NOSUCHNICK(const std::string &nick, const std::string &targetNick) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 401 " << nick << " " << targetNick << " :No such nick\r\n";
    return ss.str();
}

std::string ERR_NOTONCHANNEL(const std::string &nick, const std::string &channel) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 442 " << nick << " " << channel << " :You're not on that channel\r\n";
    return ss.str();
}

std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &targetNick, const std::string &channel) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 443 " << nick << " " << targetNick << " " << channel << " :is already on channel\r\n";
    return ss.str();
}

std::string ERR_CHANOPRIVSNEEDED(const std::string &nick, const std::string &channel) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 482 " << nick << " " << channel << " :You're not channel operator\r\n";
    return ss.str();
}

std::string RPL_INVITING(const std::string &nick, const std::string &target, const std::string &channel) {
    std::ostringstream ss;
    ss << SERVER_NAME << " 341 " << nick << " " << target << " " << channel << "\r\n";
    return ss.str();
}

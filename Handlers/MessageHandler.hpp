/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:03 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 12:49:21 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>


class MessageHandler
{
private:
    std::string getMessage(std::string& buffer, std::size_t it);
    std::string getMessage(std::string& strBegin, std::istringstream& stream);
public:
    MessageHandler();
    ~MessageHandler();

    std::vector<std::string> ircTokenizer(std::string buffer);
};
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:03 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 12:53:30 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

// TALVEZ DEPOIS SUBSTITUIR POR METHODOS ESTATICOS
// JA QUE NAO POSSUI VARIAVEIS PROPRIAS

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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:19 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 12:51:31 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHandler.hpp"

// EXEMPLOS DE MENSAGEM - SEM O PREFIXO POR ENQUANTO
//
// PRIVMSG #meu_canal :Olá, galera!
// PRIVMSG Amigo123 :Oi, tudo bem?


// ! PROV DEIXAR ESSSA CLASSE NA MAO DO ARTHUR, POR TER BOAS PRATICAS DE PARSE E TOKENIZER


MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

// TODO: DISCUTIR COM O GRUPO SE O TOKENIZER
// TODO: RECEBERA O BUFFER INTEIRO OU O FD
// TODO: POREM, IRRELEVANTE JA QUE N POSSUI
// TODO: TESTE DE CARGA
std::vector<std::string> MessageHandler::ircTokenizer(std::string buffer)
{
    std::istringstream stream(buffer);
    std::string tempToken;
    std::vector<std::string> tokens;

    std::string message;

    while (stream >> tempToken)
    {
        if (tempToken[0] == ':')
        {
            message = getMessage(buffer, buffer.find(tempToken));
            //message = getMessage(tempToken, stream);   
            break ;
        }
        std::cout << "TOKENS: " << tempToken << std::endl;
    }

    std::cout << "MENSAGEM ISOLADA: " << message << std::endl;

    return tokens;
}


// METHODO PEQUENO MAS APENAS PARA DEIXAR MAIS CLEAN
std::string MessageHandler::getMessage(std::string& buffer, std::size_t it)
{
    std::string result;

    result.append(buffer, it);

    return result;
}



// ! DEPRECATED
// ! ACREDITO QUE NAO SEJA A MELHOR PRATICA
std::string MessageHandler::getMessage(std::string& strBegin, std::istringstream& stream)
{
    std::string fullMessage;
    std::string tempToken;

    // APPEND DA STR QUE ENCONTROU O DELIMITADOR DE MSG
    fullMessage.append(strBegin);
    
    // APPEND DO RESTO
    while (stream >> tempToken)
    {
        fullMessage.append(tempToken);
    }
    return fullMessage;
}

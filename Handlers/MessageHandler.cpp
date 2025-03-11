/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:19 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 17:17:45 by dmelo-ca         ###   ########.fr       */
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
    RegisterCommands();
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::CreateEvent(int fd)
{
    std::cout << "[DEBUG]: NOVO USUARIO CRIADO COM FD = " << fd << std::endl;
    _userService.CreateUserByFd(fd);
}

// TODO: Fazer logica para handle de desconexao
void MessageHandler::HandleEvent(int fd)
{
    char buffer[1024];
    ssize_t bytesRead;

    MessageContent messageContent;

    bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0)
    {
        std::cerr << "FATAL: Erro ao ler do descritor de arquivo" << std::endl;
        return;
    } else if (bytesRead == 0) 
    {
        std::cerr << "INFO: User disconected" << std::endl;
        _userService.RemoveUserByFd(fd);
        return;
    }

    buffer[bytesRead] = '\0';

    //std::cout << "Recebido do fd " << fd << ": " << buffer << std::endl;

    // Joga Tokens e mensagens para struct
    //std::cout << "[DEBUG]: Entrando no tokenizer" << std::endl;
    messageContent = ircTokenizer(std::string(buffer));
    
    //std::cout << "[DEBUG]: Entrando no ProcessCommand" << std::endl;
    ProcessCommand(messageContent, fd);
}

void MessageHandler::ProcessCommand(MessageContent messageContent, int clientFd)
{
    // TODO: IMPLEMENTAR LOGICA MAIS CLEAN
    if (messageContent.tokens[0] == "PASS")
        _commands["PASS"]->execute(messageContent, clientFd);
    else if (messageContent.tokens[0] == "NICK")
        _commands["NICK"]->execute(messageContent, clientFd);
    else if (messageContent.tokens[0] == "USER")
        _commands["USER"]->execute(messageContent, clientFd);
}

void MessageHandler::RegisterCommands()
{
    _commands["PASS"] = new PassCommand(_userService, _channelService);
    _commands["NICK"] = new NickCommand(_userService, _channelService);
    _commands["USER"] = new UserCommand(_userService, _channelService);
}

MessageContent MessageHandler::ircTokenizer(std::string buffer)
{
    std::istringstream stream(buffer);
    std::string tempToken;
    std::vector<std::string> tokens;

    MessageContent messageContent;

    std::string message;
    
    //std::cout << "[DEBUG]: Antes do while de append stream" << std::endl;
    // ! O PROGRAMA CRASHA NESSE WHILE QUANDO MANDA APENAS \t e enter
    while (stream >> tempToken)
    {
        // ! NS SE ISSO PODE CRASHAR O PROGRAMA
        //std::cout << "[DEBUG]: Antes na condicao que encontra mensagem" << std::endl;

        if (tempToken[0] == ':')
        {
            //std::cout << "[DEBUG]: Entra na condicao que encontra mensagem" << std::endl;
            message = getMessage(buffer, buffer.find(tempToken));
            break ;
        }
        tokens.push_back(tempToken);
        //std::cout << "TOKENS: " << tempToken << std::endl;
    }

    //std::cout << "MENSAGEM ISOLADA: " << message << std::endl;

    messageContent.tokens = tokens;
    messageContent.message = message;

    return messageContent;
}


// METHODO PEQUENO MAS APENAS PARA DEIXAR MAIS CLEAN
std::string MessageHandler::getMessage(std::string& buffer, std::size_t it)
{
    std::string result;

    result.append(buffer, it);

    return result;
}

bool MessageHandler::IsOnlyTab(std::string& buffer)
{
    if (buffer.find_first_not_of('\t') == std::string::npos)
        return true;
    return false;
}



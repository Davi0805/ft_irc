/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:43:04 by davi              #+#    #+#             */
/*   Updated: 2025/03/31 22:42:26 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerMessages.hpp"


/* 
    CLASSE COM OBJETIVO DE ADICIONAR METODOS ESTATICOS
    COM PROPOSITO DE FORMATAR E/OU MANDAR MENSAGEMS PARA OS CLIENTES
    SEGUINDO AS NORMAS DOCUMENTADAS NO RFC DO IRC    
*/

ServerMessages::ServerMessages()
{
}

ServerMessages::~ServerMessages()
{
}

/* 
    APOS AUTENTICAR, DE ACORDO COM MINHAS PESQUISAS O SERVIDOR
    COSTUMA MANDAR AS RESPOSTAS COM CODIGO DE 001 A 005
    CONTENDO INFORMCAOES PERTINENTES DO SERVIDOR, QUE CLARAMENTE
    O NOSSO NAO TEM, POR SER UM SERVIDOR DESENVOLVIDO PARA APRENDIZAGEM    
*/

void ServerMessages::SendWelcomeMessage(int fd, std::string nickname)
{
    std::ostringstream stream;
    std::string svName = Server::getInstance().getServerName();
    // 001 - Welcome message
    stream << ":" << svName << " 001 " << nickname << " :Welcome to ft_irc!\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    // 002 - Host info
    stream << ":" << svName << " 002 " << nickname << " :Host is "<< svName << " and is set locally lol" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    // 003 - Server creation date
    stream << ":" << svName << " 003 " << nickname << " :Server was created - what, in march? i dont know xd" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 004 - Server features
    stream << ":" << svName << " 004 " << nickname << " ft_irc-1.0 ioklt iotkl" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 005 - Some configs
    stream << ":" << svName << " 005 " << nickname << " CHANTYPES=# PREFIX=(o)@ CHANMODES=,,, NICKLEN=9 CASEMAPPING=ascii :are supported by this server" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // LUSERS
    stream << ":" << svName << " 251 " << nickname << " :There are " << UserService::getInstance().getFdsMap().size() << " users on 1 server (PS: this is network-wide)\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    stream << ":" << svName << " 255 " << nickname << " :There are " << UserService::getInstance().getFdsMap().size() << " users and 0 servers (PS: this is local)\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // MOTD cuz why not
    /**
     * 375 start of motd
     * 372 motd message
     * 376 end of motd
     */
    stream << ":" << svName << " 375 " << nickname << " :- " << svName << " Message of the Day -" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    stream << ":" << svName << " 372 " << nickname << " :- This is a line of the MOTD" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    stream << ":" << svName << " 372 " << nickname << " :- This is another line of the MOTD"  << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    stream << ":" << svName << " 376 " << nickname << " :End of /MOTD command" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    return;
}

/* 
    O MESMO QUE FOI CITADO ACIMA, POREM PARA QUANDO SE JUNTA EM UM CANAL
*/
void ServerMessages::JoinedChannel(User* user, Channel* channel)
{
    std::ostringstream stream;

    std::cout << "[DEUBG] JOINED CHANNEL CALLED LALALAL" << std::endl;

    // RFC E SEUS CODIGOS DE RESPOSTA
    // APENAS MENSAGEM DIZENDO QUE DEU JOIN NO SERVER COM SUCESSO
    stream << ":" << user->getNick() << "!~user@host JOIN " << channel->getChannelName() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    channel->broadcastMessageTemp(stream.str(), user->getFd());
    stream.str("");

    // 332 - CONTENDO O TOPICO DO CANAL
    stream << ":" << Server::getInstance().getServerName() << " 332 " << user->getNick() << " " << channel->getChannelName() << " : " << channel->getChannelTopic() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 353 - LISTA DE USUARIOS DO CANAL
    stream << ":" << Server::getInstance().getServerName() << " 353 " << user->getNick() << " = " << channel->getChannelName() << " :" << channel->getAllUserString() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    std::cout << "USERS: " << channel->getAllUserString() << std::endl;

    // 366 - CODIGO QUE NOTIFICA FIM DA LISTA
    stream << ":" << Server::getInstance().getServerName() << " 366 " << user->getNick() << " " << channel->getChannelName() << " :" << "End of /NAMES list" << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);

    channel->broadcastMessageTemp(stream.str(), user->getFd());
    stream.str("");   
}


/* 
    DIFERENTE DOS METODOS ANTERIORES, ESSE METODO E PARA FORMATACAO.
    OU SEJA, ELE NAO ENVIA NADA, POIS NESSE CASO ESSE METODO SERA UTILIZADO
    MAJORITARIAMENTE EM BROADCAST DE MENSAGEM, SENDO MELHOR TER A LOGICA DE BROADCAST
    SEPARADA DA CLASSE SERVER MESSAGE    
*/
std::string ServerMessages::PrivMsgFormatter(User* user, Channel* channel, std::string message)
{
    std::ostringstream stream;

    stream << ":" << user->getNick() << "!~" << user->getUser() << "@host PRIVMSG " << channel->getChannelName() << " :" << message << "\r\n";
    
    return stream.str();   
}

std::string ServerMessages::PrivMsgFormatter(User* sender, User* receiver, std::string message)
{
    std::ostringstream stream;

    stream << ":" << sender->getNick() << "!~" << sender->getUser() << "@host PRIVMSG " << receiver->getNick() << " :" << message << "\r\n";
    
    return stream.str();
}

std::string ServerMessages::ConvertMessageContentToA(MessageContent content)
{
    std::string result;

    for (size_t i = 0; i < content.tokens.size(); i++)
    {
        result.append(content.tokens[i]);
        result.append(" ");
    }
    result.append(content.message);
    result.append("\r\n"); // indiferente, porem boa pratica
                        // apararentemente n funciona ao fazer
                        // por outros computadores pois o protocolo
                        // precisa de acesso a port forwarding
                        // via rede privada
                        // e provavelmente o vitor deve ter bloqueado
    return result;
}

std::string ServerMessages::WhoReply(User* user, Channel* channel)
{
    std::ostringstream stream;

    std::vector<User*> tempChannel = channel->getUsers();
    std::string svName = Server::getInstance().getServerName();

    for (size_t i = 0; i < tempChannel.size(); i++)
    {
        stream << ":" << svName << " 352 " << user->getNick() << " " << channel->getChannelName() << " host " << svName << tempChannel[i]->getNick() << " H :0 " << tempChannel[i]->getRealName() << "\r\n";
    }
    stream << ":" << svName << " 315 " << user->getNick() << " " << channel->getChannelName() << " :End of /WHO list." << "\r\n";

    return stream.str();
}

/** 
 * TODO DOXYGEN PLS
 * 
 * @brief Sends error code and a message associated with that error code
 * @param fd client fd to send error message to
 * @param errorCode error number identifier
 * @param nickname client nickname
 * @param param1 might not be used in some commands but needed in some others
 * @param param2 might not be used in some commands but needed in some others
 */
void ServerMessages::SendErrorMessage(int fd, int errorCode, const std::string& nickname, const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::ostringstream stream;

    // Adiciona o prefixo do servidor e o código do erro
    stream << ":" << Server::getInstance().getServerName() << " " << errorCode;
    
    if (!nickname.empty())
        stream << " " << nickname;

    // TODO implementar cada parametro corretamente para cada erro? quem se chega a frente? kkkkkk
    // Se houver um alvo (ex: um canal ou usuário específico), adicionamos
    if (!param1.empty())
        stream << " " << param1;

    // Associa o código de erro à mensagem correspondente
    switch (errorCode)
    {
        case ERR_NOSUCHNICK: stream << " :No such nick/channel"; break;
        case 402: stream << " :No such server"; break;
        case ERR_NOSUCHCHANNEL: stream << " :No such channel"; break;
        case ERR_CANNOTSENDTOCHAN: stream << " :Cannot send to channel"; break;
        case 405: stream << " :You have joined too many channels"; break;
        case ERR_NORECIPIENT: stream << " :No recipient given"; break;
        case ERR_NOTEXTTOSEND: stream << " :No text to send"; break;
        case ERR_UNKNOWNCOMMAND: stream << " :Unknown command"; break;
        case ERR_NONICKNAMEGIVEN: stream << " :No nickname given"; break;
        case ERR_ERRONEUSNICKNAME: stream << " :Erroneous nickname"; break;
        case ERR_NICKNAMEINUSE: stream << " :Nickname is already in use"; break;
        case 441: stream << " :They aren't on that channel"; break;
        case ERR_NOTONCHANNEL: stream << " :You're not on that channel"; break;
        case ERR_USERONCHANNEL: stream << " :is already on channel"; break;
        case ERR_NOTREGISTERED: stream << " :You have not registered"; break;
        case ERR_NEEDMOREPARAMS: stream << " :Not enough parameters"; break;
        case ERR_ALREADYREGISTERED: stream << " :Unauthorized command (already registered)"; break;
        case ERR_PASSWDMISMATCH: stream << " :Password incorrect - disconnecting..."; break;
        case ERR_KEYSET: stream << " :Channel key already set"; break;
        case ERR_CHANNELISFULL: stream << " :Channel is full"; break;
        case ERR_UNKNOWNMODE: stream << " :Unknown mode flag"; break;
        case ERR_INVITEONLYCHAN: stream << " :Invite-only channel"; break;
        case ERR_BANNEDFROMCHAN: stream << " :Banned from channel"; break;
        case ERR_BADCHANNELKEY: stream << " :Bad channel key"; break;
        case ERR_CHANOPRIVSNEEDED: stream << " :You're not channel operator"; break;
        case ERR_UMODEUNKNOWNFLAG: stream << " :Unknown MODE flag"; break;
        case ERR_USERSDONTMATCH: stream << " :Cannot change mode for other users"; break;
        default:  stream << " :Unknown error"; break;
    }

    stream << "\r\n";  // IRC requer a terminação \r\n

    std::cout << stream.str() << std::endl;
    // Envia a mensagem ao cliente
    send(fd, stream.str().c_str(), stream.str().size(), 0);
}

void ServerMessages::NickMsg(const std::map<int, User*> fdsMap, const std::string& oldNick, const std::string& newNick)
{
    std::ostringstream stream;

    stream << ":" << oldNick << " NICK :" << newNick << "\r\n";

    for (std::map<int, User*>::const_iterator it = fdsMap.begin(); it != fdsMap.end(); ++it)
    {
        send(it->first, stream.str().c_str(), stream.str().size(), 0);
    }
}

void ServerMessages::QuitMsg(Channel* channel, User* user, std::string message)
{
    std::ostringstream stream;

    stream << ":" << user->getNick() << "!~" << user->getUser() << "@host QUIT :" << message << "\r\n";

    for(size_t i = 0; i < channel->getUsers().size(); i++)
        send(channel->getUsers()[i]->getFd(), stream.str().c_str(), stream.str().size(), 0);
}

void ServerMessages::PartMsg(Channel* channel, User* user, std::string message)
{
    std::ostringstream stream;

    stream << ":" << user->getNick() << "!~" << user->getUser() << "@host PART " << channel->getChannelName() << " :" << message << "\r\n";

    for(size_t i = 0; i < channel->getUsers().size(); i++)
        send(channel->getUsers()[i]->getFd(), stream.str().c_str(), stream.str().size(), 0);
}

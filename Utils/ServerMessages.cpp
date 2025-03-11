/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:43:04 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 23:12:22 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerMessages.hpp"

ServerMessages::ServerMessages()
{
}

ServerMessages::~ServerMessages()
{
}

void ServerMessages::MensagemAutenticado(int fd, std::string nickname)
{
    std::ostringstream stream;

    // RFC E SEUS CODIGOS DE RESPOSTA
    // 001 - CODIGO DE BOAS VINDAS
    stream << ":" << SERVER_NAME << " 001 " << nickname << " :Bem vindo ao nosso irc, malandragem!\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    // 002 - INFO DO HOST - INSIGNIFICANTE PARA GNT POIS RODA EM LOCAL E SE TRATA DE UM PROJETO EDUCACIONAL
    stream << ":" << SERVER_NAME << " 002 " << nickname << " :O host setado "<< SERVER_NAME << " e ficticio pois esta hosteado localmente" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    // 003 - DATA DE CRIACAO DO SERVIDOR
    // ! NAO SEI SE PODEMOS USAR TIME FUNCTIONS, LOGO VOU SETAR UMA MENSAGEM ALEATORIA E DEPOIS MODIFICAMOS
    stream << ":" << SERVER_NAME << " 003 " << nickname << " :Na teoria, aqui passaria a data de criacao do servidor" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 004 - RECURSOS DO SERVIDOR
    // ! QUE PORRA QUE ISSO QUER DIZER??
    stream << ":" << SERVER_NAME << " 004 " << nickname << " :Nao tenho ideia do que recursos do servidor significa" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 005 - CONFIGURACOES DE CANAIS - APARENTEMENTE TIPOS DE CANAIS E OUTRAS COISAS
    // ! SE POSSIVEL ALGUEM DEPOIS REVISE ISSO
    stream << ":" << SERVER_NAME << " 005 " << nickname << " :JESUS, ALGUEM TA AFIM DE LER O RFC INTEIRO?" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
}

void ServerMessages::JoinedChannel(User* user, Channel* channel)
{
    std::ostringstream stream;

    // RFC E SEUS CODIGOS DE RESPOSTA
    // APENAS MENSAGEM DIZENDO QUE DEU JOIN NO SERVER COM SUCESSO
    stream << ":" << user->getNick() << "!~user@host JOIN :" << channel->getChannelName() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 332 - CONTENDO O TOPICO DO CANAL
    stream << ":" << SERVER_NAME << " 332 " << user->getNick() << " " << channel->getChannelName() << " :" << channel->getChannelTopic() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 353 - LISTA DE USUARIOS DO CANAL
    stream << ":" << SERVER_NAME << " 353 " << user->getNick() << " = " << channel->getAllUserString() << " :" << channel->getChannelTopic() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 366 - CODIGO QUE NOTIFICA FIM DA LISTA
    stream << ":" << SERVER_NAME << " 366 " << user->getNick() << " " << channel->getChannelName() << " :" << "Fim da lista de usuarios" << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
}

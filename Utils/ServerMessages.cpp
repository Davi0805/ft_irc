/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:43:04 by davi              #+#    #+#             */
/*   Updated: 2025/03/13 16:07:29 by dmelo-ca         ###   ########.fr       */
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

// TODO: ACREDITO QUE A LISTA DE USUARIOS SO VAI SER ENVIADA PELO COMANDO WHO
// TODO: POIS O CLIENT AO ENTRAR NO CANAL, MANDA O COMANDO WHO
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

/* 
    O MESMO QUE FOI CITADO ACIMA, POREM PARA QUANDO SE JUNTA EM UM CANAL
*/
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
    stream << ":" << SERVER_NAME << " 353 " << user->getNick() << " @ " << channel->getChannelName() << " :" << channel->getAllUserString() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 366 - CODIGO QUE NOTIFICA FIM DA LISTA
    stream << ":" << SERVER_NAME << " 366 " << user->getNick() << " " << channel->getChannelName() << " :" << "End of /NAMES list" << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
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

    stream << ":" << user->getNick() << "!~" << user->getUser() << "@host PRIVMSG " << channel->getChannelName() << " " << message << "\r\n";
    
    return stream.str();   
}

std::string ServerMessages::PrivMsgFormatter(User* sender, User* receiver, std::string message)
{
    std::ostringstream stream;

    stream << ":" << sender->getNick() << "!~" << sender->getUser() << "@host PRIVMSG " << receiver->getNick() << " " << message << "\r\n";
    
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

    for (size_t i = 0; i < tempChannel.size(); i++)
    {
        stream << ":" << SERVER_NAME << " 352 " << user->getNick() << " " << channel->getChannelName() << " host " << SERVER_NAME << tempChannel[i]->getNick() << " H :0 " << " COLOCAR AQUI O REAL NAME" << "\r\n";
    }
    stream << ":" << SERVER_NAME << " 315 " << user->getNick() << " " << channel->getChannelName() << " :End of /WHO list." << "\r\n";

    return stream.str();
}

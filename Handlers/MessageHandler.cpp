/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:19 by davi              #+#    #+#             */
/*   Updated: 2025/03/15 15:55:50 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHandler.hpp"

// EXEMPLOS DE MENSAGEM - SEM O PREFIXO POR ENQUANTO
//
// PRIVMSG #meu_canal :Olá, galera!
// PRIVMSG Amigo123 :Oi, tudo bem?


/* 
    CLASSE COM OBJETIVO DE DAR HANDLE DOS EVENTOS,
    SEJA CRIAR UM EVENTO (CRIAR USER E ARMAZENAR FD E DADOS DO USUARIO),
    HANDLE DE MENSAGENS CHAMANDO SERVICES E LOGICAS DE PARSING E TOKENIZACAO
    E REGISTRO DE COMANDOS
*/


MessageHandler::MessageHandler()
{
    RegisterCommands();
}

// APARENTA IRRELEVANTE DE CERTA FORMA, POIS NUNCA CHEGA AQUI
// POREM, ESTA FEITO CASO VOCES QUEIRAM TER UM COMANDO PARA 
// FECHAR O SERVIDOR HAHAHA
MessageHandler::~MessageHandler()
{
    FreeCommands();
}


/* 
    RECEBER FD APOS ACEITAR CONEXAO TCP E CRIAR USUARIO
    REFERENCIANDO O FD QUE SERA UTILIZADO DURANTE TODO O
    PROGRAMA PARA RECEBER E ENVIAR MENSAGENS
*/
void MessageHandler::CreateEvent(int fd)
{
    std::cout << "[DEBUG]: NOVO USUARIO CRIADO COM FD = " << fd << std::endl;
    _userService.CreateUserByFd(fd);
}


/* 
   METODO PARA DAR HANDLE DE EVENTO, ONDE LE O FD
   E ATRIBUI PARA UM BUFFER QUE SERA PROCESSADO E
   EXECUTADO UTILIZANDO OS METODOS DE TOKENIZACAO
   E PARSE
*/
// TODO: Fazer logica para handle de desconexao
bool MessageHandler::HandleEvent(int fd)
{
    MessageContent messageContent;
    char buffer[1024];
    std::string buf;

    while (true)
    {
        ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);
        
        if (bytesRead > 0)
        {
            buf.append(buffer, bytesRead);
            continue;
        }

        if (bytesRead == 0) // Desconexão
        {
            std::cerr << "INFO: User disconnected" << std::endl;
            _userService.RemoveUserByFd(fd);
            return false;
        }

        if (errno == EAGAIN || errno == EWOULDBLOCK)
            break; // Sem mais dados disponíveis, saída normal

        std::cerr << "FATAL: Erro ao ler do descritor de arquivo: " << strerror(errno) << std::endl;
        return false;
    }

    std::cout   << "[DEBUG]\n\
                    Recebido do fd " << fd  << ": " << buf << " EOF DO BUFFER" 
                << std::endl;

    /* 
        IF/ELSE, POIS DIFERENTE DO NCAT, O HEXCHAT MANDA TODAS
        AS CREDENCIAIS DE UMA VEZ, FAZENDO NECESSARIO OUTRO METODO
        DE TOKENIZACAO, SENDO NECESSSARIO SEPARAR OS COMANDOS PARA 
        EXECUCAO APROPRIADA
    */
    if (buf.find("\r\n") != std::string::npos) // hexchat ends in \r\n
    {
        std::vector<std::string> splittedCommands = splitDeVariosComandos(buf);
        for (size_t i = 0; i < splittedCommands.size(); i++)
        {
            messageContent = ircTokenizer(splittedCommands[i]);
            ProcessCommand(messageContent, fd);
        }
    }
    else
    {
        messageContent = ircTokenizer(std::string(buf));    
        ProcessCommand(messageContent, fd); 
    }
    return true;
}


/* 
    METODO PARA EXECUTAR OS COMANDOS REGISTRADOS
    TODO: ADICIONAR VERFICACOES ANTES, AO INVES DE
    TODO: FAZER IF/ELSE PROCURANDO TAL COMANDO
    ! TOMAR CUIDADO POIS EXECUTAR UM COMANDO QUE 
    ! NAO EXISTE CRASHA O PROGRAMA
*/
void MessageHandler::ProcessCommand(MessageContent messageContent, int clientFd)
{
    // TODO: IMPLEMENTAR LOGICA MAIS CLEAN POIS ESSE IF ELSE E DESNECESSARIO
    // TODO: APENAS CRIAR UM METODOS IsAnCommand() e depois botar o o token no map
    if (messageContent.tokens[0] == "PASS")
        _commands["PASS"]->execute(messageContent, clientFd);
    else if (messageContent.tokens[0] == "NICK")
        _commands["NICK"]->execute(messageContent, clientFd);
    else if (messageContent.tokens[0] == "USER")
        _commands["USER"]->execute(messageContent, clientFd);
    else if (messageContent.tokens[0] == "JOIN")
        _commands["JOIN"]->execute(messageContent, clientFd);
    else if (messageContent.tokens[0] == "PRIVMSG")
        _commands["PRIVMSG"]->execute(messageContent, clientFd);
    else if (messageContent.tokens[0] == "QUIT")
        _commands["QUIT"]->execute(messageContent, clientFd);
    else if(messageContent.tokens[0] == "WHO")
        _commands["WHO"]->execute(messageContent, clientFd);
}


/* 
    METODO PARA REGISTRAR TODOS OS COMANDOS, ONDE SEMPRE SAO ALOCADOS
    COM REFERENCIAS PARA OS SERVICES(USERSERVICE E CHANNELSERVICE)

    ! AO CRIAR NOVOS COMANDOS, SEMPRE REGISTRA-LOS POIS PODE CAUSAR 
    ! CRASHES SE TENTAR EXECUTAR ALGO QUE NAO EXISTE
*/
void MessageHandler::RegisterCommands()
{
    _commands["PASS"] = new PassCommand(_userService, _channelService);
    _commands["NICK"] = new NickCommand(_userService, _channelService);
    _commands["USER"] = new UserCommand(_userService, _channelService);
    _commands["JOIN"] = new JoinCommand(_userService, _channelService);
    _commands["PRIVMSG"] = new PrivMsgCommand(_userService, _channelService);
    _commands["QUIT"] = new QuitCommand(_userService, _channelService);
    _commands["WHO"] = new WhoCommand(_userService, _channelService);
}


/* 
    TOKENIZER COM FUNCAO DE PEGAR A MENSAGEM/COMANDO INTEIRO
    E SEPARAR EM TOKENS E MENSAGEM E ATRIBUIR NA STRUCT MESSAGECONTENT

    MOTIVO: SIMPLIFICACAO DE LOGICA E PARA EVITAR DESFORMATACAO DE MENSAGEM
    EM CASO DE ESPACOS OU TABS ALEATORIOS
*/
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

    if (tokens.size() == 0)
        tokens.push_back("");
    messageContent.tokens = tokens;
    messageContent.message = message;

    return messageContent;
}


/* 
    METODO PARA EXTRAIR MENSAGEM SEM TOKENIZACAO, ONDE RECEBE REFERENCIA
    DIRETA PARA O BUFFER E ITERADOR DE ONDE A MENSAGEM COMECA, RETORNANDO
    A MENSAGEM ISOLADA
*/
std::string MessageHandler::getMessage(std::string& buffer, std::size_t it)
{
    std::string result;

    result.append(buffer, it);

    return result;
}

// Faz split de quando o cliente manda varios commandos de uma vez
// O hexchat quando vai se autenticar manda varios comandos de uma vez
// em todas as vezes eles sao separados por \r\n
std::vector<std::string> MessageHandler::splitDeVariosComandos(std::string buffer)
{
    std::vector<std::string> result;
    size_t it;
    
    it = buffer.find_first_of("\r\n");
    while (it != std::string::npos)
    {
        result.push_back(buffer.substr(0, it + 2));
        std::cout << "[TOKENS - QUANDO VARIOS CMANDOS JUNTOS]" << buffer.substr(0, it) << std::endl;
        buffer.erase(0, it + 2);
        it = buffer.find_first_of("\r\n");
    }
    return result;
}

// APARENTA IRRELEVANTE DE CERTA FORMA, POIS NUNCA CHEGA AQUI
// POREM, ESTA FEITO CASO VOCES QUEIRAM TER UM COMANDO PARA 
// FECHAR O SERVIDOR HAHAHA
void MessageHandler::FreeCommands()
{
    for(std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
        delete (it->second);
    _commands.clear();
}



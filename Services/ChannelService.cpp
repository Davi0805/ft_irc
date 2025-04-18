/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:45 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:33:03 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelService.hpp"
#include <cstdlib>

// INICIANDO A VARIAVEL INSTANCE DO SINGLETON
ChannelService *ChannelService::_instance = NULL;

/*
    ESSA CLASSE TEM O PROPOSITO DE GERENCIAR E IMPLEMENTAR METHODOS PARA
    TODAS OS CANAIS DO PROGRAMA, ONDE TEM COMO OBJETIVO, SEPARAR E/OU
    DESACOPLAR AS LOGICAS PARA MAIS ESCALABILIDADE, FACIL MANUTENCAO E
    VERSATILIDADE NA HORA DE CRIAR NOVOS COMANDOS OU IMPLEMENTAR NOVAS
    FEATURES

    USE CASES: AO CRIAR UM COMANDO, TODOS COMANDOS POSSUEM PONTEIROS PARA
    ESSA CLASSE, ONDE VOCE PODE SIMPLESMENTE REUTILIZAR OS METODOS E/OU
    IMPLEMENTAR NOVOS QUE IRAO FACILITAR/SIMPLICAR LOGICAS FUTURAS
 */

ChannelService::ChannelService(/* args */)
{
}

ChannelService::~ChannelService()
{
}

ChannelService &ChannelService::getInstance()
{
    if (!_instance)
        _instance = new ChannelService();
    return *_instance;
}

void ChannelService::quitFromAllChannels(User *user, std::string message)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if (it->second->isUserInChannel(user->getFd()))
        {
            it->second->removeUser(user->getFd());
            ServerMessages::QuitMsg(it->second, user, message);
        }
    }
}

/*
    CLASSE COM OBJETIVO DE RETORNAR UM CANAL EXISTENTE OU CRIAR CANAL
    QUANDO TAL CANAL NAO EXISTIR AINDA, SENDO SEMELHANTE AO COMANDO JOIN
    DO IRC
 */
Channel *ChannelService::get_or_createChannel(std::string channelName)
{
    std::map<std::string, Channel *>::iterator it;

    if (channelName[0] != '#')
        channelName.insert(0, "#");

    it = _channels.find(channelName);

    // Se existir retorna channel
    // Se n, cria um novo channel
    if (it != _channels.end())
        return it->second;
    else
    {
        _channels[channelName] = new Channel(channelName);

        return _channels[channelName];
    }
}

/*
    METHODO PARA ENCONTRAR TAL CANAL SEM CRIA-LO, CASO N ENCONTRE,
    PARA USAR EM CASOS ONDE SE TRATA MAIS DE CHECAGEM DO QUE PROPRIAMENTE
    DO COMANDO JOIN
 */
Channel *ChannelService::findChannel(std::string channelName)
{
    std::map<std::string, Channel *>::iterator it;
    if (channelName[0] != '#')
        channelName.insert(0, "#");

    it = _channels.find(channelName);
    if (it != _channels.end())
        return it->second;
    else
        return NULL;
}

/*
    METHODO PARA CHECAGEM SE TAL USUARIO, FAZ PARTE DE TAL CANAL, GERALMENTE CHECAGEM
    PARA TESTES HARDCODED PROVAVELMENTE EXECUTADOS VIA NCAT
*/
bool ChannelService::isUserPartOfChannel(std::string nickname, std::string channelName)
{
    Channel *temp = findChannel(channelName);
    if (temp == NULL)
        return false;

    for (size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (nickname == temp->getUsers()[i]->getNick())
            return true;
    }
    return false;
}

/*
    METHODO PARA CHECAGEM SE TAL USUARIO, FAZ PARTE DE TAL CANAL, GERALMENTE CHECAGEM
    PARA TESTES HARDCODED PROVAVELMENTE EXECUTADOS VIA NCAT
*/
bool ChannelService::isUserPartOfChannel(int fd, std::string channelName)
{
    Channel *temp = findChannel(channelName);
    if (temp == NULL)
        return false;

    for (size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (fd == temp->getUsers()[i]->getFd())
            return true;
    }
    return false;
}

void ChannelService::broadcastSingleModeChange(Channel* channel, User* user, int fd, char mode, bool addMode, const std::string& param)
{
    std::stringstream msg;
    msg << ":" << user->getNick() << "!~" << user->getUser() << "@host MODE " 
        << channel->getChannelName() << " " << (addMode ? "+" : "-") << mode;

    if (!param.empty())
        msg << " " << param;

    msg << "\r\n";

    std::string finalMsg = msg.str();
    send(fd, finalMsg.c_str(), finalMsg.size(), 0);
    channel->broadcastMessageTemp(finalMsg, fd);
}

void ChannelService::handleModeChange(User *user, int fd, const std::string &channelName, const std::string &modeString, std::vector<std::string> &params)
{
    Channel *channel = findChannel(channelName);
    if (!channel)
        return;

    bool addMode = true;
    size_t paramIndex = 0;
    std::string validModes = "itkol";

    for (size_t i = 0; i < modeString.size(); i++)
    {
        char mode = modeString[i];
        if (mode == '+')
        {
            addMode = true;
            continue;
        }
        if (mode == '-')
        {
            addMode = false;
            continue;
        }

        if (validModes.find(mode) == std::string::npos)
        {
            std::stringstream errorMsg;
            errorMsg << ":server 472 " << user->getNick() << " " << mode << " :Unknown mode flag\r\n";
            send(fd, errorMsg.str().c_str(), errorMsg.str().size(), 0);
            continue;
        }

        applyMode(channel, mode, addMode, params, paramIndex, fd, user);
    }
}


bool ChannelService::applyMode(Channel *channel, char mode, bool addMode, std::vector<std::string> &params, size_t &paramIndex, int fd, User* user)
{
    switch (mode)
    {
    case 'i':
        channel->setInviteOnly(addMode);
        broadcastSingleModeChange(channel, user, fd, mode, addMode, "");
        return true;

    case 't':
        channel->setRestrictedTopic(addMode);
        broadcastSingleModeChange(channel, user, fd, mode, addMode, "");
        return true;

    case 'k':
        if (addMode)
        {
            if (paramIndex >= params.size())
            {
                ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
                return false;
            }
            std::string password = params[paramIndex++];
            if (channel->hasPassword())
            {
                ServerMessages::SendErrorMessage(fd, ERR_KEYSET, channel->getChannelName());
                return true;
            }
            channel->setRequiresPassword(password);
            broadcastSingleModeChange(channel, user, fd, mode, addMode, password);
        }
        else
        {
            channel->removePassword();
            broadcastSingleModeChange(channel, user, fd, mode, addMode, "");
        }
        return true;

        case 'l':
        if (addMode)
        {
            if (paramIndex >= params.size())
            {
                ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
                return false;
            }
            int limit = std::atoi(params[paramIndex++].c_str());
            if (limit <= 0)
            {
                ServerMessages::SendErrorMessage(fd, ERR_UNKNOWNMODE, "l", "Invalid limit value");
                return false;
            }
            channel->setUserLimit(limit);
    
            std::stringstream ss;
            ss << limit;
            broadcastSingleModeChange(channel, user, fd, mode, addMode, ss.str());
        }
        else
        {
            channel->removeUserLimit();
            broadcastSingleModeChange(channel, user, fd, mode, addMode, "");
        }
        return true;    
    case 'o':
        if (paramIndex >= params.size())
        {
            ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
            return false;
        }

        {
            std::stringstream ss(params[paramIndex++]);
            std::string nick;
            bool applied = false;
            while (std::getline(ss, nick, ','))
            {
                User *target = UserService::getInstance().findUserByNickname(nick);
                if (!target)
                {
                    ServerMessages::SendErrorMessage(fd, ERR_NOSUCHNICK, nick);
                    continue;
                }
                if (!channel->isUserInChannel(target->getFd()))
                {
                    ServerMessages::SendErrorMessage(fd, ERR_USERNOTINCHANNEL, nick, channel->getChannelName());
                    continue;
                }
                if (addMode)
                    channel->promoteToOperator(nick);
                else
                    channel->demoteOperator(nick);

                broadcastSingleModeChange(channel, user, fd, mode, addMode, nick);
                applied = true;
            }
            return applied;
        }

    default:
        ServerMessages::SendErrorMessage(fd, ERR_UNKNOWNMODE, std::string(1, mode), "is unknown mode char to me");
        return false;
    }
}



void ChannelService::broadcastModeChange(Channel *channel, User *user, int fd, const std::string &modeString, std::vector<std::string> &params, size_t paramIndex)
{
    std::stringstream msg;
    msg << ":" << user->getNick() << "!~" << user->getUser() << "@host MODE " << channel->getChannelName() << " " << modeString;
    for (size_t i = 0; i < paramIndex; i++)
        msg << " " << params[i];
    msg << "\r\n";
    send(fd, msg.str().c_str(), msg.str().size(), 0);
    channel->broadcastMessageTemp(msg.str(), fd);
}

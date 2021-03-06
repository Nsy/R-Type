#include <iostream>
#include "ProtocolLobby.hpp"
#include "Client.hpp"

ProtocolLobby::ProtocolLobby()
{
  this->actionmap[CHAT] = &ProtocolLobby::actionChat;
  this->actionmap[LOBBY_ERROR] = &ProtocolLobby::actionError;
  this->actionmap[LOBBY_PLAYERS] = &ProtocolLobby::actionPlayers;
}

ProtocolLobby::ProtocolLobby(ProtocolLobby const & other)
{
  this->actionmap = other.actionmap;
}

ProtocolLobby &		ProtocolLobby::operator=(ProtocolLobby const & other)
{
  if (&other != this)
    {
      this->actionmap = other.actionmap;
    }
  return (*this);
}

ProtocolLobby::~ProtocolLobby(void)
{
}

bool			ProtocolLobby::action(ushort instruction,
					      PacketData & data, Client &c)
{
  std::map<eProtocolPacketLobby, ptr_functlobby>::iterator it;
  ptr_functlobby	ptr;

  if (instruction >= LOBBY_MAX)
    (void)this->actionError(data, c);
  if ((it = this->actionmap.find(static_cast<eProtocolPacketLobby>(instruction))) == this->actionmap.end())
    {
      this->actionError(data, c);
      return false;
    }
  ptr = it->second;
  return ((this->*ptr)(data, c));
}

bool			ProtocolLobby::actionError(PacketData &, Client &)
{
  return (false);
}

bool			ProtocolLobby::actionChat(PacketData & data, Client &c)
{
  std::string login;
  std::string msg;

  login = data.getNextString();
  msg = data.getNextString();
  c.getGraphic().addToConversation("<" + login + "> " + msg + "\n");
  return (false);
}

bool			ProtocolLobby::actionPlayers(PacketData & data, Client &c)
{
  std::string login;
  short nb;
  std::list<std::string> &map = c.getGraphic().getPlayerNameList();
  std::list<std::string>::iterator it;

  bool exist = false;

  nb = data.getNextChar();
  login = data.getNextString();
  for (it = map.begin(); it != map.end(); ++it)
    {
      if (login == *it)
	exist = true;
    }
  if (nb == -1)
    {
      if (exist)
	map.remove(login);
    }
  if (nb == 1)
    {
      if (!exist)
	map.push_front(login);
    }
  return (false);
}

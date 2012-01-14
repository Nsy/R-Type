#include <iostream>
#include "ProtocolMovement.hpp"
#include "PlayerMovement.hpp"
#include "BulletMovement.hpp"
#include "Client.hpp"
#include "SpriteInfo.hpp"

ProtocolMovement::ProtocolMovement()
{
  this->actionmap[MOVEMENT_ERROR]	= &ProtocolMovement::actionError;
  this->actionmap[MOVE]			= &ProtocolMovement::actionMove ;
  this->actionmap[UPDATEPLAYER]		= &ProtocolMovement::actionUpdatePlayer;
  this->actionmap[UPDATEENEMY]		= &ProtocolMovement::actionUpdateEnemy;
  this->actionmap[COLLISION]		= &ProtocolMovement::actionCollision;
  this->actionmap[UPDATEBULLET]		= &ProtocolMovement::actionUpdateBullet;
  this->actionmap[NEWBULLET]		= &ProtocolMovement::actionNewBullet;

}

ProtocolMovement::ProtocolMovement(ProtocolMovement const & other)
{
  this->actionmap = other.actionmap;
}

ProtocolMovement &		ProtocolMovement::operator=(ProtocolMovement const & other)
{
  if (&other != this)
    {
      this->actionmap = other.actionmap;
    }
  return (*this);
}

ProtocolMovement::~ProtocolMovement(void)
{
}

bool			ProtocolMovement::action(ushort instruction,
						 PacketData & data, Client&c)
{
  std::map<eProtocolPacketMovement, ptr_funct>::iterator it;
  ptr_funct		ptr;

  if (instruction >= MOVEMENT_MAX)
    (void)this->actionError(data, c);
  if ((it = this->actionmap.find(static_cast<eProtocolPacketMovement>(instruction))) == this->actionmap.end())
    {
      (void)this->actionError(data, c);
      return false;
    }
  ptr = it->second;
  return ((this->*ptr)(data, c));
}

bool			ProtocolMovement::actionError(PacketData &, Client &)
{
  return (false);
}

bool		ProtocolMovement::actionMove(PacketData &data, Client &)
{
  (void)data;
  return (false);
}

bool		ProtocolMovement::actionUpdatePlayer(PacketData &data, Client &c)
{
  char id;
  std::string login;
  unsigned int x;
  unsigned int y;
  std::map<int, LibGraphic::PlayerMovement *> &playerMap = c.getGraphic().getPlayerMap();

  id = data.getNextChar();
  login = data.getNextString();
  x = data.getNextUint32();
  y = data.getNextUint32();

  if (id < 0 || id > 4)
    {
      std::cout << "[info] UpdatePlayer id not 0 to 4" << std::endl;
      return (false);
    }
  if (login != c.getGraphic().getLogin())
    {
      LibGraphic::Coord coord;
      if (playerMap.find(id) == playerMap.end())
  	{
	  LibGraphic::PlayerMovement *m;
	  m = new LibGraphic::PlayerMovement(c.getGraphic().getWindow(),
     c.getGraphic().getSprite("PlayerShip"), static_cast <eShipColor>(id));
	  playerMap[id] = m;
	}
      coord.x = x;
      coord.y = y;
      playerMap[id]->setCoord(coord);
    }
  else
    {
      c.getGraphic().setMyId(static_cast<eShipColor>(id));
    }
  return (false);
}

bool		ProtocolMovement::actionUpdateEnemy(PacketData &data, Client &)
{
  (void)data;
  return (false);
}

bool		ProtocolMovement::actionCollision(PacketData &data, Client &c)
{
  char id;
  std::string login;
  unsigned int x;
  unsigned int y;
  std::map<int, LibGraphic::PlayerMovement *> &playerMap = c.getGraphic().getPlayerMap();

  id = data.getNextChar();
  login = data.getNextString();
  x = data.getNextUint32();
  y = data.getNextUint32();
  LibGraphic::Coord coord;
  coord.x = 400;
  coord.y = 400;
  if (login == c.getGraphic().getLogin())
    {
      c.getGraphic().setMyPosition(coord);
    }
  else
    {
      if (playerMap.find(id) != playerMap.end())
  	{
	  playerMap[id]->setCoord(coord);
	}
    }
  return (false);
}

bool		ProtocolMovement::actionUpdateBullet(PacketData &data, Client &c)
{
  int		i = 0;
  int		nbBull;
  std::list<LibGraphic::BulletMovement *> &list = c.getGraphic().getBulletList();

  list.clear();
  nbBull = data.getNextUint32();
  while (i != nbBull)
    {
      LibGraphic::BulletMovement *b = new LibGraphic::BulletMovement(c.getGraphic().getWindow(),
				     c.getGraphic().getSprite("Missile"), LibGraphic::NORMAL_BULLET);

      LibGraphic::Coord coord;
      coord.x = data.getNextUint32();
      coord.y = data.getNextUint32();
		if (coord.x == 0 && coord.y == 0)
		{
		b->setCoord(coord);
		list.push_front(b);
		}
      ++i;
    }
  return (false);
}

bool		ProtocolMovement::actionNewBullet(PacketData &data, Client &)
{
  (void)data;
  return (false);
}


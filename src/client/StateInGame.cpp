#include <iostream>
#include "StateInGame.hpp"
#include "TileInfo.hpp"
#include "Language.hpp"
#include "SpriteInfo.hpp"

extern LibGraphic::Volume gVolume;
extern LibGraphic::Language language;
extern bool errorToPrint;

LibGraphic::StateInGame::StateInGame(std::map<std::string const, GraphicRessource *> const & ressourcesSprite,
						   std::map<std::string const, MyMusic *> const & ressourcesPlayList,
						   std::map<std::string const, MySound *> const & ressourcesSounds,
						   std::map<std::string const, sf::Font *> const & ressourcesFont,
						   sf::RenderWindow & app):
  _ressourcesSprite(ressourcesSprite), _ressourcesPlayList(ressourcesPlayList),
  _ressourcesSounds(ressourcesSounds), _ressourcesFont(ressourcesFont),
  _app(app), _lives(0), _score(0)
{
  this->_nextState = UNKNOWN_STATE;
  this->_mapClock.Reset();
  this->Clock.Reset();
  this->_frontClock.Reset();

  this->_myid = BLUESHIP;
  this->_player = new LibGraphic::PlayerMovement(this->_app,
					      this->getSprite("PlayerShip"));
}

LibGraphic::StateInGame::~StateInGame()
{
}

bool LibGraphic::StateInGame::init()
{
  return true;
}

int LibGraphic::StateInGame::MapX(int x)
{
  return (x * TILE_SIZE_X);
}

int LibGraphic::StateInGame::MapY(int y)
{
  return (y * TILE_SIZE_Y);
}

void LibGraphic::StateInGame::drawStarField()
{
  static int scale = 0;
  static int scalefront = 0;
  sf::Sprite &b = this->getSprite("Starfield" + this->_gameLvl);
  sf::Sprite &b2 = this->getSprite("Starfield" + this->_gameLvl);
  sf::Sprite &front = this->getSprite("SecondField" + this->_gameLvl);
  sf::Sprite &front2 = this->getSprite("SecondField" + this->_gameLvl);
  float time = this->Clock.GetElapsedTime();
  float timefront = this->_frontClock.GetElapsedTime();
  static int i = 0;

  if (time < 0.1)
    {
      b.SetPosition(0 - scale, 0);
      this->_app.Draw(b);
      b2.SetPosition(1680 - scale, 0);
      this->_app.Draw(b2);
    }
  else
    {
      scale += 2;
      if (scale >= 1680)
	scale = 0;
      b.SetPosition(0 - scale, 0);
      this->_app.Draw(b);
      b2.SetPosition(1680 - scale, 0);
      this->_app.Draw(b2);
      this->Clock.Reset();
    }
  if (timefront < 0.1)
    {
      // if (i == 2)
      // 	{
	  scalefront += 3;
	//   i = 0;
	// }
      if (scalefront >= 1680)
	scalefront = 0;
      ++i;
      front.SetPosition(0 - scalefront, 0);
      this->_app.Draw(front);
      front2.SetPosition(1680 - scalefront, 0);
      this->_app.Draw(front2);
    }
  else
    {
      front.SetPosition(0 - scalefront, 0);
      this->_app.Draw(front);
      front2.SetPosition(1680 - scalefront, 0);
      this->_app.Draw(front2);
      this->_frontClock.Reset();
    }
}

void LibGraphic::StateInGame::drawMap()
{
  static int gpos = 0;
  sf::Sprite &test = this->getSprite("SpriteMap");
  int i;
  float time = this->_mapClock.GetElapsedTime();

  if (time < 0.1)
    {
      gpos -= 1;
      i = 0;
      this->_rMap.recupFromFile("ressources/map/"+ this->_gameLvl + ".map");
      while (i < this->_rMap.size())
	{
	  test.SetPosition(MapX(i) + gpos, 0);
	  test.SetSubRect(sf::IntRect(MapX(this->_rMap[i].up), MapY(0),
				      MapX(this->_rMap[i].up + 1), MapY(1)));
	  this->_app.Draw(test);
	  test.SetPosition(MapX(i) + gpos, 1050 - MapY(1));
	  test.SetSubRect(sf::IntRect(MapX(this->_rMap[i].down), MapY(1),
				      MapX(this->_rMap[i].down + 1), MapY(2)));
	  this->_app.Draw(test);
	  ++i;
	}
    }
  else
    {
      i = 0;
      this->_rMap.recupFromFile("ressources/map/"+ this->_gameLvl + ".map");
      while (i < this->_rMap.size())
	{
	  test.SetPosition(MapX(i) + gpos, 0);
	  test.SetSubRect(sf::IntRect(MapX(this->_rMap[i].up), MapY(0),
				      MapX(this->_rMap[i].up + 1), MapY(1)));
	  this->_app.Draw(test);
	  test.SetPosition(MapX(i) + gpos, 1050 - MapY(1));
	  test.SetSubRect(sf::IntRect(MapX(this->_rMap[i].down), MapY(1),
				      MapX(this->_rMap[i].down + 1), MapY(2)));
	  this->_app.Draw(test);
	  ++i;
	}
      this->_mapClock.Reset();
    }
}

void LibGraphic::StateInGame::draw()
{
  this->drawStarField();
  this->drawMap();
  this->drawText();
  this->drawPlayers();
}

void LibGraphic::StateInGame::drawPlayers()
{
  std::cout << this->_myid << std::endl;
  this->_player->setId(this->_myid);
  this->_player->draw();
  std::map<int, PlayerMovement *>::iterator it;

  for (it = this->_playerMap.begin(); it != this->_playerMap.end(); ++it)
    {
      std::cout << "en plus de moi il y a " << (int)it->second->getId() <<std::endl;
      if (it->second->getId() != this->_myid)
	{
	  std::cout << "je l'affiche en "<< it->second->getX() << " " << it->second->getY() <<std::endl;
	  it->second->draw();
	}
    }
}

void LibGraphic::StateInGame::drawText()
{
}

LibGraphic::Event LibGraphic::StateInGame::gereEvent()
{
  sf::Event Event;

  while (this->_app.GetEvent(Event))
    {
      if (Event.Type == sf::Event::KeyPressed)
	{
	  switch (Event.Key.Code)
	    {
	    case sf::Key::Escape :
	      //return LibGraphic::__EVENT_QUIT;
	      {
		// if (errorToPrint)
		//   errorToPrint = false;
		// else
		//   {
		// this->_app.Close();
		// exit(EXIT_SUCCESS);
		this->_nextState = ROOMLIST;
		return (EVENT_CHANGE_STATE);
		// }
		break;
	      }
	    default : break;
	    }
	}
      if (Event.Type == sf::Event::JoyButtonReleased)
      	{
      	  switch (Event.JoyButton.Button)
      	    {
	    case 0:
      	      {
		break;
      	      }
      	    case 1:
      	      {
		break;
      	      }
      	    default : break;
      	    }
      	}
    }
  this->_player->move(Event);
  return EVENT_NONE;
}

void LibGraphic::StateInGame::readText(const sf::Event & Event)
{
}

LibGraphic::eStates LibGraphic::StateInGame::getNextState()
{
  return this->_nextState;
}

sf::Sprite & LibGraphic::StateInGame::getSprite(std::string const & spriteName) const
{
  return ((*this->_ressourcesSprite.find(spriteName)).second->_sprite);
}

LibGraphic::MyMusic * LibGraphic::StateInGame::getMusic(std::string const & musicName) const
{
  return ((*this->_ressourcesPlayList.find(musicName)).second);
}

sf::Font * LibGraphic::StateInGame::getFont(std::string const & fontName) const
{
  return ((*this->_ressourcesFont.find(fontName)).second);
}

inline sf::String * LibGraphic::StateInGame::getStdToSfString(std::string const & s, sf::Font * daFont)
{
  return (new sf::String(s, *daFont));
}

RecupMap &LibGraphic::StateInGame::getRecupMap(void)
{
  return (this->_rMap);
}

std::string const &LibGraphic::StateInGame::getInGameName(void) const
{
  return (this->_gameName);
}

std::string const &LibGraphic::StateInGame::getGameLvl(void) const
{
  return (this->_gameLvl);
}

void LibGraphic::StateInGame::setGameName(std::string const &s)
{
  this->_gameName = s;
}

void LibGraphic::StateInGame::setGameLvl(std::string const &s)
{
  this->_gameLvl = s;
}

LibGraphic::eMovement LibGraphic::StateInGame::getLastMove() const
{
  return this->_player->getLastMove();
}

std::map<int, LibGraphic::PlayerMovement *> const & LibGraphic::StateInGame::getPlayerMap() const
{
  return this->_playerMap;
}

std::map<int, LibGraphic::PlayerMovement *> & LibGraphic::StateInGame::getPlayerMap()
{
  return this->_playerMap;
}

eShipColor LibGraphic::StateInGame::getMyId() const
{
  return this->_myid;
}

void LibGraphic::StateInGame::setMyId(eShipColor id)
{
  this->_myid = id;
}

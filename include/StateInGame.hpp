#ifndef INGAME_H
# define INGAME_H

#include "Sfml.hpp"
#include "Istate.hpp"
#include "RecupMap.hpp"
#include "PlayerMovement.hpp"
#include "SpriteInfo.hpp"
#include "BulletMovement.hpp"
#include "IAnnim.hpp"

namespace LibGraphic
{
  class AMonsterMovement;
  class StateInGame : public AState
  {

  public:
    StateInGame(std::map<std::string const, GraphicRessource *> const &,
		       std::map<std::string const, MyMusic *> const &,
		       std::map<std::string const, MySound *> const &,
		       std::map<std::string const, sf::Font *> const &,
		       sf::RenderWindow &);
    ~StateInGame();
    virtual bool init();
    virtual void draw();
    virtual Event gereEvent();
    virtual eStates getNextState();

    RecupMap &getRecupMap(void);

    void setGameName(std::string const &s);
    void setGameLvl(std::string const &s);
    std::string const &getInGameName(void) const;
    std::string const &getGameLvl(void) const;

    eMovement getLastMove() const;
    eBulletType getLastBullet() const;

    std::map<int, PlayerMovement *> const & getPlayerMap() const;
    std::map<int, PlayerMovement *> & getPlayerMap();

    std::map<int, AMonsterMovement *> const &getMonsterMap() const;
    std::map<int, AMonsterMovement *> &getMonsterMap();

    std::list<BulletMovement *> &getBulletList();
    std::list<Coord *> &getEnemyCoordBulletList();

    std::list<IAnnim *> &getExplosionList();
    std::list<Coord *> & getCoordBulletList();

    eShipColor getMyId() const;
    void setMyId(eShipColor id);
    void setMyPosition(Coord c);
    void resetInGameState();
    void setScore(unsigned int i);
    void setLives(unsigned int i);
    void drawExplosion();
    void setRunning(bool);

  private:
    void setNextState(eState d);
    void drawBullet();
    void drawMonsters();
    void drawText();
    void drawConv();
    void drawStarField();
    void drawMap();
    void drawPlayers();
    void drawMenu();
    void cursorMenuPos(const sf::Event&);
    void readText(const sf::Event &);
    int MapX(int x);
    int MapY(int y);
    std::string stringOfInt(int);

  private:
    bool	_running;
    int		gpos;
    RecupMap	_rMap;
    eStates _nextState;
    sf::Clock Clock;
    sf::Clock _mapClock;
    sf::Clock _frontClock;
    unsigned int _lives;
    unsigned int _score;
    std::string _gameName;
    std::string _gameLvl;
    bool _mapLoaded;
    // monsters
    // players
    // monsters
    eShipColor _myid;
    std::list<BulletMovement *>	_bulletList;
    std::list<BulletMovement *>	_enemyBulletList;
    std::map<int, PlayerMovement *> _playerMap;
    std::map<int, AMonsterMovement *> _monsterMap;
    std::list<IAnnim *>	_explosionList;
    PlayerMovement * _player;
    BulletMovement * _bulletInst;
    BulletMovement * _enemyBulletInst;
  };

}

#endif

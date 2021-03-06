#ifndef GRAPHICUTILS_H_
# define GRAPHICUTILS_H_

#include <SFML/Graphics.hpp>

namespace LibGraphic
{
  enum eStates
    {
      START,
      ROOMLIST,
      CREATEGAME,
      ROOM,
      OPTIONS,
      CREDITS,
      RANKING,
      INTRO,
      INGAME,
      UNKNOWN_STATE
    };

  enum eMovement
    {
      UP_LEFT,
      UP,
      UP_RIGHT,
      RIGHT,
      DOWN_RIGHT,
      DOWN,
      DOWN_LEFT,
      LEFT,
      NO_MOVE
    };

  enum eBulletType
    {
      NORMAL_BULLET,
      CHARGED_BULLET,
      NO_BULLET
    };

  struct Size
  {
    unsigned int width;
    unsigned int height;
  };

  struct Coord
  {
    int x;
    int y;
  };

  struct GraphicRessource
  {
    sf::Image _image;
    sf::Sprite _sprite;
    bool _isFullScreen;
    Size _dimension;
  };

  struct SpriteCoord
  {
    std::string name;
    Coord coordInFile;
  };

  struct EventUtils
  {
    sf::Event Event;
    sf::Input Input;
  };

  typedef eStates eState;
}

#endif //GRAPHICUTILS_H_

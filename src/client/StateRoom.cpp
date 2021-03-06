#include <iostream>
#include "StateRoom.hpp"
#include "Language.hpp"

extern LibGraphic::Volume gVolume;
extern LibGraphic::Language language;
extern bool errorToPrint;
extern bool helpToShow;
LibGraphic::StateRoom::StateRoom(std::map<std::string const, GraphicRessource *> const & ressourcesSprite,
						   std::map<std::string const, MyMusic *> const & ressourcesPlayList,
						   std::map<std::string const, MySound *> const & ressourcesSounds,
						   std::map<std::string const, sf::Font *> const & ressourcesFont,
						   sf::RenderWindow & app):
  AState(ressourcesSprite, ressourcesPlayList, ressourcesSounds, ressourcesFont, app),
  _chat(""), _conversation(""), _game(NULL)
{
  this->_currentButton = BUTTON_ROOM_CHAT;
  this->_nextState = UNKNOWN_STATE;
  if (language == ENGLISH)
    this->_chat = "Write here to talk !";
  else
    this->_chat = "Ecrire ici pour parler !";
  this->_isDefaultText = true;
}

LibGraphic::StateRoom::~StateRoom()
{
}

bool LibGraphic::StateRoom::init()
{
  return true;
}

void LibGraphic::StateRoom::drawInputButton()
{
  if (this->_currentButton == BUTTON_ROOM_CHAT)
    {
      sf::Sprite &input = this->getSprite("Input");
      input.SetPosition(300, 758);
      this->_app.Draw(input);
    }
}

void LibGraphic::StateRoom::drawPlayerList(void)
{
  std::list<std::string>::iterator it;
  std::string res;
  sf::String *tmp;

  for (it = this->_playerNameList.begin(); it != this->_playerNameList.end(); ++it)
    {
      // std::cout << "log: " << *it << std::endl;
      res = res + *it + "\n";
    }
  tmp = this->getStdToSfString(res, this->getFont("StartFontF"));
  tmp->SetPosition(1100, 328 + 303);
  tmp->SetScale(0.6, 0.6);
  this->_app.Draw(*tmp);
}

void LibGraphic::StateRoom::drawSelectedMap(void)
{
  if (!(this->_game))
    return ;
  if ((this->_game->getMap() != "Sun" &&
       this->_game->getMap() != "Star"))
    return ;
  sf::Sprite &preview = this->getSprite("Preview" + this->_game->getMap());
  sf::String *tmp;

  tmp = this->getStdToSfString(this->_game->getMap(), this->getFont("StartFontF"));
  tmp->SetPosition(1200, 290);
  tmp->SetScale(0.6, 0.6);
  preview.SetPosition(1090, 328);
  preview.SetScale(1, 1);
  this->_app.Draw(preview);
  this->_app.Draw(*tmp);
}

void LibGraphic::StateRoom::draw()
{
  sf::Sprite &back = this->getSprite("StartMenuBackground");
  sf::Sprite &menu_coins = this->getSprite("Room-coin");
  sf::Sprite &menu = this->getSprite("RoomMenu");

  sf::Sprite &Button = this->getSprite("BasicButton");

  menu.SetPosition(245, 210);
  menu_coins.SetPosition(192, 145);

  menu.SetColor(sf::Color(255, 255, 255, 210));

  this->_app.Draw(back);
  this->_app.Draw(menu);
  this->drawSelectedMap();
  this->drawPlayerList();
  this->drawInputButton();
  this->_app.Draw(menu_coins);

  Button.SetPosition(1225, 870);
  this->_app.Draw(Button);

  Button.SetPosition(1435, 870);
  this->_app.Draw(Button);

  this->drawText();
}

void LibGraphic::StateRoom::drawConv()
{
  sf::String *tmp;

  tmp = this->getStdToSfString(this->_conversation, this->getFont("StartFontF"));
  tmp->SetPosition(305, 290);
  tmp->SetScale(0.5, 0.5);
  this->_app.Draw(*tmp);
}

void LibGraphic::StateRoom::drawText()
{
  sf::String *tmp;

  if (language == ENGLISH)
    {
      tmp = this->getStdToSfString("Leave", this->getFont("StartFontF"));
      tmp->SetPosition(1077, 838);
    }
  else
    {
      tmp = this->getStdToSfString("Quitter", this->getFont("StartFontF"));
      tmp->SetPosition(1070, 838);
    }
  tmp->SetScale(0.6, 0.6);
  tmp->SetColor(sf::Color(255,255,0, 255));
  if (this->_currentButton != BUTTON_ROOM_QUIT)
    tmp->SetColor(sf::Color(255,255,255, 205));
  this->_app.Draw(*tmp);

  if (language == ENGLISH)
    {
      tmp = this->getStdToSfString("Start", this->getFont("StartFontF"));
      tmp->SetPosition(1287, 838);
    }
  else
    {
      tmp = this->getStdToSfString("Lancer", this->getFont("StartFontF"));
      tmp->SetPosition(1280, 838);
    }
  tmp->SetScale(0.6, 0.6);
  tmp->SetColor(sf::Color(255,255,0, 255));
  if (this->_currentButton != BUTTON_ROOM_START)
    tmp->SetColor(sf::Color(255,255,255, 205));
  this->_app.Draw(*tmp);

  tmp = this->getStdToSfString(this->_chat, this->getFont("StartFontF"));
  tmp->SetText(this->_chat);
  tmp->SetPosition(310, 770);
  tmp->SetScale(0.5, 0.5);
  tmp->SetColor(sf::Color(0, 0, 0, 255));
  if (this->_currentButton != BUTTON_ROOM_CHAT)
    tmp->SetColor(sf::Color(255, 255, 255, 155));
  this->_app.Draw(*tmp);
  this->drawConv();
}

LibGraphic::Event LibGraphic::StateRoom::gereEvent()
{
  sf::Event Event;

  while (this->_app.GetEvent(Event))
    {
      if (Event.Type == sf::Event::KeyPressed)
	{
	  switch (Event.Key.Code)
	    {
	    case sf::Key::Escape :
	      {
		if (errorToPrint)
		  errorToPrint = false;
		else if (this->_currentButton == BUTTON_ROOM_QUIT)
		  {
		    this->playSound();
		    this->_nextState = ROOMLIST;
		    return EVENT_CHANGE_STATE;
		  }
		this->_currentButton = BUTTON_ROOM_QUIT;
		break;
	      }
	    // case sf::Key::H :
	    //   {
	    // 	if (helpToShow)
	    // 	  helpToShow = false;
	    // 	else
	    // 	  helpToShow = true;
	    // 	break;
	    //   }
	    case sf::Key::Return :
	      {
		if (this->_currentButton == BUTTON_ROOM_QUIT)
		  {
		    // this->_nextState = ROOMLIST;
		    this->playSound();
		    return (EVENT_ROOM_QUIT);
		  }
		else if (this->_currentButton == BUTTON_ROOM_CHAT)
		  {
		    return (EVENT_ROOM_CHAT);
		  }
		else if (this->_currentButton == BUTTON_ROOM_START)
		  {
		    this->playSound();
		    return (EVENT_ROOM_START);
		  }
		break;
	      }
	    case sf::Key::Back :
	      {
		if (this->_currentButton == BUTTON_ROOM_CHAT)
		  this->_chat = this->_chat.substr(0, this->_chat.length() - 1);
		break;
	      }
	    default : break;
	    }
	}
      if (Event.Type == sf::Event::TextEntered &&
	  (Event.Text.Unicode > 20 && Event.Text.Unicode < 128))
	readText(Event);
      cursorMenuPos(Event);
      if (Event.Type == sf::Event::JoyButtonReleased)
	{
	  switch (Event.JoyButton.Button)
	    {
	    case 0:
	      {
		if (this->_currentButton == BUTTON_ROOM_QUIT)
		  {
		    this->playSound();
		    this->_nextState = ROOMLIST;
		    return EVENT_ROOM_QUIT;
		  }
		else if (this->_currentButton == BUTTON_ROOM_CHAT)
		  {
		    return (EVENT_ROOM_CHAT);
		  }
		else if (this->_currentButton == BUTTON_ROOM_START)
		  {
		    this->playSound();
		    return (EVENT_ROOM_START);
		  }
		break;
	      }
	    case 1:
	      {
		if (this->_currentButton == BUTTON_ROOM_QUIT)
		  {
		    this->playSound();
		    this->_nextState = ROOMLIST;
		    return EVENT_CHANGE_STATE;
		  }
		this->_currentButton = BUTTON_ROOM_QUIT;
		break;
	      }
	    default : break;
	    }
	}
    }
  return EVENT_NONE;
}

void LibGraphic::StateRoom::readText(const sf::Event & Event)
{
  if (this->_isDefaultText)
    {
      this->_isDefaultText = !this->_isDefaultText;
      this->_chat = "";
    }
  if (this->_currentButton == BUTTON_ROOM_CHAT && this->_chat.size() <= 25)
    this->_chat += static_cast<char>(Event.Text.Unicode);
}

LibGraphic::eStates LibGraphic::StateRoom::getNextState()
{
  return this->_nextState;
}

void LibGraphic::StateRoom::cursorMenuPos(const sf::Event & Event)
{
  const sf::Input & Input = this->_app.GetInput();
  float JoystickPOV = Input.GetJoystickAxis(0, sf::Joy::AxisPOV);

  if ((JoystickPOV == -1 && Event.Type != sf::Event::KeyPressed) ||
      this->Clock.GetElapsedTime() < 0.1)
    return;
  switch (this->_currentButton)
    {
    case BUTTON_ROOM_CHAT :
      {
	if ((JoystickPOV > 135 && JoystickPOV < 225) ||
	    Event.Key.Code == sf::Key::Down || Event.Key.Code == sf::Key::Tab)
	  this->_currentButton = BUTTON_ROOM_QUIT;
	break;
      }
    case BUTTON_ROOM_QUIT :
      {
	if ((JoystickPOV > 315 || (JoystickPOV < 45 && JoystickPOV != -1)) ||
	    Event.Key.Code == sf::Key::Up)
	  this->_currentButton = BUTTON_ROOM_CHAT;
	else if ((JoystickPOV > 45 && JoystickPOV < 135) ||
		 Event.Key.Code == sf::Key::Right || Event.Key.Code == sf::Key::Tab)
	  this->_currentButton = BUTTON_ROOM_START;
	break;
      }
    case BUTTON_ROOM_START :
      {
	if ((JoystickPOV > 315 || (JoystickPOV < 45 && JoystickPOV != -1)) ||
	    Event.Key.Code == sf::Key::Up)
	  this->_currentButton = BUTTON_ROOM_CHAT;
	else if ((JoystickPOV > 225 && JoystickPOV < 315) ||
		 Event.Key.Code == sf::Key::Left || Event.Key.Code == sf::Key::Tab)
	  this->_currentButton = BUTTON_ROOM_QUIT;
	break;
      }
    default : break;
    }
  this->Clock.Reset();
}

std::string const & LibGraphic::StateRoom::getMessage() const
{
  return this->_chat;
}

void LibGraphic::StateRoom::setMessage(std::string const &s)
{
  this->_chat = s;
}

std::string const & LibGraphic::StateRoom::getConversation() const
{
  return this->_conversation;
}

void LibGraphic::StateRoom::addToConversation(std::string const & add)
{
  this->_conversation += add;
}

void LibGraphic::StateRoom::setGameForPreview(InfoGame *g)
{
  this->_game = g;
}

std::list<std::string> &LibGraphic::StateRoom::getPlayerNameList(void)
{
  return (this->_playerNameList);
}

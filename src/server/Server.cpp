#include "Server.hpp"
#include "Selector.hpp"
#include "PacketManager.hpp"
#ifndef _WIN32
# include "TCPServerSocketUnix.h"
# include "UDPServerSocketUnix.h"
#else
# include "TCPServerSocketWindows.h"
# include "UDPServerSocketWindows.h"
#endif

Server&	Server::operator=(Server const &){return *this;}
Server::Server(Server const &){}


Server::Server(void)
{
#ifndef _WIN32
  this->_selector = new Selector<int>;
  this->_udp = new UDPServerSocketUnix(this->_selector);
  this->_listener = new TCPServerSocketUnix(this->_selector);
#else
  this->_selector = new Selector<SOCKET>;
  this->_udp = new UDPServerSocketWindows(this->_selector);
  this->_listener = new TCPServerSocketWindows(this->_selector);
#endif
}

Server::~Server(void)
{
}

bool Server::init(int port)
{
  this->_port = port;
  if (!(this->_listener->SNCreate(SERV_ADDR, port)))
    {
      std::cerr << "Error: Can't Create Listener Socket on port " << this->_port << std::endl;
      return (false);
    }
  if (!(this->_udp->SNCreate(SERV_ADDR, port)))
    {
      std::cerr << "Error: Can't Create Udp Socket on port " << this->_port << std::endl;
      return (false);
    }
  std::cout << "Server Listening on port " << this->_port << std::endl;
 if (!this->_listener->SNListen())
    {
      std::cerr << "Error: Can't listen on socket" << std::endl;
    }
  this->_listener->SNAddRead();
  this->_udp->SNAddRead();
  return (true);
}

bool	Server::addClient(ATCPClientSocket *sock)
{
  User	*user;

  if (sock->getIp())
    {
      user = new User(sock, sock->getIp(), this->_pm);
      if (this->_userMap.find(sock->getIp()) == this->_userMap.end())
	{
	  std::cout << "Info: User "<< sock->getIp() << " added to list"  << std::endl;
	  this->_userMap[sock->getIp()] = user;
	  return (true);
	}
      std::cout << "User " << sock->getIp() << " is already connected" << std::endl;
      sock->SNClose();
      delete user;
      return (false);
    }
  std::cerr << "Error: Can't resolve client ip adress" << std::endl;
  return (false);
}

bool Server::getNewClient(void)
{
  if (this->_listener->SNGetRead())
    {
      ATCPClientSocket *newClient = NULL;
      if ((newClient = this->_listener->SNAccept()) == false)
  	  return (false);
      newClient->SNAddRead();
      newClient->SNAddWrite();
      std::cout << "New connection";
      if (newClient->getIp())
	{
	  std::cout << " from " << newClient->getIp();
	}
      std::cout << std::endl;
      this->addClient(newClient);
    }
  return (true);
}

bool Server::removeClient(User *user, ATCPClientSocket *socket)
{
  if (socket && user)
    {
      std::cout << "Info: User " <<
	socket->getIp() << " disconnected" <<
	std::endl;
      user->setSafe(false);
      if (socket->getIp())
	this->_quitQueue.push(socket->getIp());
      socket->SNDelRead();
      socket->SNDelWrite();
      socket->SNClose();
      return (true);
    }
  return (false);
}

bool Server::readFromClients(void)
{
  std::map<std::string, User *>::iterator	it;
  User				*user = NULL;
  ATCPClientSocket		*socket = NULL;

  for (it = this->_userMap.begin(); it != this->_userMap.end(); ++it)
    {
      user = it->second;
      if (user && user->isSafe() && (socket = user->getSocketTCP()) != NULL)
	{
	  if (socket->SNGetRead() == true)
	    {
	      if (user->feedPacketAggregator() == false)
		{
		  this->removeClient(user, socket);
		}
	    }
	}
    }
  if (this->_udp->SNGetRead())
    {
      int size;
      char msg[1024];
      std::string ip;

      if ((size = this->_udp->SNReadClient(msg, 1024, ip))) //todo check size
  	{
  	  if (this->_userMap.find(ip) != this->_userMap.end())
  	    {
	      this->_userMap[ip]->feedPacketAggregator(msg, size);
  	    }
  	}
    }
  return (true);
}

bool	Server::cleanClients(void)
{
  std::string ip;

  if (this->_quitQueue.empty())
    return (true);
  while (!this->_quitQueue.empty())
    {
      ip = this->_quitQueue.front();
      if (this->_userMap.find(ip) != this->_userMap.end())
	{
	  delete this->_userMap[this->_quitQueue.front()];
	  this->_userMap.erase(ip);
	}
      this->_quitQueue.pop();
    }
  return (true);
}

bool	Server::writeToClients(void)
{
  std::map<std::string, User *>::iterator	it;
  User *user = NULL;

  for (it = this->_userMap.begin(); it != this->_userMap.end(); ++it)
    {
      if ((user = it->second) == NULL)
	continue;
      user->aggregatePacketToSend();
    }
  return (true);
}

bool	Server::processPackets(void)
{
  std::map<std::string, User *>::iterator	it;
  User *user = NULL;

  for (it = this->_userMap.begin(); it != this->_userMap.end(); ++it)
    {
      if ((user = it->second) == NULL)
	continue;
      user->processPackets();
    }
  return (true);
}

bool Server::run(void)
{
  while (true)
    {
      this->_listener->SNAddRead();
      if (!this->_selector->SNSelect())
	{
	  std::cerr << "Error: Select" << std::endl;
	  return (false);
	}
      this->getNewClient();
      this->readFromClients();
      this->processPackets();
      this->writeToClients();
      this->cleanClients();
    }
}

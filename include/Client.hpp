#ifndef CLIENT_H_
# define CLIENT_H_

#include "Sfml.hpp"
#include "ClientNetwork.hpp"

class	Client
{
private:
  Client(const Client&);
  Client&	operator=(const Client&);
public:
  Client(void);
  ~Client(void);
  Client(std::string const &ip, int port);
  bool init(void);
  bool run(void);
  bool gereGraphicAction(void);
  bool gereNetworkAction(void);
  ClientNetwork &getNetwork(void);
private:
  ClientNetwork		cNetwork;
  LibGraphic::Sfml	cGraphic;
};

#endif	// CLIENT_H_

#include <iostream>
#include "Client.hpp"
#include "Protocol.h"
#include "PacketData.hpp"
#include "PacketFactory.hpp"

Client::Client(void) : cNetwork("", 0), cGraphic(cNetwork)
{}

Client::~Client(void)
{}

Client::Client(const Client&other) :
  cNetwork(other.cNetwork), cGraphic(cNetwork)
{}

Client&	Client::operator=(const Client&)
{
  return *this;
}

Client::Client(std::string const &ip, int port) :
  cNetwork(ip, port), cGraphic(cNetwork)
{
}

bool Client::init(void)
{
  if (!this->cGraphic.init())
    {
      std::cerr << "Error: Can't initialize sfml" << std::endl;
      return (false);
    }
  return (true);
}

bool Client::run(void)
{
  int i;

  i = 0;
  if (!this->cNetwork.connect("127.0.0.1", 12348))// a faire ds le play
    {
      //et pas en dur !
      return (false);
    }
  while (true)
    {
      if (!this->cNetwork.select())
      	{
      	  std::cerr << "Error: Can't Monitor Sockets" << std::endl;
      	  return (false);
      	}
      this->cNetwork.feedPacketAggregatorTCP();
      this->cNetwork.feedPacketAggregatorUDP();
      this->cGraphic.getEvent();
      this->cGraphic.clean();
      this->cGraphic.draw();
      if (i == 0)
      	{
      	  PacketData data;
      	  PacketData data1;
      	  PacketData data2;
      	  PacketData data3;

	  data.addShort(256);
	  data.addShort(1);

      	  data1.addString("toto", 4);
      	  data1.addString("mon message", 11);

      	  data2.addChar('c');
	  data2.addShort(42);
	  data2.addString("toto");
	  data2.addShort(4);
	  data2.addUint32(1);

      	  data3.addString("prout");
      	  data3.addShort(400);
      	  ProtocolPacket *protocolPacket = PacketFactory::createPacket(THE_GAME, static_cast<ushort>(QUITGAME), data);
      	  ProtocolPacket *protocolPacket1 = PacketFactory::createPacket(LOBBY, static_cast<ushort>(CHAT), data1);
      	  ProtocolPacket *protocolPacket2 = PacketFactory::createPacket(MOVEMENT, static_cast<ushort>(NEWBULLET), data2);
      	  ProtocolPacket *protocolPacket3 = PacketFactory::createPacket(GAME_DETAILS, static_cast<ushort>(SCORE), data3);

      	  this->cNetwork.pushTCP(protocolPacket);
      	  this->cNetwork.pushTCP(protocolPacket1);
      	  this->cNetwork.pushTCP(protocolPacket2);
      	  this->cNetwork.pushTCP(protocolPacket3);

      	  this->cNetwork.pushUDP(protocolPacket);
      	  this->cNetwork.pushUDP(protocolPacket1);
      	  this->cNetwork.pushUDP(protocolPacket2);
      	  this->cNetwork.pushUDP(protocolPacket3);
      	  ++i;
      	}
      this->cNetwork.process();
      this->cNetwork.sendPacketToServer(); // static ok?
    }
  return (true);
}

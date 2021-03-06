#ifndef PROTOCOLLOBBY_H_
# define PROTOCOLLOBBY_H_

# include	<map>
# include	"IClientProtocolAction.hpp"
# include	"PacketData.hpp"
# include	"eProtocolPacketLobby.hpp"

class		ProtocolLobby : public IClientProtocolAction
{
private:
  typedef bool (ProtocolLobby::*ptr_functlobby)(PacketData &, Client &);
  std::map<eProtocolPacketLobby, ptr_functlobby>	actionmap;

public:

  ProtocolLobby(void);
  ~ProtocolLobby(void);
  ProtocolLobby(const ProtocolLobby&);
  ProtocolLobby&	operator=(const ProtocolLobby&);
  virtual bool		action(ushort instruction, PacketData & data,Client&);
private:

  bool		actionError(PacketData &data, Client &);
  bool		actionChat(PacketData &data, Client &);
  bool		actionPlayers(PacketData &data, Client&);
};

#endif	// PROTOCOLLOBBY_H_


#ifndef			PACKETMANAGER_H_
# define		PACKETMANAGER_H_

# include		<map>
# include		"user/User.hpp"
# include		"PacketFactory.hpp"
# include		"ProtocolPacketData.hpp"
# include		"iProtocolAction.hpp"

class			PacketManager
{
/* ************************************************************************* */
/*                             Attributes                                    */
/* ************************************************************************* */
 private:
  std::map<eProtocolPacketGroup, iProtocolAction *>	groupaction;

/* ************************************************************************* */
/*                             Coplien Form                                  */
/* ************************************************************************* */
 public:
  PacketManager(void);
  PacketManager(PacketManager const &);
  PacketManager &		operator=(PacketManager const &);
  ~PacketManager(void);

/* ************************************************************************* */
/*                             Member Functions                              */
/* ************************************************************************* */
 public:
  bool				send(User * user, eProtocolPacketGroup group,
 				     ushort instruction,
				     ProtocolPacketData & data);

  // bool				rcsv(UserNetwork * socket);
  // bool				rcsv(User * user);

/* ************************************************************************* */
/*                             Private Functions                             */
/* ************************************************************************* */
 // private:
 //  void				actionError();
 //  ProtocolPacket *		RcvPacket(UserNetwork * net,
 // 					  User * user = NULL);
};

#endif				// !PACKETMANAGER_H_


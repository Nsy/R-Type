#ifndef __EPROTOCOLPACKET_GAME_DETAILS_HPP__
#define __EPROTOCOLPACKET_GAME_DETAILS_HPP__

enum		eProtocolPacketGameDetails
  {
    GAMEDATAIL_ERROR	= 99,
    PLAYERLOGIN		= 0,
    SCORE		= 1,
    GETMAP		= 2,
    PLAYERLIFE		= 3,
    GAME_DETAILS_MAX	= 4
  };

#endif // __EPROTOCOLPACKET_GAME_DETAILS_HPP__

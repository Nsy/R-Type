#ifndef EGRAPHICACTION_H
# define EGRAPHICACTION_H

enum eGraphicAction
  {
    START_PLAY,
    ROOMLIST_REFRESH,
    ROOMLIST_JOIN,
    ROOMLIST_SPECTATE,
    CREATE_CREATE,
    ROOM_CHAT,
    UNKNOWN
  };

enum eNetworkAction
  {
    JOIN_SUCCESS,
    START_GAME_SUCESS,
    GET_GAME_SUCESS,
    CREATE_SUCESS,
    UNKNOWN_NET
  };

#endif //EGRAPHICACTION_H

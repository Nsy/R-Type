#ifndef			PROTOCOLPACKETDATA_H_
# define		PROTOCOLPACKETDATA_H_

# include		<vector>
# include		<algorithm>
# include		<string>
# include		"types.h"

class			PacketData
{
/* ************************************************************************* */
/*                             Attributes                                    */
/* ************************************************************************* */
 private:
  std::vector<DataRawType>	data;
  uint				it;

/* ************************************************************************* */
/*                             Coplien Form                                  */
/* ************************************************************************* */
 public:
  PacketData(void);
  PacketData(DataRawType const * data, ushort size);
  PacketData(PacketData const &);
  PacketData &		operator=(PacketData const &);
  ~PacketData(void);

/* ************************************************************************* */
/*                             Member Functions                              */
/* ************************************************************************* */
 public:
  void			prettyPrint(void) const;

 public:
  DataRawType const *		getData(void) const;
  ushort		getDataSize(void) const;

 public:
  std::string		getNextString(void);
  char			getNextChar(void);
  uchar			getNextUChar(void);
  short			getNextShort(void);
  ushort		getNextUShort(void);
  unsigned int  getNextUint32(void);
  template <typename T>
  T			getData(void);

 public:
  void			addString(char const *, ushort);
  void			addString(std::string const &);
  void			addChar(char);
  void			addUChar(uchar);
  void			addShort(short);
  void			addUShort(ushort);
  void			addUint32(short);
  template <typename T>
  void			addData(T d);
};

void			uglyPrinter(char *data, int size);

#endif			// !PROTOCOLPACKETDATA_H_

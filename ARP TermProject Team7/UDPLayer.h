#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "RouterTable.h"

class CUDPLayer 
: public CBaseLayer  
{
private:
	unsigned char destip[4];
	RouterTable* table;
public:
	CUDPLayer(char* pName);
	virtual ~CUDPLayer();
	void ResetHeader();

	void	Init(RouterTable* table) { this->table = table; }
	BOOL	Send(unsigned char* ppayload, int nlength);
	BOOL	Receive(unsigned char* ppayload);

	typedef struct _UDPLayer_HEADER{
		unsigned short srcPort;
		unsigned short destPort;
		unsigned short length;
		unsigned short checksum;
	}UDPLayer_HEADER, *PUDPLayer_HEADER ;

protected:
	UDPLayer_HEADER	m_sHeader;
};

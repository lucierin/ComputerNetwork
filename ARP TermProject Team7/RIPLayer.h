#pragma once
#include "BaseLayer.h"
#include "RouterTable.h"

class CRIPLayer 
: public CBaseLayer
{
private:	
	RouterTable* table;
public:
	CRIPLayer(char* pName);
	virtual ~CRIPLayer();
	void ResetHeader();
	
	void	Init(RouterTable* table) { this->table = table; }
	BOOL	Send(unsigned char* ppayload, int nlength);
	BOOL	Receive(unsigned char* ppayload);

	typedef  struct _RIPHeader
{
	uchar command;
	uchar version;
	ushort unused;
	uchar address_family[2];
	ushort route_tag;
	uchar ip[4];
	uchar subnetmask[4];
	uchar nextIP[4];
	uint metric;
}RIPLayer_HEADER, *PRIPLayer_HEADER ;

protected:
	RIPLayer_HEADER	m_sHeader;
};


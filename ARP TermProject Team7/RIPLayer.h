#pragma once
#include "BaseLayer.h"
#include "RouterTable.h"
struct RIPHeader
{
	uchar command;
	uchar version;
	ushort unused;
	uchar address_family[2];
	uchar route_tag[2];
	uchar ip[4];
	uchar subnetmask[4];
	uchar nextip[4];
	int metric;
};
class CRIPLayer : public CBaseLayer
{
private:
	RouterTable* table;
public:
	CRIPLayer(char* pName);
	~CRIPLayer(void);

	void Init(RouterTable* table) { this->table = table; }
	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);
};


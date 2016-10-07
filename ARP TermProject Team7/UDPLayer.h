#pragma once
#include "BaseLayer.h"
#include "RouterTable.h"
struct UDPHeader
{
	ushort src;
	ushort dest;
	ushort length;
	ushort checksum;
};
class CUDPLayer : public CBaseLayer
{
private:
	RouterTable* table;
public:
	CUDPLayer(char* pName);
	~CUDPLayer(void);

	void Init(RouterTable* table) { this->table = table; }
	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);
};


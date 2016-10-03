#pragma once
#include "BaseLayer.h"

typedef unsigned char uchar;//1byte
typedef unsigned short ushort; //2byte
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
class RIPLayer : public CBaseLayer
{
public:
	RIPLayer(void);
	~RIPLayer(void);
};


#pragma once
#include "BaseLayer.h"
struct UDPHeader
{
	ushort src;
	ushort dest;
	ushort length;
	ushort checksum;
};
class UDPLayer 
{
public:
	UDPLayer(void);
	~UDPLayer(void);
	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);
};


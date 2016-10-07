#include "StdAfx.h"
#include "UDPLayer.h"


CUDPLayer::CUDPLayer(char* pName) : CBaseLayer(pName)
{
}


CUDPLayer::~CUDPLayer(void)
{
}

BOOL CUDPLayer::Send(unsigned char* ppayload, int nlength)
{
	return false;
}

BOOL CUDPLayer::Receive(unsigned char* ppayload)
{
	return false;
}
#include "StdAfx.h"
#include "RIPLayer.h"


CRIPLayer::CRIPLayer(char* pName) : CBaseLayer(pName)
{
}


CRIPLayer::~CRIPLayer(void)
{
}

BOOL CRIPLayer::Send(unsigned char* ppayload, int nlength)
{
	return false;
}

BOOL CRIPLayer::Receive(unsigned char* ppayload)
{
	return false;
}
#include "StdAfx.h"
#include "UDPLayer.h"


CUDPLayer::CUDPLayer(char* pName)
{
	ResetHeader( );
}


CUDPLayer::~CUDPLayer(void)
{
}
void CUDPLayer::ResetHeader()
{
	m_sHeader.srcPort =0;
	m_sHeader.destPort =0;
	m_sHeader.length =0;
	m_sHeader.checksum =0;
}
BOOL CUDPLayer::Send(unsigned char* ppayload, int nlength)
{
	
	
	BOOL bSuccess = FALSE ;
	bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader,20 + nlength);

	return bSuccess;
}

BOOL CUDPLayer::Receive(unsigned char* ppayload)
{
	return false;
}
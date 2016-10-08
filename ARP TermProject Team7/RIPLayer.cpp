#include "StdAfx.h"
#include "RIPLayer.h"


CRIPLayer::CRIPLayer(char* pName)
{
	ResetHeader( );
}


CRIPLayer::~CRIPLayer(void)
{
}
void CRIPLayer::ResetHeader()
{
	m_sHeader.command =0;
	m_sHeader.version =0;
	m_sHeader.unused =0;
	memcpy(m_sHeader.address_family,0,2);
	m_sHeader.route_tag = 0;
	memcpy(m_sHeader.ip,0,2);
	memcpy(m_sHeader.subnetmask,0,2);
	memcpy(m_sHeader.nextIP,0,2);
	m_sHeader.metric = 0;
}
BOOL CRIPLayer::Send(unsigned char* ppayload, int nlength)
{
	
	
	BOOL bSuccess = FALSE ;
	bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader,20 + nlength);

	return bSuccess;
}

BOOL CRIPLayer::Receive(unsigned char* ppayload)
{
	return false;
}
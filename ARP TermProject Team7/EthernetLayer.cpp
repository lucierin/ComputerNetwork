// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARP TermProject Team7.h"
#include "EthernetLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEthernetLayer::CEthernetLayer( char* pName )
: CBaseLayer( pName )
{
	ResetHeader( ) ;
}

CEthernetLayer::~CEthernetLayer()
{
}

void CEthernetLayer::ResetHeader()
{
	memset( m_sHeader.enet_dstaddr, 0, 6 ) ;
	memset( m_sHeader.enet_srcaddr, 0, 6 ) ;
	memset( m_sHeader.enet_data, 0, MAX_ETHERNET_DATA ) ;
	m_sHeader.enet_type = 0x0208 ; // 0x0800
}

unsigned char* CEthernetLayer::GetEnetDstAddress() 
{
	return m_sHeader.enet_srcaddr;
}

unsigned char* CEthernetLayer::GetEnetSrcAddress()
{
	return m_sHeader.enet_dstaddr;
}

void CEthernetLayer::SetEnetSrcAddress(unsigned char *pAddress)
{
	memcpy( &m_sHeader.enet_srcaddr, pAddress, 6 ) ;
}

void CEthernetLayer::SetEnetDstAddress(unsigned char *pAddress)
{

	memcpy( &m_sHeader.enet_dstaddr, pAddress, 6 ) ;
}
void CEthernetLayer::SetType(unsigned short op)
{
	m_sHeader.enet_type = op;
}
void CEthernetLayer::setCount(int count) {
	thisCount = count;
}

BOOL CEthernetLayer::Send(unsigned char *ppayload, int nlength)
{
	memset(m_sHeader.enet_data, 0, MAX_ETHERNET_DATA);
	memcpy( m_sHeader.enet_data, ppayload, nlength );

	BOOL bSuccess = FALSE ;
	bSuccess = mp_UnderLayer[0]->Send((unsigned char*) &m_sHeader,nlength + 14);

	return bSuccess ;
}
	
BOOL CEthernetLayer::Receive( unsigned char* ppayload )
{
	PETHERNET_HEADER pFrame = (PETHERNET_HEADER) ppayload ;

	BOOL bSuccess = FALSE ;
	unsigned char zero[6];
	zero[0] = 0xff; zero[1] = 0xff;
	zero[2] = 0xff; zero[3] = 0xff;
	zero[4] = 0xff; zero[5] = 0xff;

	if((memcmp((char *)pFrame->enet_dstaddr,(char *)m_sHeader.enet_srcaddr,6)==0
		|| memcmp((char *)pFrame->enet_dstaddr, zero, 6) == 0) //broadcast
	&& memcmp((char *)pFrame->enet_srcaddr,(char *)m_sHeader.enet_srcaddr,6)!=0) //mac주소가 같다는 건 자기자신이라는 뜻이니 pass하자
	{
		if(pFrame->enet_type == 0x0008){
			bSuccess = mp_aUpperLayer[1]->Receive((unsigned char*) pFrame->enet_data); //IP
		}
		else if (pFrame->enet_type == 0x0608) {
			bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*) pFrame->enet_data); //ARP
		}
	}
	return bSuccess ;
}

// IPLayer.cpp: implementation of the CIPLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARP TermProject Team7.h"
#include "IPLayer.h"
#include "ARP TermProject Team7Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIPLayer::CIPLayer( char* pName )
: CBaseLayer( pName )
{
	ResetHeader( );
	sem = CreateSemaphore(0, 1, 1, 0);//modified
}

CIPLayer::~CIPLayer()
{
}

void CIPLayer::ResetHeader()
{
	m_sHeader.ip_verlen = 0x00;
	m_sHeader.ip_tos = 0x00;
	m_sHeader.ip_len = 0x0000;
	m_sHeader.ip_id = 0x0000;
	m_sHeader.ip_fragoff = 0x0000;
	m_sHeader.ip_ttl = 0x00;
	m_sHeader.ip_proto = 0x00;
	m_sHeader.ip_cksum = 0x00;
	memset( m_sHeader.ip_src, 0, 4);
	memset( m_sHeader.ip_dst, 0, 4);
	memset( m_sHeader.ip_data, 0, IP_DATA_SIZE);
}

void CIPLayer::SetSrcIPAddress(unsigned char* src_ip1, unsigned char* src_ip2)
{
	memcpy( srcip[0], src_ip1, 4);
	memcpy( srcip[1], src_ip2, 4);
}

void CIPLayer::SetDstIPAddress(unsigned char* dst_ip)
{
	memcpy( m_sHeader.ip_dst, dst_ip, 4);
}

void CIPLayer::SetFragOff(unsigned short fragoff)
{
	m_sHeader.ip_fragoff = fragoff;
}

BOOL CIPLayer::Send(unsigned char* ppayload, int nlength)
{
	memcpy( m_sHeader.ip_data, ppayload, nlength ) ;
	
	BOOL bSuccess = FALSE ;
	bSuccess = mp_UnderLayer[1]->Send((unsigned char*)&m_sHeader,20 + nlength);

	return bSuccess;
}

unsigned char* CIPLayer::GetSrcIPAddress()
{
	return m_sHeader.ip_src;
}

unsigned char* CIPLayer::GetDstIPAddress()
{
	return m_sHeader.ip_dst;

}


BOOL CIPLayer::Receive(unsigned char* ppayload)
{
	WaitForSingleObject(sem, 0L);//modified
	PIPLayer_HEADER pFrame = (PIPLayer_HEADER) ppayload ;
	

	BOOL bSuccess = FALSE ;
	
	unsigned char* ip_target;
	ip_target = (unsigned char*)malloc(sizeof(unsigned char) * 4);
	int index = table->get(pFrame->ip_dst, ip_target);
	
	/////table에 경로가 없으면 패킷을 버림/////
	if (index == -1) return bSuccess;
	
	
	//////nlength를 구하여 다시 보낼수 있게한다./////
	unsigned char len[2];
	unsigned short temp =  pFrame->ip_len;
	memcpy(len, (unsigned char*)&temp, 2);
	int plen = ((int)len[0]<<8) + (int)len[1];
		
	
	///////ttl값을 -1하고 ip_cksum을 조정한다/////
	if(pFrame->ip_ttl == 0 ) return bSuccess; /// ttl이 0이면 패킷을 버린다.
	pFrame->ip_ttl -= (unsigned char)1;
	int sum = pFrame->ip_cksum;
	sum = sum + 1;
	pFrame->ip_cksum+=1;
	if (sum > 0xffff) pFrame->ip_cksum += 1;
	
		
	if ((memcmp(ip_target, srcip[0], 4) == 0) || (memcmp(ip_target, srcip[0], 4) == 0)) return bSuccess;

	((CARPLayer*)mp_UnderLayer[index])->SetIPTargetAddress(ip_target);
	bSuccess = mp_UnderLayer[index]->Send((unsigned char*)pFrame, plen);
	
	ReleaseSemaphore(sem, 1, 0);//modified
	return bSuccess ;
}

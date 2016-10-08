// IPLayer.cpp: implementation of the CIPLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARPLayer.h"
#include "ARPTable.h"
#include "EthernetLayer.h"
#include "ARP TermProject Team7Dlg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARPLayer::CARPLayer(char* pName)
	: CBaseLayer(pName)
{
	ResetHeader();

}

CARPLayer::~CARPLayer()
{
}
void CARPLayer::init(ARPTable* arpTable) {
	this->arpTable = arpTable;
}

void CARPLayer::ResetHeader()
{
	
	m_sHeader.hard_type = 0x0100;
	m_sHeader.prot_type = 0x0008;
	m_sHeader.hard_size = 6;
	m_sHeader.prot_size = 4;
	m_sHeader.op		= 0x0100;
	memset(m_sHeader.enet_sender_addr, 0, 6);
	memset(m_sHeader.ip_sender_addr, 0, 4);
	memset(m_sHeader.enet_target_addr, 0, 6);
	memset(m_sHeader.ip_target_addr, 0, 4);

}
void CARPLayer::setCount(int count) {
	thisCount = count;
}
void CARPLayer::SetEnetSenderAddress(unsigned char* pAddress)
{
	memcpy(m_sHeader.enet_sender_addr, pAddress, 6);
}

void CARPLayer::SetIPSenderAddress(unsigned char* pAddress)
{
	memcpy(m_sHeader.ip_sender_addr, pAddress, 4);
}

void CARPLayer::SetEnetTargetAddress(unsigned char* pAddress)
{
	memcpy(m_sHeader.enet_target_addr, pAddress, 6);
}

void CARPLayer::SetIPTargetAddress(unsigned char* pAddress)
{
	memcpy(m_sHeader.ip_target_addr, pAddress, 4);
}

void CARPLayer::SetOP(unsigned short op) {
	m_sHeader.op = op;
}

BOOL CARPLayer::Send(unsigned char* ppayload, int nlength)
{
	BOOL bSuccess = FALSE;

	tempData = (unsigned char*)malloc(sizeof(unsigned char) * nlength);//////////////ARP가 실행후 보내기 위한 데이터 임시 저장장소
	memcpy(tempData, ppayload, nlength);
	tempLength = nlength;
	
	//////////////////ARP Table 검사. ARP에 IP값이 존재하면 ethernet주소 리턴, 없으면 0으로 채운 ethernet주소//////////////////
	memcpy(m_sHeader.enet_target_addr, arpTable->find(m_sHeader.ip_target_addr), 6);
	unsigned char zero[6];
	zero[0] = 0x00; zero[1] = 0x00;
	zero[2] = 0x00; zero[3] = 0x00;
	zero[4] = 0x00; zero[5] = 0x00;

	if (memcmp((char *)m_sHeader.enet_target_addr, zero, 6) == 0)
	{
		//////////////////ip가 없으면 ARP 헤더만 보냄///////////////////
		zero[0] = 0xff; zero[1] = 0xff;
		zero[2] = 0xff; zero[3] = 0xff;
		zero[4] = 0xff; zero[5] = 0xff;

		m_sHeader.op = 0x0100;


		////////////////////ethernet 주소 를 브로드케스트로 설정/////////////////////////
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(m_sHeader.enet_sender_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(zero);
		((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0608);
		arpTable->AddIncompleteByIp(m_sHeader.ip_target_addr);
		bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader, 28);

	}
	else
	{
		/////////////////ip가 있으면 ARP Table에서 enthernet주소를 enthernetLayer에 알려줌////////////////
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(m_sHeader.enet_sender_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(m_sHeader.enet_target_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0008);
		bSuccess = mp_UnderLayer[0]->Send(tempData, nlength);
	}
	return bSuccess;
}

BOOL CARPLayer::Receive(unsigned char* ppayload)
{
	PARPLayer_HEADER pFrame = (PARPLayer_HEADER) ppayload;
	
	BOOL bSuccess = FALSE;
	/////////proxy ARP table에서 ip를 가지고 ethernet주소를 찾는다. /////////////
	/////////있으면 해당  ethernet주소, 없으면 0으로 채워진 주소/////////////
	unsigned char eth[6];

	unsigned char zero[6];
	zero[0] = 0x00; zero[1] = 0x00;
	zero[2] = 0x00; zero[3] = 0x00;
	zero[4] = 0x00; zero[5] = 0x00;

	///////////해당 IP인지 검사///////////////////
	if (memcmp((char *)pFrame->ip_target_addr, (char *)m_sHeader.ip_sender_addr, 4) == 0) 
	{
		//////////////////////target receive///////////
		if(pFrame->op == 0x0100) { 
			//////////////////해당 ARP주소가 있으면 다시 저장 안함////////////////////
			arpTable->check(ppayload);


			////////////////////ARP frame 설정 swap/////////////
			m_sHeader.op = 0x0200;
			memcpy(m_sHeader.enet_target_addr, pFrame->enet_sender_addr,6);
			memcpy(m_sHeader.ip_target_addr, pFrame->ip_sender_addr, 4);		
		
			/////////////////////ethernet 설정////////////////////
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(m_sHeader.enet_sender_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(m_sHeader.enet_target_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0608);

			return bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader, 28);
		}
		//////////////////////sender receive///////////
		else if(pFrame->op == 0x0200) {			
			//////////////////해당 ARP주소가 있으면 다시 저장 안함////////////////////
			arpTable->check(ppayload);
	
			pFrame->op = 0x0100;
			

			////////////////////ARP frame  설정 /////////////
			memcpy(m_sHeader.enet_target_addr, pFrame->enet_sender_addr, 6);
			memcpy(m_sHeader.ip_target_addr, pFrame->ip_sender_addr, 4);


			/////////////////////ethernet 설정////////////////////
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(m_sHeader.enet_sender_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(m_sHeader.enet_target_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0008);

			///////////////원래 보내고자 했던 데이터 보냄///////////////
			return bSuccess = mp_UnderLayer[0]->Send(tempData, tempLength);
		}
	}
	///////////////////target과 sender IP주소가 같으면 gratitiousARP이다//////////////////
	else if (memcmp((char *)pFrame->ip_target_addr, (char *)pFrame->ip_sender_addr, 4) == 0) {
		////////////////////////ARP Table에 해당하는 IP가 있는지 조사/////////////////////
		arpTable->check(ppayload);
		bSuccess = TRUE;
		return bSuccess;
	}
	else {
		memcpy(eth, pFrame->ip_target_addr, 4);
		////////////////////ARP frame 설정 swap/////////////
		memcpy(pFrame->enet_target_addr, pFrame->enet_sender_addr, 6);
		memcpy(pFrame->ip_target_addr, pFrame->ip_sender_addr, 4);
		memcpy(pFrame->enet_sender_addr, m_sHeader.enet_sender_addr, 6);
		memcpy(pFrame->ip_sender_addr, eth, 4);
		pFrame->op = 0x0200;
		/////////////////////ethernet 설정////////////////////
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(pFrame->enet_sender_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(pFrame->enet_target_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0608);

		return bSuccess = mp_UnderLayer[0]->Send((unsigned char*)pFrame, 28);
	}

	return bSuccess;
}



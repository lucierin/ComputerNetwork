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

	tempData = (unsigned char*)malloc(sizeof(unsigned char) * nlength);//////////////ARP�� ������ ������ ���� ������ �ӽ� �������
	memcpy(tempData, ppayload, nlength);
	tempLength = nlength;
	
	//////////////////ARP Table �˻�. ARP�� IP���� �����ϸ� ethernet�ּ� ����, ������ 0���� ä�� ethernet�ּ�//////////////////
	memcpy(m_sHeader.enet_target_addr, arpTable->find(m_sHeader.ip_target_addr), 6);
	unsigned char zero[6];
	zero[0] = 0x00; zero[1] = 0x00;
	zero[2] = 0x00; zero[3] = 0x00;
	zero[4] = 0x00; zero[5] = 0x00;

	if (memcmp((char *)m_sHeader.enet_target_addr, zero, 6) == 0)
	{
		//////////////////ip�� ������ ARP ����� ����///////////////////
		zero[0] = 0xff; zero[1] = 0xff;
		zero[2] = 0xff; zero[3] = 0xff;
		zero[4] = 0xff; zero[5] = 0xff;

		m_sHeader.op = 0x0100;


		////////////////////ethernet �ּ� �� ��ε��ɽ�Ʈ�� ����/////////////////////////
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(m_sHeader.enet_sender_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(zero);
		((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0608);
		arpTable->AddIncompleteByIp(m_sHeader.ip_target_addr);
		bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader, 28);

	}
	else
	{
		/////////////////ip�� ������ ARP Table���� enthernet�ּҸ� enthernetLayer�� �˷���////////////////
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
	/////////proxy ARP table���� ip�� ������ ethernet�ּҸ� ã�´�. /////////////
	/////////������ �ش�  ethernet�ּ�, ������ 0���� ä���� �ּ�/////////////
	unsigned char eth[6];

	unsigned char zero[6];
	zero[0] = 0x00; zero[1] = 0x00;
	zero[2] = 0x00; zero[3] = 0x00;
	zero[4] = 0x00; zero[5] = 0x00;

	///////////�ش� IP���� �˻�///////////////////
	if (memcmp((char *)pFrame->ip_target_addr, (char *)m_sHeader.ip_sender_addr, 4) == 0) 
	{
		//////////////////////target receive///////////
		if(pFrame->op == 0x0100) { 
			//////////////////�ش� ARP�ּҰ� ������ �ٽ� ���� ����////////////////////
			arpTable->check(ppayload);


			////////////////////ARP frame ���� swap/////////////
			m_sHeader.op = 0x0200;
			memcpy(m_sHeader.enet_target_addr, pFrame->enet_sender_addr,6);
			memcpy(m_sHeader.ip_target_addr, pFrame->ip_sender_addr, 4);		
		
			/////////////////////ethernet ����////////////////////
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(m_sHeader.enet_sender_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(m_sHeader.enet_target_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0608);

			return bSuccess = mp_UnderLayer[0]->Send((unsigned char*)&m_sHeader, 28);
		}
		//////////////////////sender receive///////////
		else if(pFrame->op == 0x0200) {			
			//////////////////�ش� ARP�ּҰ� ������ �ٽ� ���� ����////////////////////
			arpTable->check(ppayload);
	
			pFrame->op = 0x0100;
			

			////////////////////ARP frame  ���� /////////////
			memcpy(m_sHeader.enet_target_addr, pFrame->enet_sender_addr, 6);
			memcpy(m_sHeader.ip_target_addr, pFrame->ip_sender_addr, 4);


			/////////////////////ethernet ����////////////////////
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(m_sHeader.enet_sender_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(m_sHeader.enet_target_addr);
			((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0008);

			///////////////���� �������� �ߴ� ������ ����///////////////
			return bSuccess = mp_UnderLayer[0]->Send(tempData, tempLength);
		}
	}
	///////////////////target�� sender IP�ּҰ� ������ gratitiousARP�̴�//////////////////
	else if (memcmp((char *)pFrame->ip_target_addr, (char *)pFrame->ip_sender_addr, 4) == 0) {
		////////////////////////ARP Table�� �ش��ϴ� IP�� �ִ��� ����/////////////////////
		arpTable->check(ppayload);
		bSuccess = TRUE;
		return bSuccess;
	}
	else {
		memcpy(eth, pFrame->ip_target_addr, 4);
		////////////////////ARP frame ���� swap/////////////
		memcpy(pFrame->enet_target_addr, pFrame->enet_sender_addr, 6);
		memcpy(pFrame->ip_target_addr, pFrame->ip_sender_addr, 4);
		memcpy(pFrame->enet_sender_addr, m_sHeader.enet_sender_addr, 6);
		memcpy(pFrame->ip_sender_addr, eth, 4);
		pFrame->op = 0x0200;
		/////////////////////ethernet ����////////////////////
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetSrcAddress(pFrame->enet_sender_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetEnetDstAddress(pFrame->enet_target_addr);
		((CEthernetLayer*)mp_UnderLayer[0])->SetType((unsigned short)0x0608);

		return bSuccess = mp_UnderLayer[0]->Send((unsigned char*)pFrame, 28);
	}

	return bSuccess;
}



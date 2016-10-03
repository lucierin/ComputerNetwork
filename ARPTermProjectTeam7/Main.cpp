#include "StdAfx.h"
#include "Main.h"


Main::Main(void)
{
}


Main::~Main(void)
{
}

void Main::InitLayer()
{
	m_NI[0] = new CNILayer("NI0");
	m_NI[1] = new CNILayer("NI1");
	m_ETH[0] = new CEthernetLayer("Ethernet0");
	m_ETH[1] = new CEthernetLayer("Ethernet1");
	m_IP = new CIPLayer("IP");
	m_ARP[0] = new CARPLayer("ARP0");
	m_ARP[1] = new CARPLayer("ARP1");

	for(int i = 0; i < 2; i++)
	{
		m_NI[i]->SetUpperUnderLayer(m_ETH[i]);
		m_ETH[i]->SetUpperUnderLayer(m_ARP[i]);
		m_ETH[i]->SetUpperLayer(m_IP);
		m_IP->SetUnderLayer(m_ARP[i]);

		m_ETH[i]->setCount(i);
		m_ARP[i]->setCount(i);
	}

	m_IP->SetUpperUnderLayer(this);
}
void Main::InitAddress(CListBox* listARP, CListBox* listProxy, CListBox* listRouter)
{
	if(listARP) arpManager = new ARPManager(listARP, true);
	if(listProxy) proxyManager = new ARPManager(listProxy, false);
	InitMac();
	InitIP();
	
	for(int i = 0; i < 2; i++)
	{
		arpManager->init(i, mac, ipaddr[i]);
		proxyManager->init(i, mac, ipaddr[i]);
		m_ARP[i]->init(arpManager->Get(i), proxyManager->Get(i));
		m_NI[i]->init(arpManager->Get(i), proxyManager->Get(i));
	}

	m_IP->SetSrcIPAddress(ipaddr[0], ipaddr[1]);

	RouterTable* table = new RouterTable();
	if(listRouter) table->init(listRouter, m_NI[0]->GetAdapterObject(0)->description,
		m_NI[1]->GetAdapterObject(0)->description);
}
void Main::InitMac()
{
	memset(mac, 0, 6);

	PPACKET_OID_DATA OidData = new PACKET_OID_DATA();//(PPACKET_OID_DATA)malloc(sizeof(PACKET_OID_DATA));
	OidData->Oid = 0x01010101;
	OidData->Length = 6;

	LPADAPTER adapter[2];
	for(int i = 0; i < 2; i++)
	{
		adapter[i] = PacketOpenAdapter(m_NI[i]->GetAdapterObject(i)->name);
		for(int i = 0; i < 6; i++)
			mac[i] = OidData->Data[i];
		m_ARP[i]->SetEnetTargetAddress(mac);
		m_ETH[i]->SetEnetDstAddress(mac);

		PacketRequest(adapter[i], FALSE, OidData);
		for(int i = 0; i < 6; i++)
			mac[i] = OidData->Data[i];
		m_ARP[i]->SetEnetTargetAddress(mac);
		m_ETH[i]->SetEnetSrcAddress(mac);
	}
}
void Main::InitIP()
{
	WSADATA wsaData;

	PHOSTENT hostinfo;
	char hostname[50];
	memset(hostname, 0, sizeof(hostname));

	for(int i = 0; i < 2; i++)
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		int nError = gethostname(hostname, sizeof(hostname));
		if(nError == 0)
		{
			hostinfo = gethostbyname(hostname);
			InAddress* addr = (InAddress*) hostinfo->h_addr_list[i];
			ipaddr[i][0] = addr->S_un.S_un_b.s_b1;
			ipaddr[i][1] = addr->S_un.S_un_b.s_b2;
			ipaddr[i][2] = addr->S_un.S_un_b.s_b3;
			ipaddr[i][3] = addr->S_un.S_un_b.s_b4;
		}
		m_ARP[i]->SetIPSenderAddress(ipaddr[i]);

		WSACleanup();
		m_NI[i]->SetAdapterNumber(i);
		m_NI[i]->PacketStartDriver();
	}
}
void Main::ARPRemove(CListBox* listARP)
{
	ARPTable* table = m_ARP[0]->getArpTable();
	table->remove(listARP->GetCurSel());
	listARP->DeleteString(listARP->GetCurSel());
}
void Main::ARPClear()
{
	m_ARP[0]->getArpTable()->clear();
}
ARPTable* Main::GetProxyTable()
{
	return m_ARP[0]->getProxyTable();
}
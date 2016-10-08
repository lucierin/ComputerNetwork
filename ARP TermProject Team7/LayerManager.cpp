#include "StdAfx.h"
#include "LayerManager.h"


LayerManager::LayerManager()
{
}


LayerManager::~LayerManager(void)
{
}

void LayerManager::Init(MainDialog* dlg)
{
	this->dlg = dlg;
	mip = new CIPLayer("IP");
	mrip = new CRIPLayer("RIP");
	mudp = new CUDPLayer("UDP");

	mudp->SetUnderUpperLayer(dlg);
	mrip->SetUnderUpperLayer(mudp);
	mip->SetUnderUpperLayer(mrip);
	
	rtable = new RouterTable();
	rtable->init(&dlg->m_ListRouter);
	mip->Init(rtable, this);
	mudp->Init(rtable);
	mrip->Init(rtable);
}

void LayerManager::AddPort()
{
	int index = marps.size();
	CARPLayer* arp = new CARPLayer("ARP");
	CEthernetLayer* eth = new CEthernetLayer("ETH");
	CNILayer* ni = new CNILayer("NI");
	ARPTable* table = new ARPTable();

	arp->SetUnderUpperLayer(mip);
	eth->SetUnderUpperLayer(arp);
	ni->SetUnderUpperLayer(eth);

	marps.push_back(arp);
	meths.push_back(eth);
	mnis.push_back(ni);
	
	ni->SetAdapterNumber(index);
	ni->PacketStartDriver();

	arp->init(table);
	ni->init(table);

	uchar* ip = GetIP(index);
	uchar* mac = GetMac(index);
	table->init(mac, ip);
	tables.push_back(table);
}

void LayerManager::RemoveAt(int index)
{
	int k = 0;
	REMOVE_AT_LAYER(CARPLayer*, marps);
	REMOVE_AT_LAYER(CEthernetLayer*, meths);
	REMOVE_AT_LAYER(CNILayer*, mnis);
}

uchar* LayerManager::GetIP(int index)
{
	char hostname[50];
	PHOSTENT hostinfo;
	uchar* ipaddr = new uchar[4];

	memset(hostname, 0, sizeof(hostname));
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	int nError = gethostname(hostname, sizeof(hostname));
	if(nError == 0)
	{
		hostinfo = gethostbyname(hostname);
		InAddr* addr = (InAddr*)hostinfo->h_addr_list[index];
		ipaddr[0] = SunB.s_b1;
		ipaddr[1] = SunB.s_b2;
		ipaddr[2] = SunB.s_b3;
		ipaddr[3] = SunB.s_b4;
	}
	
	WSACleanup();

	return ipaddr;
}

uchar* LayerManager::GetMac(int index)
{
	uchar* mac = new uchar[6];
	memset(mac, 0, 6);

	PPACKET_OID_DATA OidData = new PACKET_OID_DATA();
	//OidData = (PPACKET_OID_DATA)malloc(sizeof(PACKET_OID_DATA));
	OidData->Oid = 0x01010101;
	OidData->Length = 6;

	LPADAPTER adapter = PacketOpenAdapter(mnis[index]->GetAdapterObject(index)->name);
	for(int i = 0; i < 6; i++)
		mac[i] = OidData->Data[i];
	
	return mac;
}
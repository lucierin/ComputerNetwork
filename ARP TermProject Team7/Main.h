#pragma once
#include "IPLayer.h"
#include "EthernetLayer.h"
#include "NILayer.h"
#include "ARPLayer.h"
#include "ARPManager.h"
#include <map>
#define SL std::pair<String, Map>
typedef struct in_addr InAddress;
class Main : public CBaseLayer
{
public:
	CIPLayer*			m_IP;
	CEthernetLayer*		m_ETH[2];
	CNILayer*			m_NI[2];
	CARPLayer*			m_ARP[2];

	ARPManager*			arpManager;
	ARPManager*			proxyManager;
	UCHAR ipaddr[2][4];
	UCHAR mac[6];
public:
	Main(void);
	~Main(void);
	void InitLayer();
	void InitAddress(CListBox* listARP = 0, CListBox* listProxy = 0, CListBox* listRouter = 0);
	void InitMac();
	void InitIP();
	void ARPRemove(CListBox* listARP);
	void ARPClear();
	ARPTable* GetProxyTable();
};


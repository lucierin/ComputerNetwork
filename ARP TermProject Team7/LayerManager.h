#pragma once
#include "UDPLayer.h"
#include "RIPLayer.h"
#include "IPLayer.h"
#include "NILayer.h"
#include "EthernetLayer.h"
#include "ARPLayer.h"
#include "ARP TermProject Team7Dlg.h"
#include "ARPTable.h"
#include "RouterTable.h"

class LayerManager
{
private:
	SVector<CARPLayer*> marps;
	SVector<CEthernetLayer*> meths;
	SVector<CNILayer*> mnis;
	SVector<ARPTable*> tables;
	RouterTable* rtable;
	MainDialog* dlg;
	CUDPLayer* mudp;
	CRIPLayer* mrip;
	CIPLayer* mip;
	
public:
	LayerManager();
	~LayerManager(void);
	void Init(MainDialog* dlg);
	void AddPort();
	void RemoveAt(int index);
	uchar* GetIP(int index);
	uchar* GetMac(int index);
};

typedef struct in_addr InAddr;
#define SunB addr->S_un.S_un_b

#define REMOVE_AT_LAYER(type, name)\
for(Iterator(type) i = name.begin(); i != name.end(); k++)\
{\
	if(k == index)\
	{\
		type layer = (*i);\
		i = name.erase(i);\
		delete layer;\
	}\
	else\
	{\
		i++;\
	}\
}
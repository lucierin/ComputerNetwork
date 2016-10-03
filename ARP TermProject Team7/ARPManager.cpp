#include "StdAfx.h"
#include "ARPManager.h"


ARPManager::ARPManager(CListBox* listbox, bool isARP)
{
	this->listbox = listbox;
	this->isARP = isARP;
}


ARPManager::~ARPManager(void)
{
}

void ARPManager::init(int index, UCHAR* mac, UCHAR* ip)
{
	tables[index] = new ARPTable(isARP);
	tables[index]->init(this, mac, ip);
}
void ARPManager::initLeft(unsigned char* mac, unsigned char* ip)
{
	tables[0] = new ARPTable(isARP);
	tables[0]->init(this, mac, ip);
}
void ARPManager::initRight(unsigned char* mac, unsigned char* ip)
{
	tables[1] = new ARPTable(isARP);
	tables[1]->init(this, mac, ip);
}
void ARPManager::update()
{
	listbox->ResetContent();
	tables[0]->addAll();
	tables[1]->addAll();
}
void ARPManager::AddString(PARP_ROW row, ARPTable* table, bool isARP)
{
	char value[100];
	int index = (tables[0] == table)? 0 : 1;
	unsigned char* ip = row->ip_addr;
	unsigned char* enet = row->enet_addr;
	if(isARP) {
		sprintf(value, "interface %d : %d.%d.%d.%d  %0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x  %s", 
			index, ip[0], ip[1], ip[2], ip[3], enet[0], enet[1], enet[2], enet[3], enet[4], enet[5]
			, ((row->status == 1)? "Complete" : "Incomplete"));
	}
	else {
		sprintf(value, "%d.%d.%d.%d  %0.2x:%0.2x:%0.2x:%0.2x:%0.2x:%0.2x", 
			ip[0], ip[1], ip[2], ip[3], enet[0], enet[1], enet[2], enet[3], enet[4], enet[5]);
	}
	if(listbox) listbox->AddString(value);
}
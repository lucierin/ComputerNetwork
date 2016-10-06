#include "stdafx.h"
#include "ARPTable.h"
#include "ARPLayer.h"
#include "ARPManager.h"
#include "ARP TermProject Team7Dlg.h"


ARPTable::ARPTable(bool isARP)
{
	zero = (unsigned char*) malloc(sizeof(unsigned char) * 6);
	memset(zero, 0, 6);
	this->isARP = isARP;
}


ARPTable::~ARPTable(void)
{
}

void ARPTable::init(ARPManager* arpManager, unsigned char* mac, unsigned char* ip) {
	this->arpManager = arpManager;
	this->myMac = (unsigned char*) malloc(sizeof(unsigned char) * 6);
	this->myIP = (unsigned char*)malloc(sizeof(unsigned char) * 4);
	memcpy(myMac, mac, 6);
	memcpy(myIP, ip, 4);
}

void ARPTable::add(unsigned char* packet)
{
	CARPLayer::PARPLayer_HEADER header = (CARPLayer::PARPLayer_HEADER) packet;
	char* buffer = (char*)malloc(sizeof(char) * 40);
	PARP_ROW row = new ARP_ROW();
	memcpy(row->enet_addr, header->enet_sender_addr, 6);
	memcpy(row->ip_addr, header->ip_sender_addr, 4);
	time_t timer = time(NULL);
	localtime_s(&row->time, &timer);
	row->status = 0;//Incomplete
	if (memcmp(header->enet_sender_addr, myMac, 6) != 0)
		row->status = 1;
	table.push_back(row);
	arpManager->update();
}

void ARPTable::addRow(unsigned char* ip)
{
	PARP_ROW row = new ARP_ROW();
	memcpy(row->enet_addr, myMac, 6);
	memcpy(row->ip_addr, ip, 4);
	time_t timer = time(NULL);
	localtime_s(&row->time, &timer);
	table.push_back(row);
	arpManager->update();
}

unsigned char* ARPTable::find(unsigned char* ip)
{
	for (std::vector<PARP_ROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		if (memcmp(ip, (*i)->ip_addr, 4) == 0) {
			return (*i)->enet_addr;
		}
	}

	value[0] = 0x00; value[1] = 0x00;
	value[2] = 0x00; value[3] = 0x00;
	value[4] = 0x00; value[5] = 0x00;
	return value;
}

void ARPTable::timeCheck()
{
	for (std::vector<PARP_ROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		time_t timer = time(NULL);
		struct tm time;
		localtime_s(&time, &timer);
		int t1 = time.tm_min * 60 + time.tm_sec;
		int t2 = (*i)->time.tm_min * 60 + (*i)->time.tm_sec;
		if (t1 - t2 > 120)
		{
			PARP_ROW row = (*i);
			i = table.erase(i);
			delete row;
			if (table.size() == 0 || i == table.end()) break;
		}
	}
	arpManager->update();
}

void ARPTable::check(unsigned char* packet)
{
	CARPLayer::PARPLayer_HEADER header = (CARPLayer::PARPLayer_HEADER) packet;
	for (std::vector<PARP_ROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		PARP_ROW row = (*i);

		time_t timer = time(NULL);
		struct tm time;
		localtime_s(&time, &timer);
		if (time.tm_sec - (*i)->time.tm_sec > 50)
		{
			i = table.erase(i);
			delete row;
			if(i == table.end() || table.size() == 0) break;
		}

		if (memcmp(row->ip_addr, header->ip_sender_addr, 4) == 0) {
			checkGratitious(row->enet_addr, header->enet_sender_addr);
			//memcpy(row->enet_addr, header->enet_sender_addr, 6);
			arpManager->update();
			return;
		}
	}
	add(packet);//만약 그어떤 테이블도 ip가 겹치는 곳이 없는 경우 새로 추가한다
}

void ARPTable::remove(int index)
{
	int find = 0;
	for (std::vector<PARP_ROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		if (find == index)
		{
			PARP_ROW row = (*i);
			i = table.erase(i);
			delete row;
			if (table.size() == 0 || i == table.end()) break;
		}
		find++;
	}
}

void ARPTable::clear()
{
	table.clear();
	arpManager->update();
}


void ARPTable::addAll()
{
	char* value = (char*) malloc(sizeof(char) * 100);
	for (std::vector<PARP_ROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		unsigned char* enet = (*i)->enet_addr;
		unsigned char* ip = (*i)->ip_addr;
		//arpManager->AddString((*i), this, isARP);
	}
	delete value;
}


void ARPTable::checkGratitious(unsigned char* origin, unsigned char* ether)
{	
	char data[6];
	memcpy(data, origin, 6);
	for (std::vector<PARP_ROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		if(memcmp((*i)->enet_addr, data, 6) == 0) {
			memcpy((*i)->enet_addr, ether, 6);
			(*i)->status = 1;
		}
	}
}

void ARPTable::addNew(unsigned char* ip)
{
	if (memcmp(ip, myIP, 4) == 0) return;
	PARP_ROW row = new ARP_ROW();
	memcpy(row->enet_addr, zero, 6);
	memcpy(row->ip_addr, ip, 4);
	time_t timer = time(NULL);
	localtime_s(&row->time, &timer);
	table.push_back(row);
	row->status = 0;
	arpManager->update();
}


void ARPTable::allset(unsigned char* eth)
{
	for (std::vector<PARP_ROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		memcpy((*i)->enet_addr, eth, 6);
	}
}
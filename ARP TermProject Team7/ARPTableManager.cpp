#include "StdAfx.h"
#include "ARPTableManager.h"


ARPTableManager::ARPTableManager(bool isARP)
{
	this->isARP = isARP;
}


ARPTableManager::~ARPTableManager(void)
{
}

void ARPTableManager::init(CListBox* listbox)
{
	this->listbox = listbox;
}
void ARPTableManager::addTable(unsigned char* mac, unsigned char* ip)
{
	ARPTable* table = new ARPTable(this, this->isARP);
	table->init(listbox, mac, ip);
	tables.push_back(table);
}
ARPTable* ARPTableManager::get(int index)
{
	if (index >= tables.size()) return NULL;
	else tables[index];
}
void ARPTableManager::update()
{
	listbox->ResetContent();
	for (std::vector<ARPTable*>::iterator i = tables.begin(); i != tables.end(); i++) {
		(*i)->update();
	}
}


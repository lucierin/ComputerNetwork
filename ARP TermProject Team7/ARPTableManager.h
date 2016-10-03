#pragma once
#include <vector>
#include "ARPTable.h"
class ARPTableManager
{
private:
	std::vector<ARPTable*> tables;
	CListBox* listbox;
	bool isARP;//false is proxy
public:
	ARPTableManager(bool isARP);
	~ARPTableManager(void);
	void init(CListBox* listbox);
	void addTable(unsigned char* mac, unsigned char* ip);
	ARPTable* get(int index);
	void update();
};


#pragma once
#include "ARPTable.h"
class ARPManager
{
private:
	//CListBox* listbox;
	ARPTable* tables[2];
	bool isARP;
public:
	ARPManager(bool isARP);
	~ARPManager(void);
	void init(int index, UCHAR* mac, UCHAR* ip);
	void initLeft(unsigned char* mac, unsigned char* ip);
	void initRight(unsigned char* mac, unsigned char* ip);
	void update();
	//void AddString(PARP_ROW row, ARPTable* table, bool isARP);
	ARPTable* getLeft() { return tables[0]; }
	ARPTable* getRight() { return tables[1]; }
	ARPTable* Get(int index) { return tables[index]; }
};


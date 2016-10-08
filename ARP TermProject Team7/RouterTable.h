#pragma once
#include "NILayer.h"

typedef struct _RT_ROW {
	unsigned char netmask[4];
	unsigned char dest[4];
	unsigned char gateway[4];
	int flag; //up, gateway, host
	int index; //index가 0이면 left 1이면 right
	int metric;
} RTROW, *PRTROW;

class RouterTable
{
private:
	std::vector<PRTROW> table;
	CListBox* listbox;
	unsigned char* zero;
public:
	//char *name1, *name2;
	RouterTable(void);
	~RouterTable(void);
	void init(CListBox* listbox);
	void add(unsigned char* netmask, unsigned char* dest, unsigned char* gateway, int flag, int index, int metric);
	int get(unsigned char* ip, unsigned char* gateway);
	void update();
	void remove(int index);
	void clear();
};


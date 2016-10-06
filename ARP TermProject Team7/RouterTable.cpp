#include "StdAfx.h"
#include "RouterTable.h"
#include "RouterDialog.h"


RouterTable::RouterTable(void)
{
	zero = (unsigned char*) malloc(sizeof(unsigned char*) * 6);
	memset(zero, 0, 6);
}


RouterTable::~RouterTable(void)
{
}


void RouterTable::init(CListBox* listbox, char* name1, char* name2)
{
	this->listbox = listbox;
	this->name1 = name1;
	this->name2 = name2;
}

void RouterTable::add(unsigned char* netmask, unsigned char* dest, unsigned char* gateway, int flag, int index, int metric)
{
	PRTROW row = new RTROW();
	memcpy(row->netmask, netmask, 4);
	memcpy(row->dest, dest, 4);
	memcpy(row->gateway, gateway, 4);
	row->flag = flag;
	row->index = index;
	row->metric = metric;
	table.push_back(row);
	update();
}

void RouterTable::update()
{
	this->listbox->ResetContent();
	for(std::vector<PRTROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		char abc[100];
		char sflag[4];
		unsigned char* netmask = (*i)->netmask;
		unsigned char* gateway = (*i)->gateway;
		unsigned char* dest = (*i)->dest;
		int flag = (*i)->flag;
		int index = (*i)->index;
		int metric = (*i)->metric;

		sprintf(sflag, "%s%s%s", (FLAGUP(flag))? "U":"", (FLAGGATEWAY(flag))? "G":"", (FLAGHOST(flag))? "H":"");
		if(memcmp(gateway, zero, 4) == 0) {
			sprintf(abc, "%d.%d.%d.%d  %d.%d.%d.%d 연결됨 %s Port:%d %d", dest[0], dest[1], dest[2], dest[3]
						,netmask[0], netmask[1], netmask[2], netmask[3], sflag, index+1, metric);
		}
		else {
			sprintf(abc, "%d.%d.%d.%d  %d.%d.%d.%d %d.%d.%d.%d %s Port:%d %d", dest[0], dest[1], dest[2], dest[3]
						,netmask[0], netmask[1], netmask[2], netmask[3]
						,gateway[0], gateway[1], gateway[2], gateway[3], sflag, index+1, metric);
		}
		this->listbox->AddString(abc);
	}
}

void RouterTable::remove(int index)
{
	int k = 0;
	for(std::vector<PRTROW>::iterator i = table.begin(); i != table.end(); i++)
	{
		if(k == index) {
			i = table.erase(i);
			return;
		}
		k++;
	}
	update();
}
//테이블에 대응하는 dest반환
int RouterTable::get(unsigned char* ip, unsigned char* gateway)
{
	for(std::vector<PRTROW>::iterator i = table.begin(); i != table.end(); i++) 
	{
		PRTROW row = (*i);

		//subnet을 통한 분리
		unsigned char data[4];
		unsigned char tableData[4];
		unsigned char zero[4];
		zero[0] = 0x00; zero[1] = 0x00;
		zero[2] = 0x00; zero[3] = 0x00;

		for(int i = 0; i < 4; i++) {
			data[i] = ip[i] & row->netmask[i];
			tableData[i] = row->dest[i] & row->netmask[i];
		}

		//subnet으로 제거된 나머지 ip와 열의 dest를 찾아 비교한다
		if(memcmp(data, tableData, 4) == 0) {
			if(memcmp(row->gateway, zero, 4) == 0)
				memcpy(gateway, ip, 4);
			else
				memcpy(gateway, row->gateway, 4);
			return row->index;
		}
	}
	//table의 destination중에는 없었다
	return -1;
}
void RouterTable::clear()
{
	while(table.size() > 0)
		remove(0);
}
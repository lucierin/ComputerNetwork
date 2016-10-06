#pragma once
#define TABLE_LENGTH 10
#include <time.h>
#include <vector>

class CARPTermProjectTeam7Dlg;
typedef struct _ARP_ROW {
	unsigned char ip_addr[4];
	unsigned char enet_addr[6];
	struct tm time;
	int status;
} ARP_ROW, *PARP_ROW;

class ARPManager;
class ARPTable
{
private:
	std::vector<PARP_ROW> table;
	ARPManager* arpManager;
	unsigned char* myMac; //무조건 6자리다
	unsigned char* myIP;
	unsigned char* zero;
	bool isARP;
public:
	unsigned char value[6];
	ARPTable(bool isARP);
	~ARPTable(void);
	void init(ARPManager* arpManager, unsigned char* mac, unsigned char* ip);
	void AddIncompleteByPacket(unsigned char* packet);
	void AddProxy(unsigned char* ip);
	void AddIncompleteByIp(unsigned char* ip);
	unsigned char* find(unsigned char* ip);
	void timeCheck();
	void check(unsigned char* packet);

	void remove(int index);
	void clear();
	void addAll();
	void allset(unsigned char* eth);

	void checkGratitious(unsigned char* origin, unsigned char* ether);
};
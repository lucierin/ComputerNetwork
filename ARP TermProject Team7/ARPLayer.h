#pragma once
#include "BaseLayer.h"
#include "ARPTable.h"
#include "ARPTableManager.h"

class CARPLayer : public CBaseLayer
{
private:
	inline void		ResetHeader();
	int				thisCount;
	
	ARPTable*		 arpTable;
public:
	
	CARPLayer(char* pName);
	virtual ~CARPLayer();
	void			init(ARPTable* arpTable);

	void			SetEnetSenderAddress(unsigned char* pAddress);
	void			SetIPSenderAddress(unsigned char* pAddress);
	void			SetEnetTargetAddress(unsigned char* pAddress);
	void			SetIPTargetAddress(unsigned char* pAddress);
	void			setCount(int count);
	void			SetOP(unsigned short op);
	unsigned char*	GetEnetSenderAddress();
	unsigned char*	GetIPSenderAddress();
	unsigned char*	GetEnetTargetAddress();
	unsigned char*	GetIPTargetAddress();

	BOOL			Send(unsigned char* ppayload, int nlength);
	BOOL			Receive(unsigned char* ppayload);

	unsigned char*	findTable(unsigned char* ip_addr);
	BOOL			addTable(unsigned char* ip_addr, unsigned char* enet_addr);
	BOOL			removeTable(unsigned char* ip_addr);

	unsigned char*   tempData;
	int				 tempLength;
	ARPTable*		 getArpTable() { return arpTable; }


	typedef struct _ARPLayer_HEADER 
	{
		unsigned short hard_type;			// hard type				(2byte)
		unsigned short prot_type;			// protcol type				(2byte)
		unsigned char hard_size;			// hardwara length			(1byte)
		unsigned char prot_size;			// protcol length			(1byte)
		unsigned short op;					// operation fild			(2byte)
		unsigned char enet_sender_addr[6];	// sneder Ethernet address	(6byte)
		unsigned char ip_sender_addr[4];	// sneder IP address		(4byte)
		unsigned char enet_target_addr[6];	// target Ethernet address	(6byte)
		unsigned char ip_target_addr[4];	// target IP address		(4byte)
	}ARPLayer_HEADER, *PARPLayer_HEADER;

	public:

		ARPLayer_HEADER	m_sHeader;
};




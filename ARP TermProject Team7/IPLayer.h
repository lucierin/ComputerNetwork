// IPLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "RouterTable.h"
#include "ARPLayer.h"
#define  IP_COUNT 10

class LayerManager;
class CIPLayer 
: public CBaseLayer  
{
private:
	RouterTable* table;
	inline void		ResetHeader( );
	unsigned char srcip[IP_COUNT][4];
	unsigned char destip[4];
	LayerManager* manager;
	HANDLE sem;//modified
public:
	CIPLayer( char* pName );
	virtual ~CIPLayer();
	void SetSrcIPAddress(unsigned char* src_ip1, unsigned char* src_ip2);
	void addSrcIPAddress(unsigned char* src_ip);
	void deleteSrcIPAddress(unsigned char* src_ip);
	void SetDstIPAddress(unsigned char* dst_ip);
	void SetFragOff(unsigned short fragoff);
	void SetIpProto(unsigned char ip_proto);

	void Init(RouterTable* table, LayerManager* manager);
	RouterTable* getTable() { return table; }
	unsigned char* GetSrcIPAddress();
	unsigned char* GetDstIPAddress();
	

	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);

	typedef struct _IPLayer_HEADER {
		unsigned char ip_verlen;	// ip version		(1byte)
		unsigned char ip_tos;		// type of service	(1byte)
		unsigned short ip_len;		// total packet length	(2byte)
		unsigned short ip_id;		// datagram id			(2byte)
		unsigned short ip_fragoff;	// fragment offset		(2byte)
		unsigned char ip_ttl;		// time to live in gateway hops (1byte)
		unsigned char ip_proto;		// IP protocol			(1byte)
		unsigned short ip_cksum;	// header checksum		(2byte)
		unsigned char ip_src[4];	// IP address of source	(4byte)
		unsigned char ip_dst[4];	// IP address of destination	(4byte)
		unsigned char ip_data[IP_DATA_SIZE]; // variable length data
		
	} IPLayer_HEADER, *PIPLayer_HEADER ;
	
	typedef struct _ICMP_HEADER {
		unsigned char type;	// ip version		(1byte)
		unsigned char code;		// type of service	(1byte)
		unsigned short cksum;		// total packet length	(2byte)
		unsigned int message;		// datagram id			(2byte)
	} ICMP_HEADER, *PICMP_HEADER;

protected:
	IPLayer_HEADER	m_sHeader ;
	ICMP_HEADER m_icmp;
};





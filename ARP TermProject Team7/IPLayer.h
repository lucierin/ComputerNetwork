// IPLayer.h: interface for the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"
#include "RouterTable.h"
#include "ARPLayer.h"

class CIPLayer 
: public CBaseLayer  
{
private:
	RouterTable* table;
	inline void		ResetHeader( );
	unsigned char srcip[2][4];
	unsigned char destip[4];
	HANDLE sem;//modified
public:
	CIPLayer( char* pName );
	virtual ~CIPLayer();
	void SetSrcIPAddress(unsigned char* src_ip1, unsigned char* src_ip2);
	void SetDstIPAddress(unsigned char* dst_ip);
	void SetFragOff(unsigned short fragoff);

	void init(RouterTable* table) { this->table = table; }
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
	
protected:
	IPLayer_HEADER	m_sHeader ;
};





// BaseLayer.cpp: implementation of the CBaseLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARP TermProject Team7.h"
#include "BaseLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseLayer::CBaseLayer( char* pName )		// 모든 계층이 상속 받을 기본 계층의 생성자	
: m_nUpperLayerCount( 0 ),	// 자신의 위계층을 가르킬 Layercount를 초기화
  m_nUnderLayerCount( 0 )		// 자산의 아래계층을 초기화
{
	m_pLayerName = pName ;	// 만들어 지는 계층의 이름을 입력받은 것으로 한다.
}

CBaseLayer::~CBaseLayer()	// 이 계층이 사라질 때 없앨 소멸자
{

}

void CBaseLayer::SetUnderUpperLayer(CBaseLayer *pUULayer)	// 자신과 자신의 아래가 될 계층의 연결하기 위한 함수
{
	if ( !pUULayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUnderUpperLayer] The variable , 'pUULayer' is NULL" ) ;
#endif
		return ;			
	}

	SetUnderLayer(pUULayer);	// 들어온 파라매터를 자신의 아래계층으로 연결한다.
	pUULayer->SetUpperLayer(this);	// 들어온 파라메터의 위계층으로 자신을 연결한다.

}

void CBaseLayer::SetUpperUnderLayer(CBaseLayer *pUULayer)	// 자기 자신과 자신의 위가 될 계층을 연결하기 위한 함수
{
	if ( !pUULayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUpperUnderLayer] The variable , 'pUULayer' is NULL" ) ;
#endif
		return ;			
	}

	SetUpperLayer(pUULayer);	// 파라메터로 들어온 것을 자신의 위계층으로 설정
	pUULayer->SetUnderLayer(this);	// 파라메터로 들어온 것의 아래에 자신을 연결

}

void CBaseLayer::SetUpperLayer(CBaseLayer *pUpperLayer )	// 자신의 위계층으로 설정하는 함수
{
	if ( !pUpperLayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUpperLayer] The variable , 'pUpperLayer' is NULL" ) ;
#endif
		return ;			
	}

	// UpperLayer is added..
	this->mp_aUpperLayer[ m_nUpperLayerCount++ ] = pUpperLayer ;	// 위계층의 저정한 배열에 파라메터를 삽입
}

void CBaseLayer::SetUnderLayer(CBaseLayer *pUnderLayer)		// 자산의 아래 계층으로 설정하는 함수
{
	if ( !pUnderLayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUnderLayer] The variable , 'pUnderLayer' is NULL\n" ) ;
#endif
		return ;			
	}

	// UnderLayer assignment..
	this->mp_UnderLayer[m_nUnderLayerCount++] = pUnderLayer;		// 자신의 아래를 가르키는 변수에 파라메터를 가르키게 한다.
}

CBaseLayer* CBaseLayer::GetUpperLayer(int nindex)		// 해당 index에 해당하는 위 계층을 return하는 함수
{
	if ( nindex < 0						||
		 nindex > m_nUpperLayerCount	||
		 m_nUpperLayerCount < 0			)
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::GetUpperLayer] There is no UpperLayer in Array..\n" ) ;
#endif 
		return NULL ;
	}

	return mp_aUpperLayer[ nindex ] ;
}

CBaseLayer* CBaseLayer::GetUnderLayer(int nindex)		// 자신의 아래 계층을 리턴하는 함수
{
	if (nindex < 0 ||
		nindex > m_nUnderLayerCount ||
		m_nUnderLayerCount < 0)
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::GetUnderLayer] There is not a UnerLayer..\n" ) ;
#endif 
		return NULL ;
	}

	return mp_UnderLayer[nindex];
}

char* CBaseLayer::GetLayerName()		// 자신의 계층의 이름을 return하는 함수
{
	return m_pLayerName ; 
}

unsigned short CBaseLayer::ntohs(unsigned short a)
{
	return ( (a & 0x00ff) << 8 | (a & 0xff00) >> 8 );
}

unsigned short CBaseLayer::htons(unsigned short a)
{
	return ( (a & 0x00ff) << 8 | (a & 0xff00) >> 8 );
}
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

CBaseLayer::CBaseLayer( char* pName )		// ��� ������ ��� ���� �⺻ ������ ������	
: m_nUpperLayerCount( 0 ),	// �ڽ��� �������� ����ų Layercount�� �ʱ�ȭ
  m_nUnderLayerCount( 0 )		// �ڻ��� �Ʒ������� �ʱ�ȭ
{
	m_pLayerName = pName ;	// ����� ���� ������ �̸��� �Է¹��� ������ �Ѵ�.
}

CBaseLayer::~CBaseLayer()	// �� ������ ����� �� ���� �Ҹ���
{

}

void CBaseLayer::SetUnderUpperLayer(CBaseLayer *pUULayer)	// �ڽŰ� �ڽ��� �Ʒ��� �� ������ �����ϱ� ���� �Լ�
{
	if ( !pUULayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUnderUpperLayer] The variable , 'pUULayer' is NULL" ) ;
#endif
		return ;			
	}

	SetUnderLayer(pUULayer);	// ���� �Ķ���͸� �ڽ��� �Ʒ��������� �����Ѵ�.
	pUULayer->SetUpperLayer(this);	// ���� �Ķ������ ���������� �ڽ��� �����Ѵ�.

}

void CBaseLayer::SetUpperUnderLayer(CBaseLayer *pUULayer)	// �ڱ� �ڽŰ� �ڽ��� ���� �� ������ �����ϱ� ���� �Լ�
{
	if ( !pUULayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUpperUnderLayer] The variable , 'pUULayer' is NULL" ) ;
#endif
		return ;			
	}

	SetUpperLayer(pUULayer);	// �Ķ���ͷ� ���� ���� �ڽ��� ���������� ����
	pUULayer->SetUnderLayer(this);	// �Ķ���ͷ� ���� ���� �Ʒ��� �ڽ��� ����

}

void CBaseLayer::SetUpperLayer(CBaseLayer *pUpperLayer )	// �ڽ��� ���������� �����ϴ� �Լ�
{
	if ( !pUpperLayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUpperLayer] The variable , 'pUpperLayer' is NULL" ) ;
#endif
		return ;			
	}

	// UpperLayer is added..
	this->mp_aUpperLayer[ m_nUpperLayerCount++ ] = pUpperLayer ;	// �������� ������ �迭�� �Ķ���͸� ����
}

void CBaseLayer::SetUnderLayer(CBaseLayer *pUnderLayer)		// �ڻ��� �Ʒ� �������� �����ϴ� �Լ�
{
	if ( !pUnderLayer ) // if the pointer is null, 
	{
#ifdef _DEBUG
		TRACE( "[CBaseLayer::SetUnderLayer] The variable , 'pUnderLayer' is NULL\n" ) ;
#endif
		return ;			
	}

	// UnderLayer assignment..
	this->mp_UnderLayer[m_nUnderLayerCount++] = pUnderLayer;		// �ڽ��� �Ʒ��� ����Ű�� ������ �Ķ���͸� ����Ű�� �Ѵ�.
}

CBaseLayer* CBaseLayer::GetUpperLayer(int nindex)		// �ش� index�� �ش��ϴ� �� ������ return�ϴ� �Լ�
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

CBaseLayer* CBaseLayer::GetUnderLayer(int nindex)		// �ڽ��� �Ʒ� ������ �����ϴ� �Լ�
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

char* CBaseLayer::GetLayerName()		// �ڽ��� ������ �̸��� return�ϴ� �Լ�
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
// ProxyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ARP TermProject Team7.h"
#include "ProxyDialog.h"
#include "afxdialogex.h"


// CProxyDialog dialog

IMPLEMENT_DYNAMIC(CProxyDialog, CDialogEx)

CProxyDialog::CProxyDialog(ARPTable* proxy, CWnd* pParent /*=NULL*/)
	: CDialogEx(CProxyDialog::IDD, pParent)
{
	this->proxyTable = proxy;
}

CProxyDialog::~CProxyDialog()
{
}

void CProxyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ProxyIP);
}


BEGIN_MESSAGE_MAP(CProxyDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProxyDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CProxyDialog message handlers


void CProxyDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	unsigned char* ip = (unsigned char*) malloc(sizeof(unsigned char) * 4);
	m_ProxyIP.GetAddress(ip[0], ip[1], ip[2], ip[3]);
	proxyTable->AddProxy(ip);
	CDialogEx::OnOK();
}

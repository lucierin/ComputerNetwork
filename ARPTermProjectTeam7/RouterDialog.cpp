#include "StdAfx.h"
#include "RouterDialog.h"
#include "ARP TermProject Team7.h"


IMPLEMENT_DYNAMIC(CRouterDialog, CDialogEx)

CRouterDialog::CRouterDialog(RouterTable* table, CWnd* pParent /*=NULL*/)
	: CDialogEx(CRouterDialog::IDD, pParent)
	, flag_UP(FALSE)
	, flag_Gateway(FALSE)
	, flag_Host(FALSE)
	, m_Metric(_T(""))
{
	this->table = table;
}

CRouterDialog::~CRouterDialog()
{
}

BOOL CRouterDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_interface.InsertString(0,this->table->name1);
	m_interface.InsertString(1,this->table->name2);
	m_interface.SetCurSel(0);
	return true;
}

void CRouterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_dstip);
	DDX_Control(pDX, IDC_IPADDRESS2, m_netmask);
	DDX_Control(pDX, IDC_IPADDRESS3, m_gateway);
	DDX_Control(pDX, IDC_CHECK2, m_G);
	DDX_Control(pDX, IDC_CHECK3, m_H);
	DDX_Control(pDX, IDC_CHECK1, m_U);
	DDX_Control(pDX, IDC_COMBO1, m_interface);
	DDX_Check(pDX, IDC_CHECK1, flag_UP);
	DDX_Check(pDX, IDC_CHECK2, flag_Gateway);
	DDX_Check(pDX, IDC_CHECK3, flag_Host);
	DDX_Text(pDX, IDC_EDIT1, m_Metric);
}


BEGIN_MESSAGE_MAP(CRouterDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRouterDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRouterDialog::OnBnClickedCancel)
END_MESSAGE_MAP()










void CRouterDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
	unsigned char dest[4];
	unsigned char netmask[4];
	unsigned char gateway[4];
	m_dstip.GetAddress(dest[0], dest[1], dest[2], dest[3]);
	m_netmask.GetAddress(netmask[0], netmask[1], netmask[2], netmask[3]);
	m_gateway.GetAddress(gateway[0], gateway[1], gateway[2], gateway[3]);

	int flag = 0;
	if(flag_UP) flag += 1;
	if(flag_Gateway) flag += 10;
	if(flag_Host) flag += 100;

	this->table->add(netmask, dest, gateway, flag, m_interface.GetCurSel(), atoi(m_Metric));
}


void CRouterDialog::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


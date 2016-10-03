
// ARP TermProject Team7Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ARP TermProject Team7.h"
#include "ARP TermProject Team7Dlg.h"
#include "afxdialogex.h"
#include "ARPTable.h"
#include "ProxyDialog.h"
#include "RouterDialog.h"
#include "ARPManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CARPTermProjectTeam7Dlg 대화 상자




CARPTermProjectTeam7Dlg::CARPTermProjectTeam7Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARPTermProjectTeam7Dlg::IDD, pParent)
	, CBaseLayer("AppDlg")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CARPTermProjectTeam7Dlg::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListARP);
	//DDX_Control(pDX, IDC_IPADDRESS1, m_ARPIP);
	DDX_Control(pDX, IDC_LIST2, m_ListProxy);
	DDX_Control(pDX, IDC_LIST3, m_ListRouter);
	//DDX_Text(pDX, IDC_EDIT1, m_ETHERNET);
}

BEGIN_MESSAGE_MAP(CARPTermProjectTeam7Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CARPTermProjectTeam7Dlg::OnBnClickedItemDelete)
	ON_BN_CLICKED(IDC_BUTTON2, &CARPTermProjectTeam7Dlg::OnBnClickedAllDelete)
	//ON_BN_CLICKED(IDC_BUTTON5, &CARPTermProjectTeam7Dlg::OnBnClickedARPCacheSend)
	ON_BN_CLICKED(IDC_BUTTON3, &CARPTermProjectTeam7Dlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON9, &CARPTermProjectTeam7Dlg::OnBnClickedProxyDelete)
	//ON_BN_CLICKED(IDC_BUTTON6, &CARPTermProjectTeam7Dlg::OnBnClickedProxySend)
	ON_BN_CLICKED(IDC_BUTTON7, &CARPTermProjectTeam7Dlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_BUTTON8, &CARPTermProjectTeam7Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON4, &CARPTermProjectTeam7Dlg::OnBnClickedRouterAdd)
	ON_BN_CLICKED(IDC_BUTTON5, &CARPTermProjectTeam7Dlg::OnBnClickedRouterDelete)
END_MESSAGE_MAP()


// CARPTermProjectTeam7Dlg 메시지 처리기

BOOL CARPTermProjectTeam7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_LayerMgr.AddLayer( new CNILayer( "NI0") );
	m_LayerMgr.AddLayer(new CNILayer("NI1"));
	m_LayerMgr.AddLayer( new CEthernetLayer( "Ethernet0" ) );
	m_LayerMgr.AddLayer(new CEthernetLayer("Ethernet1"));
	m_LayerMgr.AddLayer( new CIPLayer( "IP" ) );
	m_LayerMgr.AddLayer( new CARPLayer( "ARP0" ) );
	m_LayerMgr.AddLayer(new CARPLayer("ARP1"));
	m_LayerMgr.AddLayer( this );

	m_LayerMgr.ConnectLayers("(NI0 ( *Ethernet0 ( *ARP0 ( *IP ) *IP ( *AppDlg ) ) ) *NI1 ( *Ethernet1 ( *ARP1 ( *IP ) *IP ( *AppDlg ) ) ) )");

	CBaseLayer* NILayer[2];
	CBaseLayer* EthernetLayer[2];
	CBaseLayer* ARPLayer[2];
	for (int i = 0; i < 2; i++) {
		

		char ni[4] = "NI ";
		char ethernet[10] = "Ethernet ";
		char arp[5] = "ARP ";

		ni[2] = (char)(48 + i);
		ethernet[8] = (char)(48 + i);
		arp[3] = (char)(48 + i);

		NILayer[i] = (CBaseLayer*)m_LayerMgr.GetLayer(ni);
		EthernetLayer[i] = (CBaseLayer*)m_LayerMgr.GetLayer(ethernet);
		ARPLayer[i] = (CBaseLayer*)m_LayerMgr.GetLayer(arp);
	}
	
	
	CBaseLayer* IPLayer = (CBaseLayer*) m_LayerMgr.GetLayer("IP");
	CBaseLayer* ARP_DIgLayer = (CBaseLayer*) m_LayerMgr.GetLayer("AppDlg");
	
	for (int i = 0; i < 2; i++) {
		NILayer[i]->SetUpperUnderLayer(EthernetLayer[i]);
		EthernetLayer[i]->SetUpperUnderLayer(ARPLayer[i]);
		EthernetLayer[i]->SetUpperLayer(IPLayer);
		IPLayer->SetUnderLayer(ARPLayer[i]);
	}
	
	IPLayer->SetUpperUnderLayer(ARP_DIgLayer);

	m_IP = (CIPLayer*) m_LayerMgr.GetLayer("IP");

	for (int i = 0; i < 2; i++) {


		char ni[4] = "NI ";
		char ethernet[10] = "Ethernet ";
		char arp[5] = "ARP ";

		ni[2] = (char)(48 + i);
		ethernet[8] = (char)(48 + i);
		arp[3] = (char)(48 + i);

		m_ETH[i] = (CEthernetLayer*)m_LayerMgr.GetLayer(ethernet);
		m_NI[i] = (CNILayer*)m_LayerMgr.GetLayer(ni);
		m_ARP[i] = (CARPLayer*)m_LayerMgr.GetLayer(arp);

		m_ETH[i]->setCount(i);
		m_ARP[i]->setCount(i);
	}
	
	InitAddress();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CARPTermProjectTeam7Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CARPTermProjectTeam7Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CARPTermProjectTeam7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CARPTermProjectTeam7Dlg::OnBnClickedItemDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_ListARP.GetCurSel();
	ARPTable* table = m_ARP[0]->getArpTable();
	table->remove(index);
	m_ListARP.DeleteString(m_ListARP.GetCurSel());
}


void CARPTermProjectTeam7Dlg::OnBnClickedAllDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ARP[0]->getArpTable()->clear();
}

void CARPTermProjectTeam7Dlg::InitAddress( )
{
	unsigned char* mac = (unsigned char*) malloc(sizeof(unsigned char) * 6);
	ARPManager* arpManager = new ARPManager(&m_ListARP, true);
	ARPManager* proxyManager = new ARPManager(&m_ListProxy, false);
	
	memset(mac, 0, 6);
	mac[0] = 0x00; mac[1] = 0x00;
	mac[2] = 0x00; mac[3] = 0x00;
	mac[4] = 0x00; mac[5] = 0x00;
	PPACKET_OID_DATA OidData;
	OidData = (PPACKET_OID_DATA)malloc(sizeof(PACKET_OID_DATA));
	OidData->Oid = 0x01010101;
	OidData->Length = 6;

	LPADAPTER adapter[2];

	for (int i = 0; i < 2; i++) {
		adapter[i] = PacketOpenAdapter(m_NI[i]->GetAdapterObject(i)->name);
		mac[0] = OidData->Data[0]; mac[1] = OidData->Data[1];
		mac[2] = OidData->Data[2]; mac[3] = OidData->Data[3];
		mac[4] = OidData->Data[4]; mac[5] = OidData->Data[5];

		m_ARP[i]->SetEnetTargetAddress(mac);
		m_ETH[i]->SetEnetDstAddress(mac);
	}
		
	WSADATA wsaData;
	

	PHOSTENT hostinfo;
	char hostname[50];
	unsigned char ipaddr[2][4];
	memset(hostname, 0, sizeof(hostname));
	//memset(ipaddr, 0, sizeof(ipaddr));
	
	for (int i = 0; i < 2; i++) {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		int nError = gethostname(hostname, sizeof(hostname));
		if (nError == 0)
		{
			hostinfo = gethostbyname(hostname);
			// ip address 파악
			struct in_addr* addr = (struct in_addr*)hostinfo->h_addr_list[i];
			ipaddr[i][0] = addr->S_un.S_un_b.s_b1;
			ipaddr[i][1] = addr->S_un.S_un_b.s_b2;
			ipaddr[i][2] = addr->S_un.S_un_b.s_b3;
			ipaddr[i][3] = addr->S_un.S_un_b.s_b4;
		}

		PacketRequest(adapter[i], FALSE, OidData);
		mac[0] = OidData->Data[0]; mac[1] = OidData->Data[1];
		mac[2] = OidData->Data[2]; mac[3] = OidData->Data[3];
		mac[4] = OidData->Data[4]; mac[5] = OidData->Data[5];

		m_ARP[i]->SetEnetSenderAddress(mac);
		m_ETH[i]->SetEnetSrcAddress(mac);
		
		
		m_ARP[i]->SetIPSenderAddress(ipaddr[i]);
				

		WSACleanup();

		m_NI[i]->SetAdapterNumber(i);
		m_NI[i]->PacketStartDriver();
		
		if(i == 0) {
			arpManager->initLeft(mac, ipaddr[0]);
			proxyManager->initLeft(mac, ipaddr[0]);
			m_ARP[i]->init(arpManager->getLeft(), proxyManager->getLeft());
			m_NI[i]->init(arpManager->getLeft(), proxyManager->getLeft());
		}
		else {
			arpManager->initRight(mac, ipaddr[1]);
			proxyManager->initRight(mac, ipaddr[1]);
			m_ARP[i]->init(arpManager->getRight(), proxyManager->getRight());
			m_NI[i]->init(arpManager->getRight(), proxyManager->getRight());
		}
	}	
	m_IP->SetSrcIPAddress(ipaddr[0], ipaddr[1]);

	RouterTable* rt = new RouterTable();
	rt->init(&m_ListRouter, m_NI[0]->GetAdapterObject(0)->description, m_NI[1]->GetAdapterObject(1)->description);
	m_IP->init(rt);
}


void CARPTermProjectTeam7Dlg::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CProxyDialog dlg(m_ARP[0]->getProxyTable(), this);
	dlg.DoModal();
}


void CARPTermProjectTeam7Dlg::OnBnClickedProxyDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_ListProxy.GetCurSel();
	ARPTable* table = m_ARP[0]->getProxyTable();
	table->remove(index);
	m_ListARP.DeleteString(m_ListProxy.GetCurSel());
}


void CARPTermProjectTeam7Dlg::OnBnClickedQuit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CARPTermProjectTeam7Dlg::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


BOOL CARPTermProjectTeam7Dlg::Receive( unsigned char* ppayload )
{
	return true;
}

void CARPTermProjectTeam7Dlg::OnBnClickedRouterAdd()
{
	CRouterDialog EntryDlg(m_IP->getTable());
	EntryDlg.DoModal();
}


void CARPTermProjectTeam7Dlg::OnBnClickedRouterDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

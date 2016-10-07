
// ARP TermProject Team7Dlg.h : ��� ����
//

#pragma once

#include "EthernetLayer.h"
#include "NILayer.h"
#include "ARPLayer.h"
#include "IPLayer.h"
#include "LayerManager.h"
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"
#include "RIPLayer.h"
#include "UDPLayer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// CARPTermProjectTeam7Dlg ��ȭ ����
class CARPTermProjectTeam7Dlg : public CDialogEx, public CBaseLayer
{
// �����Դϴ�.
public:
	CARPTermProjectTeam7Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.

	enum { IDD = IDD_ARPTERMPROJECTTEAM7_DIALOG };
	//CListBox		m_ListChat;		// Chatting Message View
	CString			m_unSrcEnetAddr;// Source Ethernet Address
	CString			m_unDstEnetAddr;// Destination Ethernet Address
	CString			m_stMessage;	// Chatting Message String

	CIPAddressCtrl	m_unSrcIPAddr;	// Source IP Address
	CIPAddressCtrl	m_unDstIPAddr;	// Destination IP Address
	CString			m_filePath;		// File Path
	CProgressCtrl	m_ProgressCtrl;	// Progress bar for File transfer

	CComboBox		m_ComboEnetName;// ComboBox for Ethernet Name List
	//}}AFX_DATA

	// ���� send ��ư Control
	afx_msg void OnOffFileButton(BOOL bBool);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCAppDlg)
	//����
	//public:
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
public:
	BOOL			Receive( unsigned char* ppayload );
	inline void		SendData( );

private:
	CLayerManager	m_LayerMgr;

	enum {			IPC_INITIALIZING, 
					IPC_READYTOSEND, 
					IPC_WAITFORACK,
					IPC_ERROR,
					IPC_BROADCASTMODE,
					IPC_UNICASTMODE,
					IPC_ADDR_SET,
					IPC_ADDR_RESET,
					CFT_COMBO_SET } ;

	void			SetDlgState( int state );
	void			InitAddress( );
	inline void		EndofProcess( );
	inline void		SetRegstryMessage( );

	BOOL			m_bSendReady;

// Object App
	CIPLayer*		m_IP;
	CEthernetLayer*	m_ETH[2];
	CNILayer*		m_NI[2];
	CARPLayer*		m_ARP[2];
	CRIPLayer*		m_RIP;
	CUDPLayer*		m_UDP;

// Implementation
	UINT			m_wParam;
	DWORD			m_lParam;
public:
	//CListBox m_ListARP;
	//CIPAddressCtrl m_ARPIP;
	//CListBox m_ListProxy;
	CListBox m_ListRouter;
	//CString		m_ETHERNET;
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedAdd();
};

#pragma once
#include "afxcmn.h"
#include "ARP TermProject Team7.h"
#include "RouterTable.h"

#define FLAGUP(x) ((x % 10 == 1)? true : false)
#define FLAGGATEWAY(x) ((x % 100 / 10 == 1)? true : false)
#define FLAGHOST(x) ((x / 100 == 1)? true : false)

class CRouterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRouterDialog)
private:
	RouterTable* table;

public:
	CRouterDialog(RouterTable* table, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRouterDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };
	BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_dstip;
	CIPAddressCtrl m_netmask;
	CIPAddressCtrl m_gateway;
	CButton m_G;
	CButton m_H;
	CButton m_U;
	CComboBox m_interface;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL flag_UP;
	BOOL flag_Gateway;
	BOOL flag_Host;
	afx_msg void OnEnChangeEdit1();
	CString m_Metric;
};

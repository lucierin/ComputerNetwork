#pragma once
#include "ARPTable.h"
#include "afxcmn.h"

// CProxyDialog dialog

class CProxyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProxyDialog)
private:
	ARPTable* proxyTable;

public:
	CProxyDialog(ARPTable* proxy, CWnd* pParent = NULL);   // standard constructor
	virtual ~CProxyDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_ProxyIP;
	afx_msg void OnBnClickedOk();
};

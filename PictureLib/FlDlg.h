#pragma once
#include "resource.h"
#include "AllHead.h"

// CFlDlg �Ի���

class CFlDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CFlDlg)

public:
	CFlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFlDlg();

// �Ի�������
	enum { IDD = IDD_FL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
//	CAcUiListCtrl m_list;
	CListCtrl m_list;
	CString m_sFlName;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
};

#pragma once
#include "resource.h"
#include "AllHead.h"

// CFlDlg 对话框

class CFlDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CFlDlg)

public:
	CFlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlDlg();

// 对话框数据
	enum { IDD = IDD_FL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

#pragma once
#include "resource.h"
#include "PaletteChildDlg.h"

// CMyPalette 对话框

class CMyPalette : public CAdUiPalette
{
	DECLARE_DYNAMIC(CMyPalette)

public:
	CMyPalette(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyPalette();

// 对话框数据
	enum { IDD = IDD_PALETTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	

	DECLARE_MESSAGE_MAP()
public:
	CPaletteChildDlg *m_childDlg;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL CreatChild();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	BOOL DestroyWindow();
};

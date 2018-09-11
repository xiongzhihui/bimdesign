#pragma once
#include "resource.h"
#include "AllHead.h"

// CPaletteChildDlg 对话框

class CPaletteChildDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CPaletteChildDlg)

public:
	CPaletteChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaletteChildDlg();

// 对话框数据
	enum { IDD = IDD_PALETTECHILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	CString m_sFindText;
	CString m_sDeleteName;
	CImageList m_imgList;
	std::vector<ZTCHAR*> m_vecAllItem;

	BOOL RefreshPlaette();
	BOOL DeleteItem(CString sDeleteName);
	BOOL InsertBlock(CString sName);

	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEditFind();
};

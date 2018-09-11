#pragma once
#include "resource.h"
#include "AllHead.h"

// CPaletteChildDlg �Ի���

class CPaletteChildDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CPaletteChildDlg)

public:
	CPaletteChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPaletteChildDlg();

// �Ի�������
	enum { IDD = IDD_PALETTECHILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

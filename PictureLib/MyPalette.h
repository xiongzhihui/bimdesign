#pragma once
#include "resource.h"
#include "PaletteChildDlg.h"

// CMyPalette �Ի���

class CMyPalette : public CAdUiPalette
{
	DECLARE_DYNAMIC(CMyPalette)

public:
	CMyPalette(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyPalette();

// �Ի�������
	enum { IDD = IDD_PALETTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	

	DECLARE_MESSAGE_MAP()
public:
	CPaletteChildDlg *m_childDlg;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL CreatChild();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	BOOL DestroyWindow();
};

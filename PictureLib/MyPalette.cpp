// MyPalette.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPalette.h"
#include "afxdialogex.h"


// CMyPalette 对话框

IMPLEMENT_DYNAMIC(CMyPalette, CAdUiPalette)

CMyPalette::CMyPalette(CWnd* pParent /*=NULL*/)
: CAdUiPalette()
{
	m_childDlg = NULL;
}

CMyPalette::~CMyPalette()
{
}

void CMyPalette::DoDataExchange(CDataExchange* pDX)
{
	CAdUiPalette::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyPalette, CAdUiPalette)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// int CMyPalette::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
// 	if (CAdUiPalette::OnCreate(lpCreateStruct)==-1)
// 	{
// 		return (-1);
// 	}
// 	CAcModuleResourceOverride rs;
// 	m_childDlg->Create(IDD_PALETTECHILD, this);
// 
// 	CRect rect;
// 	GetWindowRect(&rect);
// 	m_childDlg->ShowWindow(SW_SHOW);
// 	m_childDlg->MoveWindow(0, 0, rect.Width(), rect.Height(), TRUE);
// 	return (0);
// }

BOOL CMyPalette::CreatChild()
{
	CAcModuleResourceOverride rs;
	if (m_childDlg==NULL)
	{
		m_childDlg = new CPaletteChildDlg();
		m_childDlg->Create(IDD_PALETTECHILD, this);
		CRect rect;
		GetWindowRect(&rect);
		m_childDlg->ShowWindow(SW_SHOW);
		m_childDlg->MoveWindow(0, 0, rect.Width(), rect.Height(), TRUE);
	}	
	return TRUE;
}

int CMyPalette::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAdUiPalette::OnCreate(lpCreateStruct)==-1)
	{
		return -1;
	}
	CAcModuleResourceOverride rs;
	if (m_childDlg == NULL)
	{
		m_childDlg = new CPaletteChildDlg();
		m_childDlg->Create(IDD_PALETTECHILD, this);
		CRect rect;
		GetWindowRect(&rect);
		m_childDlg->ShowWindow(SW_SHOW);
		m_childDlg->MoveWindow(0, 0, rect.Width(), rect.Height(), TRUE);
	}
	return (0);
}

void CMyPalette::OnSize(UINT nType, int cx, int cy)
{
	CAdUiPalette::OnSize(nType, cx, cy);
	if (::IsWindow(m_childDlg->GetSafeHwnd()))
	{
		m_childDlg->MoveWindow(0, 0, cx, cy, TRUE);
	}
}

// CMyPalette 消息处理程序

//BOOL CMyPalette::DestroyWindow()
//{
//	// TODO:  在此添加专用代码和/或调用基类
//	if (m_childDlg!=NULL)
//	{
//		m_childDlg->DestroyWindow();
//		delete m_childDlg;
//		m_childDlg = NULL;
//	}
//	return CAdUiPalette::DestroyWindow();
//}

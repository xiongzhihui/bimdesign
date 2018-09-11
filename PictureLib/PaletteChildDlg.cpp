// PaletteChildDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PaletteChildDlg.h"
#include "afxdialogex.h"
#include "BlockJig.h"
#include <WinUser.h>
#include <algorithm>



// CPaletteChildDlg 对话框

IMPLEMENT_DYNAMIC(CPaletteChildDlg, CAcUiDialog)

CPaletteChildDlg::CPaletteChildDlg(CWnd* pParent /*=NULL*/)
: CAcUiDialog(CPaletteChildDlg::IDD, pParent)
{

}

CPaletteChildDlg::~CPaletteChildDlg()
{
}

void CPaletteChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT_FIND, m_sFindText);
}


BEGIN_MESSAGE_MAP(CPaletteChildDlg, CAcUiDialog)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CPaletteChildDlg::OnNMClickTree1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CPaletteChildDlg::OnNMRClickTree1)
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_EDIT_FIND, &CPaletteChildDlg::OnEnChangeEditFind)
END_MESSAGE_MAP()


// CPaletteChildDlg 消息处理程序
BOOL CPaletteChildDlg::RefreshPlaette()
{
	//获得配置文件中的分类
	CIniFile ini(GetIniPath());
	CString sTemp = _T("");
	ini.GetValueOfKey(_T("MAIN"), _T("分类"), sTemp);
	std::vector<CString> vecItem;
	CStringUtil::Split(sTemp, _T(","), vecItem);
	
	//清空树并且获取图库中的块名
	std::vector<ZTCHAR*> vecName;
	m_vecAllItem.clear();
	m_tree.DeleteAllItems();

	std::map<CString, HTREEITEM> nameAndItem;
	//添加分类并记录名称对应的item
	for (size_t i = 0; i<vecItem.size();i++)
	{
		HTREEITEM temp = m_tree.InsertItem(vecItem[i],0, 0);
		nameAndItem.insert(std::pair<CString, HTREEITEM>(vecItem[i], temp));
	}
	
	CString sPath = GetDwgPath();
	AcDbDatabase *pData = new AcDbDatabase(false);
	Acad::ErrorStatus es = pData->readDwgFile(sPath, SH_DENYNO);
	if (es != eOk)
	{
		delete pData;
		pData = NULL;
		return FALSE;
	}
	AcDbBlockTable *pBlkTbl = NULL;
	pData->getBlockTable(pBlkTbl, AcDb::kForRead);
	AcDbBlockTableIterator *itr = NULL;
	pBlkTbl->newIterator(itr);
	for (itr->start(); !itr->done(); itr->step())
	{
		AcDbBlockTableRecord *pRec = NULL;
		Acad::ErrorStatus es = itr->getRecord(pRec, AcDb::kForRead);
		if (es == eOk)
		{
			ZTCHAR* sName;
			pRec->getName(sName);
			if (sName[0] != '*')
			{
				vecName.push_back(sName);
				m_vecAllItem.push_back(sName);
			}

		}
		pRec->close();
	}
	pBlkTbl->close();
	delete pData;
	pData = NULL;

	std::sort(vecName.begin(), vecName.end(), [](const ZTCHAR* a, const ZTCHAR* b){return a[0] < b[0]; });
	for (size_t i = 0; i < vecName.size();i++)
	{
		std::vector<CString> vecTemp;
		CStringUtil::Split(vecName[i], _T("_"), vecTemp);
		if (vecTemp.size()==2)
		{
			CString sName = vecTemp[0];
			CString sFl = vecTemp[1];
			std::map<CString, HTREEITEM>::iterator it;
			it = nameAndItem.find(sFl);
			if (it!=nameAndItem.end())
			{
				m_tree.InsertItem(vecName[i],1,1, it->second, TVI_LAST);
			}
			
		}		
	}
	//图标插入
	HICON hicon[2];
	hicon[0]=AfxGetApp()->LoadIconW(IDI_ICON1);
	hicon[1] = AfxGetApp()->LoadIconW(IDI_ICON2);
	m_imgList.Create(16, 16, 0, 3, 3);
	m_imgList.Add(hicon[0]);
	m_imgList.Add(hicon[1]);
	m_tree.SetImageList(&m_imgList, TVSIL_NORMAL);
	return TRUE;
}

BOOL CPaletteChildDlg::DeleteItem(CString sDeleteName)
{
	CString sPath = GetDwgPath();
	AcDbDatabase *pData = new AcDbDatabase(false);
	Acad::ErrorStatus es = pData->readDwgFile(sPath, SH_DENYNO);
	if (es != eOk)
	{
		delete pData;
		pData = NULL;
		return FALSE;
	}
	AcDbBlockTable *pBlkTbl = NULL;
	pData->getBlockTable(pBlkTbl, AcDb::kForRead);
	AcDbBlockTableRecord *pRec = NULL;
	pBlkTbl->getAt(sDeleteName, pRec, AcDb::kForWrite);
	pBlkTbl->close();
	if (pRec!=NULL)
	{
		pRec->erase();
		pRec->close();
	}
	pData->saveAs(sPath);
	delete pData;
	pData = NULL;
	return TRUE;
}

BOOL CPaletteChildDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	RefreshPlaette();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CPaletteChildDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	
	CString sItem;
	CPoint point;
	GetCursorPos(&point);//获得鼠标点击的位置
	m_tree.ScreenToClient(&point);//转化为客户坐标
	UINT uFlags;

	HTREEITEM CurrentItem;
	CurrentItem = m_tree.HitTest(point, &uFlags);//获得当前点击节点的ITEM
	//获得该节点的父节点
	// HTREEITEM ParentItem=m_videotree.GetParentItem(CurrentItem);
	sItem = m_tree.GetItemText(CurrentItem);//获得节点值
	auto a=m_tree.GetParentItem(CurrentItem);
	if (a!=NULL)
	{
		acDocManager->lockDocument(acDocManager->curDocument());
		::SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		InsertBlock(sItem);
		acDocManager->unlockDocument(acDocManager->curDocument());
	}
}

BOOL CPaletteChildDlg::InsertBlock(CString sName)
{
	AcDbObjectId idBlock=CBlockUtil::CopyBlockDefFromOtherDwg(GetDwgPath(),sName);
	if (idBlock==AcDbObjectId::kNull)
	{
		return FALSE;
	}
/*	AcDbObjectId idInsert=CBlockUtil::InsertBlockRef(idBlock, AcGePoint3d::kOrigin);*/
	CBlockJig jig;
	jig.doIt(idBlock);
	return TRUE;
}

void CPaletteChildDlg::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	m_sDeleteName = _T("");
	CPoint pt,pt2;
	GetCursorPos(&pt);
	pt2 = pt;
	m_tree.ScreenToClient(&pt);
	UINT uFlags;
	HTREEITEM CurrentItem;
	CurrentItem = m_tree.HitTest(pt, &uFlags);//获得当前点击节点的ITEM
	m_sDeleteName = m_tree.GetItemText(CurrentItem);
	auto a = m_tree.GetParentItem(CurrentItem);
	if (a!=NULL)
	{
		CMenu menu;
		VERIFY(menu.CreatePopupMenu());
		menu.AppendMenuW(MF_STRING, 10001, _T("删除"));
		menu.TrackPopupMenu(TPM_LEFTBUTTON, pt2.x, pt2.y, this);
		menu.DestroyMenu();
	}
}

BOOL CPaletteChildDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT uID = (UINT)wParam;
	if (uID==10001)
	{
		acDocManager->lockDocument(acDocManager->curDocument());
		DeleteItem(m_sDeleteName);
		RefreshPlaette();
		acDocManager->unlockDocument(acDocManager->curDocument());
	}
	return CAcUiDialog::OnCommand(wParam, lParam);
}

void CPaletteChildDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
// 	CWnd *wnd=AfxGetMainWnd();
// 	wnd->GetSafeHwnd();
}


void CPaletteChildDlg::OnEnChangeEditFind()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CAcUiDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_sFindText==_T(""))
	{
		acDocManager->lockDocument(acDocManager->curDocument());
		RefreshPlaette();
		acDocManager->unlockDocument(acDocManager->curDocument());
		UpdateData(FALSE);
		return;
	}
	std::vector<TCHAR*> vecFind;
	for (size_t i = 0; i < m_vecAllItem.size();i++)
	{
		ZTCHAR * cTemp = m_vecAllItem[i];
		CString sTemp(cTemp);
		int iRec=sTemp.Find(m_sFindText);
		if (iRec!=-1)
		{
			vecFind.push_back(cTemp);
		}
	}
	m_tree.DeleteAllItems();
	auto item=m_tree.InsertItem(_T("查找到的图块"), 0, 0);
	for (size_t j = 0; j < vecFind.size(); j++)
	{
		m_tree.InsertItem(vecFind[j], item, 0);
	}
	m_tree.Expand(item,TVE_EXPAND);
	UpdateData(FALSE);
}

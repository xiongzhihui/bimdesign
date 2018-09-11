// PaletteChildDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PaletteChildDlg.h"
#include "afxdialogex.h"
#include "BlockJig.h"
#include <WinUser.h>
#include <algorithm>



// CPaletteChildDlg �Ի���

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


// CPaletteChildDlg ��Ϣ�������
BOOL CPaletteChildDlg::RefreshPlaette()
{
	//��������ļ��еķ���
	CIniFile ini(GetIniPath());
	CString sTemp = _T("");
	ini.GetValueOfKey(_T("MAIN"), _T("����"), sTemp);
	std::vector<CString> vecItem;
	CStringUtil::Split(sTemp, _T(","), vecItem);
	
	//��������һ�ȡͼ���еĿ���
	std::vector<ZTCHAR*> vecName;
	m_vecAllItem.clear();
	m_tree.DeleteAllItems();

	std::map<CString, HTREEITEM> nameAndItem;
	//��ӷ��ಢ��¼���ƶ�Ӧ��item
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
	//ͼ�����
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CPaletteChildDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	
	CString sItem;
	CPoint point;
	GetCursorPos(&point);//����������λ��
	m_tree.ScreenToClient(&point);//ת��Ϊ�ͻ�����
	UINT uFlags;

	HTREEITEM CurrentItem;
	CurrentItem = m_tree.HitTest(point, &uFlags);//��õ�ǰ����ڵ��ITEM
	//��øýڵ�ĸ��ڵ�
	// HTREEITEM ParentItem=m_videotree.GetParentItem(CurrentItem);
	sItem = m_tree.GetItemText(CurrentItem);//��ýڵ�ֵ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_sDeleteName = _T("");
	CPoint pt,pt2;
	GetCursorPos(&pt);
	pt2 = pt;
	m_tree.ScreenToClient(&pt);
	UINT uFlags;
	HTREEITEM CurrentItem;
	CurrentItem = m_tree.HitTest(pt, &uFlags);//��õ�ǰ����ڵ��ITEM
	m_sDeleteName = m_tree.GetItemText(CurrentItem);
	auto a = m_tree.GetParentItem(CurrentItem);
	if (a!=NULL)
	{
		CMenu menu;
		VERIFY(menu.CreatePopupMenu());
		menu.AppendMenuW(MF_STRING, 10001, _T("ɾ��"));
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CAcUiDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	auto item=m_tree.InsertItem(_T("���ҵ���ͼ��"), 0, 0);
	for (size_t j = 0; j < vecFind.size(); j++)
	{
		m_tree.InsertItem(vecFind[j], item, 0);
	}
	m_tree.Expand(item,TVE_EXPAND);
	UpdateData(FALSE);
}

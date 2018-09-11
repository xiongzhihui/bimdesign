// BimDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BimDlg.h"
#include "afxdialogex.h"


// CBimDlg 对话框

IMPLEMENT_DYNAMIC(CBimDlg, CAcUiDialog)

CBimDlg::CBimDlg(AcDbObjectId Entid,BOOL bMod,CWnd* pParent /*=NULL*/)
: CAcUiDialog(CBimDlg::IDD, pParent)
{
	m_idEnt = Entid;
	m_bMod = bMod;
}

CBimDlg::~CBimDlg()
{
}

void CBimDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_CAT, m_cmb_cat);
	DDX_Control(pDX, IDC_CMB_TYP, m_cmb_typ);
	DDX_Control(pDX, IDC_CMB_DES, m_cmb_des);
	DDX_Control(pDX, IDC_CMB_NOT, m_cmb_not);
	DDX_Text(pDX, IDC_EDIT_QTY, m_sQty);
	DDX_Text(pDX, IDC_EDIT_NUM, m_sNum);
	DDX_Text(pDX, IDC_EDIT_LOCATION, m_sLocation);
	DDX_Control(pDX, IDC_LIST_BIM, m_list_info);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
}

BEGIN_MESSAGE_MAP(CBimDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CBimDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CBimDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_MOD, &CBimDlg::OnBnClickedBtnMod)
	ON_BN_CLICKED(IDOK, &CBimDlg::OnBnClickedOk)
//	ON_CBN_SELENDCANCEL(IDC_CMB_CAT, &CBimDlg::OnCbnSelendcancelCmbCat)
	ON_CBN_SELCHANGE(IDC_CMB_TYP, &CBimDlg::OnCbnSelchangeCmbTyp)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BIM, &CBimDlg::OnNMDblclkListBim)
	ON_EN_KILLFOCUS(IDC_EDIT, &CBimDlg::OnEnKillfocusEdit)
//	ON_COMMAND(10000,&CBimDlg::OnClick)
	ON_CBN_SELCHANGE(IDC_CMB_CAT, &CBimDlg::OnCbnSelchangeCmbCat)
//	ON_EN_KILLFOCUS(20000,&CBimDlg::OnEdit)
END_MESSAGE_MAP()

// CBimDlg 消息处理程序

BOOL CBimDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

// 	m_btn.Create(_T("123"), NULL, CRect(0, 20, 50, 60), this, 10000);
// 	m_btn.ShowWindow(SW_SHOW);
// 
// 	m_edit2.Create(NULL, CRect(100, 20, 150, 60), this, 20000);
// 	m_edit2.ShowWindow(SW_SHOW);

	//初始化combobox
	CIniFile ini(GetIniPath());
	CString sTemp;
	std::vector<CString> vecTemp;
	ini.GetValueOfKey(_T("MAIN"), _T("category"), sTemp);
	CStringUtil::Split(sTemp, _T(","), vecTemp);
	SetCombobox(m_cmb_cat, vecTemp);
	vecTemp.clear();
	
	ini.GetValueOfKey(_T("MAIN"), _T("notes"), sTemp);
	CStringUtil::Split(sTemp, _T(","), vecTemp);
	SetCombobox(m_cmb_not, vecTemp);
	vecTemp.clear();
	//初始化 list
	m_list_info.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_info.InsertColumn(0, _T("Location"), LVCFMT_LEFT, 100);
	m_list_info.InsertColumn(1, _T("Catrgory"), LVCFMT_LEFT, 120);
	m_list_info.InsertColumn(2, _T("Type\\Size"), LVCFMT_LEFT, 120);
	m_list_info.InsertColumn(3, _T("Description"), LVCFMT_LEFT, 350);
	m_list_info.InsertColumn(4, _T("Notes"), LVCFMT_LEFT, 100);
	m_list_info.InsertColumn(5, _T("QTY"), LVCFMT_LEFT, 45);

	if (!m_bMod)
	{
		ini.GetValueOfKey(_T("MAIN"), _T("num"), m_sNum);
	}
	else
	{
		AcDbEntity *pEnt = NULL;
		acdbOpenAcDbEntity(pEnt, m_idEnt, AcDb::kForWrite);
		CString sAllItem; int iAllItem;
		CDwgDatabaseUtil::getXdata(_T("Num"), m_sNum, pEnt);
		CDwgDatabaseUtil::getXdata(_T("AllItemNum"), sAllItem, pEnt);
		iAllItem = _wtoi(sAllItem);
		for (int i = 0; i < iAllItem;i++)
		{
			CString sI; sI.Format(_T("%d"), i);
			CString sLoc,sCat, sTyp, sDes, sNot, sQty;
			
			CDwgDatabaseUtil::getXdata(_T("Loc") + sI, sLoc, pEnt);
			CDwgDatabaseUtil::getXdata(_T("Cat") + sI, sCat, pEnt);
			CDwgDatabaseUtil::getXdata(_T("Typ") + sI, sTyp, pEnt);
			CDwgDatabaseUtil::getXdata(_T("Des") + sI, sDes, pEnt);
			CDwgDatabaseUtil::getXdata(_T("Not") + sI, sNot, pEnt);
			CDwgDatabaseUtil::getXdata(_T("Qty") + sI, sQty, pEnt);

			m_list_info.InsertItem(i, _T(""));
			m_list_info.SetItemText(i, 0, sLoc);
			m_list_info.SetItemText(i, 1,sCat);
			m_list_info.SetItemText(i, 2,sTyp);
			m_list_info.SetItemText(i, 3,sDes);
			m_list_info.SetItemText(i, 4,sNot);
			m_list_info.SetItemText(i, 5,sQty);
		}
		pEnt->close();
	}
	UpdateData(FALSE);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CBimDlg::SetCombobox(CAcUiComboBox &m_cmb, std::vector<CString> vecString)
{
	m_cmb.ResetContent();
	for (size_t i = 0; i < vecString.size();i++)
	{
		m_cmb.AddString(vecString[i]);
	}
}

void CBimDlg::OnBnClickedBtnAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString sCat, sTyp, sDes, sNot;
	GetDlgItemText(IDC_CMB_CAT, sCat);
	GetDlgItemText(IDC_CMB_TYP, sTyp);
	GetDlgItemText(IDC_CMB_DES, sDes);
	GetDlgItemText(IDC_CMB_NOT, sNot);
	if (sCat==_T("")&&sTyp==_T("")&&sDes==_T("")&&sNot==_T("")&&m_sQty==_T(""))
	{
		AfxMessageBox(_T("所有项均为空不能添加"));
		return;
	}
	int i=m_list_info.GetItemCount();
	m_list_info.InsertItem(i, _T(""));
	m_list_info.SetItemText(i, 0, m_sLocation);
	m_list_info.SetItemText(i, 1, sCat);
	m_list_info.SetItemText(i, 2, sTyp);
	m_list_info.SetItemText(i, 3, sDes);
	m_list_info.SetItemText(i, 4, sNot);
	m_list_info.SetItemText(i, 5, m_sQty);
	UpdateData(FALSE);
	return;
}

void CBimDlg::OnBnClickedBtnDel()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int iCount = m_list_info.GetSelectionMark();
	if (iCount<0)
	{
		AfxMessageBox(_T("未选中需要删除的行"));
		return;
	}
	m_list_info.DeleteItem(iCount);
	m_list_info.SetSelectionMark(-1);
	UpdateData(FALSE);
}

void CBimDlg::OnBnClickedBtnMod()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int iCount = m_list_info.GetSelectionMark();
	if (iCount < 0)
	{
		AfxMessageBox(_T("未选中需要修改的行"));
		return;
	}
	CString sCat, sTyp, sDes, sNot;
	GetDlgItemText(IDC_CMB_CAT, sCat);
	GetDlgItemText(IDC_CMB_TYP, sTyp);
	GetDlgItemText(IDC_CMB_DES, sDes);
	GetDlgItemText(IDC_CMB_NOT, sNot);
	m_list_info.SetItemText(iCount, 0, m_sLocation);
	m_list_info.SetItemText(iCount, 1, sCat);
	m_list_info.SetItemText(iCount, 2, sTyp);
	m_list_info.SetItemText(iCount, 3, sDes);
	m_list_info.SetItemText(iCount, 4, sNot);
	m_list_info.SetItemText(iCount, 5, m_sQty);
	m_list_info.SetSelectionMark(-1);
	UpdateData(FALSE);
}

void CBimDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	AcDbBlockReference *pRef = NULL;
	acdbOpenObject(pRef, m_idEnt, AcDb::kForWrite);
	//修改序号 并写入扩展数据
	CBlockUtil::SetBlockRefAttribute(pRef, _T("NO"), m_sNum);
	CIniFile ini(GetIniPath());
	if (!m_bMod)
	{
		int iNum = _wtoi(m_sNum);
		iNum++;
		ini.SetValueOfKey(_T("MAIN"), _T("num"), iNum);
	}
	AcDbEntity *pEnt = static_cast<AcDbEntity*>(pRef);
	int iCount = m_list_info.GetItemCount();
	CString sCount; sCount.Format(_T("%d"), iCount);
	CDwgDatabaseUtil::SetXdata(_T("AllItemNum"), sCount, pEnt);
	CDwgDatabaseUtil::SetXdata(_T("Num"), m_sNum, pEnt);
	for (int i = 0; i < iCount;i++)
	{
		CString sI; sI.Format(_T("%d"), i);
		CString sLoc,sCat, sTyp, sDes, sNot,sQty;
		sLoc=m_list_info.GetItemText(i, 0);
		sCat=m_list_info.GetItemText(i, 1);
		sTyp=m_list_info.GetItemText(i, 2);
		sDes=m_list_info.GetItemText(i, 3);
		sNot=m_list_info.GetItemText(i, 4);
		sQty=m_list_info.GetItemText(i, 5);
		CDwgDatabaseUtil::SetXdata(_T("Loc") + sI, sLoc, pEnt);
		CDwgDatabaseUtil::SetXdata(_T("Cat") + sI, sCat, pEnt);
		CDwgDatabaseUtil::SetXdata(_T("Typ") + sI, sTyp, pEnt);
		CDwgDatabaseUtil::SetXdata(_T("Des") + sI, sDes, pEnt);
		CDwgDatabaseUtil::SetXdata(_T("Not") + sI, sNot, pEnt);
		CDwgDatabaseUtil::SetXdata(_T("Qty") + sI, sQty, pEnt);
	}
	pRef->close();
	CAcUiDialog::OnOK();
}

//void CBimDlg::OnCbnSelendcancelCmbCat()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	CString sItemText = _T("");
//	CString sTemp;
//	std::vector<CString> vecTemp;
//	m_cmb_cat.GetLBText(m_cmb_cat.GetCurSel(), sItemText);
//	CIniFile ini(GetIniPath());
//	ini.GetValueOfKey(_T("TYPESIZE"), sItemText, sTemp);
//	CStringUtil::Split(sTemp, _T(","), vecTemp);
//	SetCombobox(m_cmb_typ, vecTemp);
//	vecTemp.clear();
//	UpdateData(FALSE);
//}

void CBimDlg::OnCbnSelchangeCmbTyp()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString sItemText = _T("");
	CString sTemp;
	std::vector<CString> vecTemp;
	m_cmb_typ.GetLBText(m_cmb_typ.GetCurSel(), sItemText);
	CIniFile ini(GetIniPath());
	ini.GetValueOfKey(_T("DESCRIPTION"), sItemText, sTemp);
	CStringUtil::Split(sTemp, _T(","), vecTemp);
	SetCombobox(m_cmb_des, vecTemp);
	vecTemp.clear();
	UpdateData(FALSE);
}

void CBimDlg::OnNMDblclkListBim(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
// 	CPoint point;
// 	GetCursorPos(&point);
// 	m_list_info.ScreenToClient(&point);
// 	LVHITTESTINFO info;
// 	info.pt = point;
// 	int itsub = m_list_info.SubItemHitTest(&info);
// 	int row = info.iItem;
// 	int col = info.iSubItem;
// 	AfxMessageBox(_T("row=%d,col=%d"), row, col);
	DWORD   dwPos = GetMessagePos();
	CPoint   point(LOWORD(dwPos), HIWORD(dwPos));
	m_list_info.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	m_nItem = m_list_info.SubItemHitTest(&lvinfo);
	m_nSubItem = lvinfo.iSubItem;
	CRect rect;
	m_list_info.GetSubItemRect(lvinfo.iItem, m_nSubItem, LVIR_LABEL, rect);
	if (m_nItem != -1)
	{
// 		CString   strtemp;
// 		strtemp.Format(_T("单击的是第%d行第%d列 "), lvinfo.iItem, lvinfo.iSubItem);
// 		AfxMessageBox(strtemp);
		if (lvinfo.iSubItem==0)
		{
			ShowEdit(true, m_nItem, m_nSubItem, rect);
			UpdateWindow();
		}
	}
	*pResult = 0;
}

void CBimDlg::ShowEdit(bool bShow, int nItem, int nSubItem, CRect rc)
{
	if (m_Edit.m_hWnd==NULL)
	{
		m_Edit.Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN | WS_BORDER, CRect(0, 0, 0, 0), GetDlgItem(IDC_LIST_BIM), IDC_EDIT);
		m_Edit.ShowWindow(SW_HIDE);

		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		m_Edit.SetFont(&tpFont);
		tpFont.DeleteObject();
	}
	if (bShow == TRUE)
	{
		CString strItem = m_list_info.GetItemText(nItem,nSubItem);
		rc.top += 95;    
		rc.bottom += 98;
		rc.left += 8;
		rc.right += 13;
		m_Edit.MoveWindow(&rc);
		m_Edit.ShowWindow(SW_SHOW);
		m_Edit.SetWindowText(strItem);
		m_Edit.SetFocus();
		m_Edit.SetSel(-1);
	}
	else
		m_Edit.ShowWindow(SW_HIDE);
}

//void CBimDlg::OnEnkillFocus1()
//{
// 	UpdateData(TRUE);
// 	CString sTemp;
// 	m_Edit.GetWindowText(sTemp);
// 	if (sTemp!=_T(""))
// 	{
// 	 	m_list_info.SetItemText(m_nItem, m_nSubItem, sTemp);
// 		m_Edit.ShowWindow(SW_HIDE);
// 	}
// 	UpdateData(FALSE);
//}
// void CBimDlg::OnEnKillfocusEdit()
// {
// 	// TODO:  在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	CString sTemp;
// 	m_Edit.GetWindowText(sTemp);
// 	if (sTemp!=_T(""))
// 	{
// 		m_list_info.SetItemText(m_nItem, m_nSubItem, sTemp);
// 		m_Edit.ShowWindow(SW_HIDE);
// 	}
// 	UpdateData(FALSE);
// }

void CBimDlg::OnEnKillfocusEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString sTemp;
	m_Edit.GetWindowText(sTemp);
	if (sTemp != _T(""))
	{
		m_list_info.SetItemText(m_nItem, m_nSubItem, sTemp);
		m_Edit.ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);
}

// void CBimDlg::OnClick()
// {
// 	AfxMessageBox(_T("666"));
// }
// 
// void CBimDlg::OnEdit()
// {
// 	::PostMessage(GetDlgItem(IDC_LIST_BIM)->m_hWnd, on_d,WM_LBUTTONDOWN,NULL);
// }

void CBimDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类
	if (!m_bMod)
	{
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es;
		es = acdbOpenAcDbEntity(pEnt, m_idEnt, AcDb::kForWrite);
		if (es==eOk)
		{
			pEnt->erase();
			pEnt->close();
		}
	}
	CAcUiDialog::OnCancel();
}

void CBimDlg::OnCbnSelchangeCmbCat()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString sItemText = _T("");
	CString sTemp;
	std::vector<CString> vecTemp;
	m_cmb_cat.GetLBText(m_cmb_cat.GetCurSel(), sItemText);
	CIniFile ini(GetIniPath());
	ini.GetValueOfKey(_T("TYPESIZE"), sItemText, sTemp);
	CStringUtil::Split(sTemp, _T(","), vecTemp);
	SetCombobox(m_cmb_typ, vecTemp);
	vecTemp.clear();
	UpdateData(FALSE);
}

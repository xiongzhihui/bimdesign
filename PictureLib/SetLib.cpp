// SetLib.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetLib.h"
#include "afxdialogex.h"
#include "AllHead.h"
#include "MyPalette.h"


// CSetLib �Ի���

IMPLEMENT_DYNAMIC(CSetLib, CAcUiDialog)

CSetLib::CSetLib(CWnd* pParent /*=NULL*/)
: CAcUiDialog(CSetLib::IDD, pParent)
{

}

CSetLib::~CSetLib()
{
}

void CSetLib::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC, m_fcPreview);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Control(pDX, IDC_COMBO_FL, m_cmbFl);
}

BEGIN_MESSAGE_MAP(CSetLib, CAcUiDialog)
	ON_BN_CLICKED(IDC_BTN_SELECTPIC, &CSetLib::OnBnClickedBtnSelectpic)
	ON_BN_CLICKED(IDOK, &CSetLib::OnBnClickedOk)
END_MESSAGE_MAP()

// CSetLib ��Ϣ�������

void CSetLib::OnBnClickedBtnSelectpic()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_sName==_T(""))
	{
		AfxMessageBox(_T("������дͼԪ��"));
		return;
	}
	BeginEditorCommand();
	AcDbBlockTable *ptab=NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(ptab, AcDb::kForWrite);
	AcDbObjectId idTemp = AcDbObjectId::kNull;
	CString sFl;
	m_cmbFl.GetLBString(m_cmbFl.GetCurSel(), sFl);
	ptab->getAt(m_sName + _T("_") + sFl, idTemp);
	if (idTemp != AcDbObjectId::kNull)
	{
		AcDbBlockTableRecord*pRectemp=NULL;
		Acad::ErrorStatus es;
		es=acdbOpenObject(pRectemp, idTemp, AcDb::kForWrite);
		if (es!=eOk)
		{
		}
		else
		{
			pRectemp->erase();
			pRectemp->close();
		}		
	}
	ptab->close();
	m_idArr.removeAll();
	GetSelection(m_idArr);
	AcGePoint3d pt;
	CGetInputUtil::GetPoint(_T("�����"), pt);
	CreatBlockForAcdbobjectidarr(m_idArr, pt, m_sName + _T("_") + sFl);
	CompleteEditorCommand();
	m_fcPreview.Init(acdbHostApplicationServices()->workingDatabase(),m_sName + _T("_") + sFl);
}

extern CMyPalette *g_pPalett;

void CSetLib::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_sName==_T(""))
	{
		AfxMessageBox(_T("ͼ��������Ϊ��"));
		return;
	}
	AcDbBlockTable *pTab = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pTab, AcDb::kForRead);
	AcDbObjectId idTemp;
	CString sFl;
	m_cmbFl.GetLBString(m_cmbFl.GetCurSel(), sFl);
	pTab->getAt(m_sName + _T("_") + sFl, idTemp);
	if (idTemp==AcDbObjectId::kNull)
	{
		AfxMessageBox(_T("δ�ҵ���ӵ�ͼ��"));
		pTab->close();
		return;
	}
	pTab->close();

// 	AcDbDatabase* pTempDb = NULL;
// 	acdbHostApplicationServices()->workingDatabase()->wblock(pTempDb, idTemp);
// 	AcDbDatabase *pLibDb = new AcDbDatabase(FALSE);
// 	pLibDb->readDwgFile(GetDwgPath(), _SH_DENYNO);
// 	AcDbObjectId blockRefId = AcDbObjectId::kNull;
// 	pLibDb->insert(blockRefId, m_sName + _T("_") + sFl, pTempDb);
// 	pLibDb->saveAs(GetDwgPath());
// 	delete pLibDb;
// 	pLibDb = NULL;
// 	pTab->close();
// 	delete pLibDb;
// 	pLibDb = NULL;
// 	if (g_pPalett != NULL)
// 	{
// 	    g_pPalett->m_childDlg->RefreshPlaette();
// 	}
// 	CAcUiDialog::OnOK();

	bool b=CreatOtherDwgBlockForAcdbobjectidarr(GetDwgPath(),m_sName+_T("_")+sFl,idTemp);
	if (b)
	{
		if (g_pPalett != NULL)
		{
			g_pPalett->m_childDlg->RefreshPlaette();
		}
 		CAcUiDialog::OnOK();
	}
	return;
}

BOOL CSetLib::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	CIniFile ini(GetIniPath());
	CString sFl;
	std::vector<CString> vecFl;
	ini.GetValueOfKey(_T("MAIN"), _T("����"), sFl);
	CStringUtil::Split(sFl, _T(","),vecFl);
	for (size_t i = 0;i<vecFl.size();i++)
	{
		m_cmbFl.AddString(vecFl[i]);
	}
	m_cmbFl.SetCurSel(0);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

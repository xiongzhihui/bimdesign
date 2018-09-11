// FlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlDlg.h"
#include "afxdialogex.h"
#include "MyPalette.h"


// CFlDlg �Ի���

IMPLEMENT_DYNAMIC(CFlDlg, CAcUiDialog)

CFlDlg::CFlDlg(CWnd* pParent /*=NULL*/)
: CAcUiDialog(CFlDlg::IDD, pParent)
{

}

CFlDlg::~CFlDlg()
{
}

void CFlDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FL, m_list);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sFlName);
}


BEGIN_MESSAGE_MAP(CFlDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CFlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFlDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFlDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CFlDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFlDlg ��Ϣ�������


BOOL CFlDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	CIniFile ini(GetIniPath());
	CString sTemp;
	ini.GetValueOfKey(_T("MAIN"), (_T("����")), sTemp);
	std::vector<CString> vecFl;
	CStringUtil::Split(sTemp, _T(","), vecFl);
	m_list.InsertColumn(0, _T("����"), LVCFMT_LEFT, 150);
	for (size_t i = 0; i < vecFl.size();i++)
	{
		m_list.InsertItem(i, vecFl[i]);
	}
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CFlDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_sFlName==_T(""))
	{
		AfxMessageBox(_T("����������Ϊ��"));
		return;
	}
	int iCount=m_list.GetItemCount();
	for (int i = 0; i < iCount;i++)
	{
		CString sName = m_list.GetItemText(i, 0);
		if (sName==m_sFlName)
		{
			AfxMessageBox(_T("�������Ѵ���,�����ظ����."));
			return;
		}
	}
	m_list.InsertItem(iCount, m_sFlName);
	UpdateData(FALSE);
}


void CFlDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int i = -1;
	i=m_list.GetSelectionMark();
	if (i<0)
	{
		AfxMessageBox(_T("δѡ����Ҫɾ������"));
	}
	else
	{
		CString sTemp;
		CString sName = m_list.GetItemText(i, 0);
		sTemp.Format(_T("ȷ��Ҫɾ������:%s��ɾ�����������е�ͼԪ��������ʾ�������"), sName);
		int iRet = AfxMessageBox(sTemp, MB_YESNO);
		if (iRet==IDYES)
		{
			m_list.DeleteItem(i);
			m_list.SetSelectionMark(-1);
		}
	}
	UpdateData(FALSE);
}


void CFlDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_sFlName == _T(""))
	{
		AfxMessageBox(_T("δ��д�޸ĺ������"));
		return;
	}
	int i = -1;
	i = m_list.GetSelectionMark();
	if (i < 0)
	{
		AfxMessageBox(_T("δѡ����Ҫ�޸ĵ���"));
	}
	else
	{
		CString sTemp;
		CString sYYName = m_list.GetItemText(i, 0);
		sTemp.Format(_T("ȷ��Ҫ�޸ķ���:%s���޸ķ��������е�ͼԪ����׺�����޸�."), sYYName);
		int iRet = AfxMessageBox(sTemp, MB_YESNO);
		if (iRet == IDYES)
		{
			CString sPath = GetDwgPath();
			AcDbDatabase *pData = new AcDbDatabase(false);
			Acad::ErrorStatus es = pData->readDwgFile(sPath, SH_DENYNO);
			if (es != eOk)
			{
				delete pData;
				pData = NULL;
				return;
			}
			AcDbBlockTable *pBlkTbl = NULL;
			pData->getBlockTable(pBlkTbl, AcDb::kForRead);
			AcDbBlockTableIterator *itr = NULL;
			pBlkTbl->newIterator(itr);
			for (itr->start(); !itr->done(); itr->step())
			{
				AcDbBlockTableRecord *pRec = NULL;
				Acad::ErrorStatus es = itr->getRecord(pRec, AcDb::kForWrite);
				if (es == eOk)
				{
					ZTCHAR* sName;
					pRec->getName(sName);
					if (sName[0] != '*')
					{
						std::vector<CString> vecTemp;
						CStringUtil::Split(sName, _T("_"), vecTemp);
						if (vecTemp.size()==2)
						{
							if (vecTemp[1]==sYYName)
							{
								pRec->setName(vecTemp[0] + _T("_") + m_sFlName);
							}
						}
					}

				}
				pRec->close();
			}
			pBlkTbl->close();
			pData->saveAs(sPath);
			delete pData;
			pData = NULL;
			AfxMessageBox(_T("�޸����"));
			m_list.SetItemText(i, 0, m_sFlName);
		}
	}
	UpdateData(FALSE);
}

extern CMyPalette *g_pPalett;

void CFlDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int iCount=m_list.GetItemCount();
	CString sTemp = _T("");
	for (int i = 0; i < iCount;i++)
	{
		CString s1=m_list.GetItemText(i, 0);
		if (i!=iCount-1)
		{
			s1 += _T(",");
		}
		sTemp += s1;
	}
	CIniFile ini(GetIniPath());
	ini.SetValueOfKey(_T("MAIN"), _T("����"), sTemp);
	if (g_pPalett != NULL)
	{
		g_pPalett->m_childDlg->RefreshPlaette();
	}
	CAcUiDialog::OnOK();
}

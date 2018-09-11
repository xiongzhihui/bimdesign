#pragma once
#include "Resource.h"
#include "fcGsPreviewCtrl.h"

// CSetLib �Ի���

class CSetLib : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSetLib)

public:
	CSetLib(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetLib();

// �Ի�������
	enum { IDD = IDD_SETLIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	fcGsPreviewCtrl m_fcPreview;
	CAcUiComboBox m_cmbFl;
	AcDbObjectIdArray m_idArr;
	CString m_sName;
	afx_msg void OnBnClickedBtnSelectpic();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};

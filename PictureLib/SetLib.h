#pragma once
#include "Resource.h"
#include "fcGsPreviewCtrl.h"

// CSetLib 对话框

class CSetLib : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSetLib)

public:
	CSetLib(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetLib();

// 对话框数据
	enum { IDD = IDD_SETLIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

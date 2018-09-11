#pragma once
#include "resource.h"
#include "AllHead.h"

// CCreatGuard 对话框

class CCreatGuard : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCreatGuard)

public:
	CCreatGuard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreatGuard();

// 对话框数据
	enum { IDD = IDD_DLG_GUARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sLen;
	CString m_sNum;
	BOOL WritePoint(const AcGePoint3d &ptS,const AcGePoint3d &ptE);
	BOOL ReadPoint(AcGePoint3d &ptS,AcGePoint3d &ptE);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnDrawmid();
//	afx_msg void OnBnClickedBtnSelect();
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
};

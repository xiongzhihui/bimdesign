#pragma once
#include "Resource.h"
#include "AllHead.h"

// CBimDlg 对话框

class CBimDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CBimDlg)

public:
	CBimDlg(AcDbObjectId Entid,BOOL bMod,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBimDlg();

// 对话框数据
	enum { IDD = IDD_DLG_BIMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CAcUiComboBox m_cmb_cat;
	CAcUiComboBox m_cmb_typ;
	CAcUiComboBox m_cmb_des;
	CAcUiComboBox m_cmb_not;
	CString m_sQty;
	CString m_sNum;
	CString m_sLocation;
	CListCtrl m_list_info;
	AcDbObjectId m_idEnt;
	BOOL m_bMod;
	//用于修改的edit
	CEdit m_Edit;
	int m_nItem;
	int m_nSubItem;

// 	CEdit m_edit2;
//  CButton m_btn;
public:
	virtual BOOL OnInitDialog();
	void SetCombobox(CAcUiComboBox &m_cmb, std::vector<CString> vecString);
	void ShowEdit(bool bShow,int nItem,int nSubItem,CRect rc);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnMod();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnEditchangeCmbCat();
//	afx_msg void OnCbnSelendcancelCmbCat();
	afx_msg void OnCbnSelchangeCmbTyp();
	afx_msg void OnNMDblclkListBim(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit();
// 	afx_msg void OnClick();
// 	afx_msg void OnEdit();
	virtual void OnCancel();
	afx_msg void OnCbnSelchangeCmbCat();
};

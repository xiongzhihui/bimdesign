#pragma once
#include "resource.h"

// CPaletteSet 对话框

class CPaletteSet : public CAdUiPaletteSet
{
	DECLARE_DYNAMIC(CPaletteSet)

public:
	CPaletteSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPaletteSet();

// 对话框数据
	enum { IDD = IDD_PALETTESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void GetMinimumSize(CSize& size)
	{
		size.cx = 100;
		size.cy = 100;
	}
	void GetMaximumSize(CSize& size)
	{
		size.cx = 400;
		size.cy = 400;
	}
};

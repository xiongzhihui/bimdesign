#pragma once
#include "resource.h"

// CPaletteSet �Ի���

class CPaletteSet : public CAdUiPaletteSet
{
	DECLARE_DYNAMIC(CPaletteSet)

public:
	CPaletteSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPaletteSet();

// �Ի�������
	enum { IDD = IDD_PALETTESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

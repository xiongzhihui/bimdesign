#pragma once
#include "D:\Work\BimDesign\ZRXSDK\inc\zAcDblClkEdit.h"
#include "AllHead.h"
#include "BimDlg.h"
class CDoubleClickEdit :
	public AcDbDoubleClickEdit
{
public:
	CDoubleClickEdit();
	virtual~CDoubleClickEdit();

	void startEdit(AcDbEntity *pEnt, AcGePoint3d pt);
	void finishEdit(void);
};


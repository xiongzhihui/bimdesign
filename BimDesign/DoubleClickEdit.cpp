#include "StdAfx.h"
#include "DoubleClickEdit.h"


CDoubleClickEdit::CDoubleClickEdit()
{
}


CDoubleClickEdit::~CDoubleClickEdit()
{
}

void CDoubleClickEdit::startEdit(AcDbEntity *pEnt, AcGePoint3d pt)
{
	AcApDocument *pDoc = acDocManager->curDocument();
	acDocManager->lockDocument(pDoc);

	CString sTemp=_T("");
	CDwgDatabaseUtil::getXdata(_T("AllItemNum"), sTemp, pEnt);
	if (sTemp==_T(""))
	{
		AfxMessageBox(_T("选中的实体不含属性"));
		pEnt->close();
		acedSSSetFirst(NULL, NULL);
		acDocManager->unlockDocument(pDoc);
		return;
	}
	AcDbObjectId id = pEnt->id();
	pEnt->close();
	acedSSSetFirst(NULL, NULL);
	CAcModuleResourceOverride rs;
	CBimDlg dlg(id,TRUE);
	dlg.DoModal();
	acDocManager->unlockDocument(pDoc);
	return;
}

void CDoubleClickEdit::finishEdit(void)
{

}
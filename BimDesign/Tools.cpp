#include "stdafx.h"
#include "Tools.h"
#include "BlockUtil.h"
#include "IniFile.h"
#include "DwgDatabaseUtil.h"

#define PI 3.141592654


bool GetSelection(AcDbObjectIdArray& adoia, CString sPrompt)
{
	// Check the pickfirst
	adoia.removeAll();
	ads_name anSs;
	if (acedSSGet(_T("I"), NULL, NULL, NULL, anSs) == RTNORM)
	{
		// Get objects
		GetObjectIdArray(anSs, adoia);
		if (adoia.length() > 0)
		{
			// Clear pickfirst
			int ret = acedSSSetFirst(NULL, NULL);
			return true;
		}
	}

	TCHAR*   pcaStringPointers[2];
	TCHAR    caPromptSelecting[2048],
		caPromptRemoving[2048];
	int      iReturn;

	// Intialization. Empty the selectionset. Very important
	ads_ssadd(NULL, NULL, anSs);

	if (!sPrompt.IsEmpty()) {
		// Copy prompt strings to TCHAR array
		_tcscpy(caPromptSelecting, sPrompt);
		_tcscpy(caPromptRemoving, sPrompt);

		// Set the address pointers for in the array to give as parameter
		pcaStringPointers[0] = caPromptSelecting;
		pcaStringPointers[1] = caPromptRemoving;

		// Start selection
		iReturn = acedSSGet(_T(":$"), pcaStringPointers, NULL, NULL, anSs);

		// User pressed escape
		if (iReturn == RTCAN)
			return false;
	}
	else {
		// Start selection
		iReturn = acedSSGet(NULL, NULL, NULL, NULL, anSs);
		// User pressed escape
		if (iReturn == RTCAN)
			return false;
	}

	// Convert anSs to adoia
	GetObjectIdArray(anSs, adoia);

	return true;
}

bool GetObjectIdArray(ads_name anSs, AcDbObjectIdArray& adoiaObjects)
{
	long         lLength = 0L;
	ads_name     anObject;
	AcDbObjectId adoiObject;

	// Empty the idarray
	adoiaObjects.removeAll();

	// Get length of idarray
	ads_sslength(anSs, &lLength);

	// For every objectid
	for (int i = 0; i < lLength; i++) {
		// Get objectid from idarray
		ads_ssname(anSs, i, anObject);

		// Convert objectid to AcDbObjectId
		if (acdbGetObjectId(adoiObject, anObject) != Acad::eOk) {
			return false;
		}

		// Check if AcDbObjectId is valid
		if (!adoiObject.isValid()) {
			return false;
		}

		// Append to objectid array
		adoiaObjects.append(adoiObject);
	}

	return true;
}

CString GetDwgPath()
{
	CString sPath = CAppDirectoryUtil::GetParentDirectory();
	sPath = sPath + _T("\\Support\\templateBimBlock.dwg");
	return sPath;
}

CString GetIniPath()
{
	CString sPath = CAppDirectoryUtil::GetParentDirectory();
	sPath = sPath + _T("\\Support\\Bim.ini");
	return sPath;
}

CString GetExcPath()
{
	CString sPath = CAppDirectoryUtil::GetParentDirectory();
	sPath = sPath + _T("\\Support\\ExcelTemplate.xlsx");
	return sPath;
}

CString GetSaveExcPath()
{
	CString sPath = CAppDirectoryUtil::GetParentDirectory();
	sPath = sPath + _T("\\OutExcel\\");
	return sPath;
}

AcDbObjectId CreatBlockForAcdbobjectidarr(AcDbObjectIdArray ptarr, AcGePoint3d pt, CString sName)
{
	//创建一个块参照在当前数据库中
	AcDbBlockTable *pBlkTab = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTab, AcDb::kForWrite);
	AcDbBlockTableRecord *pBlkTabRcd;
	pBlkTabRcd = new AcDbBlockTableRecord();
	pBlkTabRcd->setName(sName);
	AcDbObjectId blockId;
	pBlkTab->add(blockId, pBlkTabRcd);
	pBlkTabRcd->setOrigin(pt);//设置块的插入点 
	pBlkTab->close();
	pBlkTabRcd->close();//关闭后获得这个块的id 

	AcDbIdMapping         adimIdMap;
	AcApDocument* pDoc = acDocManager->curDocument();
	acDocManager->lockDocument(pDoc, AcAp::kWrite);//锁定当前文档
	acdbHostApplicationServices()->workingDatabase()->deepCloneObjects(ptarr, blockId, adimIdMap);//克隆当前选中的实体到块中
	acDocManager->unlockDocument(pDoc);//解锁文档

	return blockId;
}

bool CreatOtherDwgBlockForAcdbobjectidarr(const TCHAR *fileName, const TCHAR *blkDefName, AcDbObjectId Tid)
{
	// 使用_SH_DENYNO参数打开图形(只读打开)，允许其它用户读写该文件
	AcDbDatabase* pSourceDwg = new AcDbDatabase(false);
	Acad::ErrorStatus es = pSourceDwg->readDwgFile(fileName, _SH_DENYNO);
	if (es != Acad::eOk)
	{
		delete pSourceDwg;
		pSourceDwg = NULL;

		acutPrintf(TEXT("\n读入dwg图形错误, 图形名称: %s"), fileName);
		return false;
	}
	// 打开外部图形数据库的块表，寻找给定名称的块表记录
	AcDbBlockTable* pBlkTable = NULL;
	es = pSourceDwg->getBlockTable(pBlkTable, AcDb::kForWrite);
	AcDbObjectId idTemp = AcDbObjectId::kNull;
	pBlkTable->getAt(blkDefName, idTemp);
	if (idTemp != AcDbObjectId::kNull)
	{
		int iRet=AfxMessageBox(_T("\n外部图库文件中已有指定名称的块,是否删除"), MB_YESNO);
		if (iRet==IDYES)
		{
			AcDbBlockTableRecord *pRec = NULL;
			Acad::ErrorStatus es=acdbOpenObject(pRec, idTemp, AcDb::kForWrite);
			if (pRec!=NULL)
			{
				pRec->erase();
				pRec->close();
			}
		}
		else
		{
			pBlkTable->close();
			delete pSourceDwg;
			pSourceDwg = NULL;
			return false;
		}		
	}
	AcDbObjectId blockId=NULL;
	AcDbBlockTableRecord *pTempRec = new AcDbBlockTableRecord();
	pTempRec->setName(blkDefName);
	pBlkTable->add(blockId, pTempRec);

// 	AcDbObjectId idTest = NULL;
// 	pBlkTable->getAt(_T("*Model_Space"),idTest);

	pBlkTable->close();
	pTempRec->close();
	AcDbIdMapping         adimIdMap;
	AcApDocument* pDoc = acDocManager->curDocument();
	acDocManager->lockDocument(pDoc, AcAp::kWrite);//锁定当前文档
	AcDbObjectIdArray ptarr;
	ptarr.append(Tid);
	Acad::ErrorStatus es2=acdbHostApplicationServices()->workingDatabase()->wblockCloneObjects(ptarr,blockId, adimIdMap, AcDb::kDrcReplace);
	acDocManager->unlockDocument(pDoc);//解锁文档

	pSourceDwg->saveAs(fileName);
	delete pSourceDwg;
	pSourceDwg = NULL;
	return true;
}

bool cmp(const AcGePoint3d &p1, const AcGePoint3d &p2)
{
	if (p1.y > p2.y)
	{
		return true;
	}
	else if (p1.y<p2.y)
	{
		return false;
	}
	else
	{
		if (p1.x<p2.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
};
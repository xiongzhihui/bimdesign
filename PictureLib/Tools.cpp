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
	sPath = sPath + _T("\\Support\\templatePicLib.dwg");
	return sPath;
}

CString GetIniPath()
{
	CString sPath = CAppDirectoryUtil::GetParentDirectory();
	sPath = sPath + _T("\\Support\\PicLib.ini");
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

bool CreatOtherDwgBlockForAcdbobjectidarr(const TCHAR *fileName, const TCHAR *blkDefName, AcDbObjectId &Tid)
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
	AcDbBlockTableRecord *pTempRec = new AcDbBlockTableRecord;
	es=pTempRec->setName(blkDefName);
	es=pBlkTable->add(blockId, pTempRec);

	pBlkTable->close();
	pTempRec->close();

	AcDbIdMapping         adimIdMap;

	AcApDocument* pDoc = acDocManager->curDocument();
	acDocManager->lockDocument(pDoc, AcAp::kWrite);//锁定当前文档
	AcDbObjectIdArray ptarr;
	ptarr.append(Tid);
	Acad::ErrorStatus es2 = acdbHostApplicationServices()->workingDatabase()->wblockCloneObjects(ptarr, blockId, adimIdMap, AcDb::kDrcReplace);
	acDocManager->unlockDocument(pDoc);//解锁文档




	pSourceDwg->saveAs(fileName);
	delete pSourceDwg;
	pSourceDwg = NULL;
	return true;
}

int GetPointAtPolylinePart(const AcDbPolyline *pPoly, const AcGePoint3d pt)
{
	double dDist;
	pPoly->getDistAtPoint(pt, dDist);
	for (int i = 1; i < pPoly->numVerts();i++)
	{
		double dTemp;
		AcGePoint3d ptTemp;
		pPoly->getPointAt(i,ptTemp);
		pPoly->getDistAtPoint(ptTemp,dTemp);
		if (dTemp>=dDist)
		{
			return i;
		}
	}
	return 0;
}

double GetPolylineLeftX(const AcDbPolyline *pPoly, BOOL bLeft)
{
	double dRet = 0;
	AcGePoint3d ptStr;
	pPoly->getStartPoint(ptStr);
	dRet = ptStr.x;
	for (int i = 0; i < pPoly->numVerts();i++)
	{
		AcGePoint3d ptTemp;
		pPoly->getPointAt(i, ptTemp);
		if (bLeft)
		{
			if (ptTemp.x<dRet)
			{
				dRet = ptTemp.x;
			}
		}
		else
		{
			if (ptTemp.x>dRet)
			{
				dRet = ptTemp.x;
			}
		}
	}
	return dRet;
}

// std::vector<AcGePoint3d> GetDrawCirPoint(const AcDbPolyline *pPoly, AcGePoint3d ptStr, AcGePoint3d ptEnd,double dLen,int iNum,double dAllLen)
// {
// 	std::vector<AcGePoint3d> vecPt;
// 	int iLen = pPoly->numVerts();
// 	int iStr = GetPointAtPolylinePart(pPoly, ptStr);
// 	int iEnt = GetPointAtPolylinePart(pPoly, ptEnd);
// 
// 	for (int i = 0; i < iNum;i++)
// 	{
// 		double dTemp;
// 		AcGePoint3d pStemp;		
// 		pPoly->getDistAtPoint(ptStr, dTemp);
// 		dTemp += dLen*(i+1);
// 		if (dTemp > dAllLen)
// 		{
// 			dTemp = dTemp - dAllLen;
// 		}
// 		pPoly->getPointAtDist(dTemp, pStemp);
// 		vecPt.push_back(pStemp);
// 	}
// 	return vecPt;
// }

std::vector<AcGePoint3d> GetDrawCirPoint(const AcDbPolyline *pPoly, AcGePoint3d ptStr, AcGePoint3d ptEnd, double dLen, int iNum, double dAllLen)
{
	double dDrawLen;
	double dNum=iNum;
	if (GetPointAtPolylinePart(pPoly,ptStr)>GetPointAtPolylinePart(pPoly,ptEnd))
	{
		double dTemp,dTemp2;
		pPoly->getDistAtPoint(ptEnd, dTemp);
		pPoly->getDistAtPoint(ptStr, dTemp2);
		dDrawLen = dTemp + dAllLen - dTemp2;
	}
	else
	{
		double dTemp, dTemp2;
		pPoly->getDistAtPoint(ptEnd, dTemp);
		pPoly->getDistAtPoint(ptStr, dTemp2);
		dDrawLen = dTemp - dTemp2;
	}
	dLen=dDrawLen / (dNum+2.8);
	AcGePoint3d ptNext, ptLast;
	std::vector<AcGePoint3d> vecPt;
	for (int i = 0; i < iNum; i++)
	{
		if (i == 0)
		{
			AcGePoint3dArray ptArr,ptArr2;
			AcDbCircle *pCircle = new AcDbCircle(ptStr, AcGeVector3d(0, 0, 1), dLen);
			pPoly->intersectWith(pCircle, AcDb::kOnBothOperands, ptArr);
			ptArr2 = ptArr;
			pCircle->close();
			delete pCircle;
			int iNum = ptArr.length();
			for (int j = 0; j < iNum;j++)
			{
				AcGePoint3d ptTest = ptArr2[j];
				double dTemp=ptTest.distanceTo(ptStr);
				if (dTemp>dLen+0.1)
				{
					ptArr.remove(ptTest);
				}
			}		
			AcGePoint3d pt=ptArr[0];
			AcGePoint3d pt1=ptArr[1];
			double d, d1, d2;
			pPoly->getDistAtPoint(pt, d);
			pPoly->getDistAtPoint(pt1, d1);
			pPoly->getDistAtPoint(ptStr, d2);
			if (d2 + dLen >= dAllLen)
			{
				if (d > d1)
				{
					ptNext = pt1;
				}
				else
				{
					ptNext = pt;
				}
			}
			else
			{
				if (d > d1)
				{
					ptNext = pt;
				}
				else
				{
					ptNext = pt1;
				}
			}
			ptLast = ptStr;
			vecPt.push_back(ptNext);
		}
		else
		{
			AcGePoint3dArray ptArr,ptArr2;
			AcDbCircle *pCircle = new AcDbCircle(ptNext, AcGeVector3d(0, 0, 1), dLen);
			pPoly->intersectWith(pCircle, AcDb::kOnBothOperands, ptArr);
			pCircle->close();
			delete pCircle;
			ptArr2 = ptArr;
			int iNum = ptArr.length();
			for (int j = 0; j < iNum; j++)
			{
				AcGePoint3d ptTest = ptArr2[j];
				double dTemp = ptTest.distanceTo(ptNext);
				if (dTemp > dLen+0.1)
				{
					ptArr.remove(ptTest);
				}
			}
			AcGePoint3d pt = ptArr[0];
			AcGePoint3d pt1 = ptArr[1];
			if (pt.distanceTo(ptLast)<1)
			{
				ptLast = ptNext;
				ptNext = pt1;
			}
			if (pt1.distanceTo(ptLast)<1)
			{
				ptLast = ptNext;
				ptNext = pt;
			}		
			vecPt.push_back(ptNext);
		}
	}
	return vecPt;
}

std::vector<AcGePoint3d> GetDrawCirPoint(const AcDbPolyline *pPoly, AcGePoint3d ptStr, AcGePoint3d ptEnd, double dLen, int iNum)
{
	std::vector<AcGePoint3d> vecPt;
	vecPt.push_back(ptStr);
	AcGePoint3d ptLast;
	for (int i = 0; i < iNum-1;i++)
	{
		AcGePoint3dArray ptarr;
		AcDbCircle *pCir = new AcDbCircle(ptStr, AcGeVector3d(0, 0, 1), dLen);
		pPoly->intersectWith(pCir, AcDb::kOnBothOperands, ptarr);
		pCir->close();
		delete pCir;
		int iPtNum = ptarr.length();
		for (int j = 0; j < iPtNum; j++)
		{
			AcGePoint3d ptTemp = ptarr[j];
			double dTemp = ptTemp.distanceTo(ptStr);
			if (dTemp>dLen+1)
			{
				ptarr.remove(ptTemp);
			}
		}
		AcGePoint3d p1, p2;
		p1 = ptarr[0];
		p2 = ptarr[1];
		if (i==0)
		{
			if (p1.x < p2.x)
			{
				vecPt.push_back(p1);
				ptLast = ptStr;
				ptStr = p1;
			}
			else
			{
				vecPt.push_back(p2);
				ptLast = ptStr;
				ptStr = p2;
			}
		}
		else
		{
			if (p1.distanceTo(ptLast)<1)
			{
				vecPt.push_back(p2);
				ptLast = ptStr;
				ptStr=p2;
			}
			else
			{
				vecPt.push_back(p1);
				ptLast = ptStr;
				ptStr = p1;
			}
		}		
	}
	return vecPt;
}

std::vector<AcGePoint3d> GetDrawCirMidPoint(const AcDbPolyline *pPoly, AcGePoint3d ptLeft,double dLen, int iNum)
{
	std::vector<AcGePoint3d> vecPt;
	vecPt.push_back(ptLeft);
	for (int i = 0; i < iNum;i++)
	{
		AcGePoint3dArray ptArr, ptArr2;
		AcDbCircle *pCircle = new AcDbCircle(ptLeft, AcGeVector3d(0, 0, 1), dLen);
		pPoly->intersectWith(pCircle, AcDb::kOnBothOperands, ptArr);
		pCircle->close();
		delete pCircle;
		ptArr2 = ptArr;

		int iNum2 = ptArr.length();
		for (int j = 0; j < iNum2; j++)
		{
			AcGePoint3d ptTest = ptArr2[j];
			double dTemp = ptTest.distanceTo(ptLeft);
			if (dTemp > dLen + 0.1)
			{
				ptArr.remove(ptTest);
			}
		}
		AcGePoint3d pt = ptArr[0];
		AcGePoint3d pt1 = ptArr[1];
		if (pt.x>pt1.x)
		{
			vecPt.push_back(pt);
			ptLeft = pt;
		}
		else
		{
			vecPt.push_back(pt1);
			ptLeft = pt1;
		}
	}
    return vecPt;
}
#pragma once
#include "AppDirectoryUtil.h"

bool GetSelection(AcDbObjectIdArray& adoia, CString sPrompt = _T(""));

bool GetObjectIdArray(ads_name anSs, AcDbObjectIdArray& adoiaObjects);

CString GetDwgPath();

CString GetIniPath();

AcDbObjectId CreatBlockForAcdbobjectidarr(AcDbObjectIdArray ptarr, AcGePoint3d pt, CString sName);

bool CreatOtherDwgBlockForAcdbobjectidarr(const TCHAR *fileName, const TCHAR *blkDefName, AcDbObjectId &Tid);

int GetPointAtPolylinePart(const AcDbPolyline *pPoly, const AcGePoint3d pt);

double GetPolylineLeftX(const AcDbPolyline *pPoly,BOOL bLeft);

std::vector<AcGePoint3d> GetDrawCirPoint(const AcDbPolyline *pPoly, AcGePoint3d ptStr, AcGePoint3d ptEnd, double dLen, int iNum,double dAllLen);

std::vector<AcGePoint3d> GetDrawCirPoint(const AcDbPolyline *pPoly, AcGePoint3d ptStr, AcGePoint3d ptEnd, double dLen, int iNum);

std::vector<AcGePoint3d> GetDrawCirMidPoint(const AcDbPolyline *pPoly, AcGePoint3d ptLeft, double dLen,int iNum);



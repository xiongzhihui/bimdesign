#pragma once
#include "AppDirectoryUtil.h"

bool GetSelection(AcDbObjectIdArray& adoia, CString sPrompt = _T(""));

bool GetObjectIdArray(ads_name anSs, AcDbObjectIdArray& adoiaObjects);

CString GetDwgPath();

CString GetIniPath();

CString GetExcPath();

CString GetSaveExcPath();

AcDbObjectId CreatBlockForAcdbobjectidarr(AcDbObjectIdArray ptarr, AcGePoint3d pt, CString sName);

bool CreatOtherDwgBlockForAcdbobjectidarr(const TCHAR *fileName, const TCHAR *blkDefName, AcDbObjectId Tid);

bool cmp(const AcGePoint3d &p1, const AcGePoint3d &p2);


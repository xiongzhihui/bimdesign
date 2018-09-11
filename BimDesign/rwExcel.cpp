#include "Stdafx.h"
#include "rwExcel.h"
#include "assert.h"
//#include "GlobalMFC.h"

BOOL AfxVariantConvert(CString& val, _variant_t vt)
{
	vt.ChangeType(VT_BSTR, NULL);
	if (vt.vt == VT_BSTR)
	{
		val = (LPCTSTR)vt.bstrVal;
		//val = (LPCTSTR)vt.pdispVal;
		return TRUE;
	}
	else
	{
		val = _T("");
		return FALSE;
	}
}

COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
cExcel::cExcel()
{
	pApp = NULL;
	pBooks = NULL;
	pBook = NULL;
	pSheets = NULL;
	pSheet = NULL;
	pRange = NULL;
	pInterior = NULL;
	pFont = NULL;
}
cExcel::~cExcel()
{
}

bool cExcel::StartApp(CString strMoldName)
{
	CoInitialize(NULL);

	if (!pApp.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("创建Excel服务失败!"));
		exit(1);
		return false;
	}
	pApp.SetVisible(false);
	//pApp.SetVisible(false);//VARIANT_TRUE  
	pBooks = pApp.GetWorkbooks();

	//利用模板文件建立新文档
	if (strMoldName.GetLength() > 0)
	{
		pBooks.AttachDispatch(pApp.GetWorkbooks(), true);
		pBook.AttachDispatch(pBooks.Add(_variant_t(strMoldName)));   //设置模板文件
	}


	return true;
}

void cExcel::CreateWorkBook(void)
{
	assert(pBooks != NULL);
	//pBook = pBooks->Add((long)xlWorkbook);
	pBook = pBooks.Add(vtMissing);
}

void cExcel::OpenWorkBook(CString filename, CString sSheetName /*= _T("")*/)
{
	assert(pBooks != NULL);
	pBook = pBooks.Open(filename, vtMissing, vtMissing,
		vtMissing, vtMissing, vtMissing, vtMissing,
		vtMissing, vtMissing, vtMissing, vtMissing,
		vtMissing, vtMissing);// , vtMissing, vtMissing);
	pSheets.AttachDispatch(pBook.GetWorksheets(), true);
	if ( ! sSheetName.IsEmpty())
	{
		pSheet.AttachDispatch(pSheets.GetItem(_variant_t(sSheetName)), true);
	}
	else
	{
		pSheet.AttachDispatch(pSheets.GetItem(COleVariant((short)(1))), true);
	}
}

void cExcel::SelectAcitveSheet(void)
{
	assert(pBook != NULL);
	pSheets = pBook.GetWorksheets();
	pSheet = pBook.GetActiveSheet();
}

void cExcel::SetSheetName(CString sheetname)
{
	assert(pSheet != NULL);
	pSheet.SetName(sheetname);
}

void cExcel::SelectRange(CString range)
{
	assert(pSheet != NULL);
	//pRange = pSheet.GetRange(_variant_t(range),vtMissing);
	pRange = pSheet.GetRange(_variant_t(range), _variant_t(range));
}

void cExcel::SelectRange(int x, int y)
{
	CString range;
	x = (0 == x) ? 1 : x;
	y = (0 == y) ? 1 : y;
	//x = (x > 26) ? (x % 26) : (x);	
	if (x <= 26)
	{
		range.Format(_T("%c%d"), x + 'A' - 1, y);
	}
	else
	{
		//对J==0 出错问题的修改 以及编号大于52的问题修改
		int j = x % 26;
		if (j==0)
		{
			int k = x / 26;
			char Temp = 63 + k;
			range.Format(_T("%cZ%d"), Temp,y);
		}
		else
		{
			int k = x / 26;
			char Temp = 64 + k;
			char c = j + 'A' - 1;
			CString str1;
			str1.Format(_T("%c"), c);
			CString str;
			str.Format(_T("%c%s"), Temp, str1);
			range.Format(_T("%s%d"), str, y);
		}
	}
   // range.Format(_T("%c%d"), x + 'A' - 1, y);
	SelectRange(range);
}

void cExcel::GetFont(void)
{
	assert(pFont != NULL);
	pFont = pRange.GetFont();
}

void cExcel::SwitchToSheet(CString sSheetName)
{
	pSheet.ReleaseDispatch();
	pSheet.AttachDispatch(pSheets.GetItem(_variant_t(sSheetName)), true);
}

bool IsNumber(CString sValue, bool bAcceptDoubles)
{
	TCHAR cCurrent = 0;
	int   iDot = 0;

	// check
	if (sValue.IsEmpty()) {
		return false;
	}
	// Check each character of the string
	for (int i = 0; i < sValue.GetLength(); i++) {
		cCurrent = sValue.GetAt(i);

		// The minus sign may only be at the begin
		if (cCurrent == _T('-') && i == 0) {
			continue;
		}
		// A dot may not occure of we do not accept doubles
		if (cCurrent == _T('.') && !bAcceptDoubles) {
			return false;
		}
		// A dot may only occure once
		if (cCurrent == _T('.')) {
			iDot++;
			if (iDot == 1) {
				continue;
			}
			else {
				return false;
			}
		}

		// A number is something between 0 and 9, dah!
		if (cCurrent < _T('0') || cCurrent > _T('9')) {
			return false;
		}
	}

	// We passed our wonderfull check,
	// so the string is an int or double Yiihaa
	return true;
}

void cExcel::PutValue(CString string)
{
	assert(pRange != NULL);
	if ( !string.IsEmpty() && ! IsNumber(string,true))
	{
		pRange.SetNumberFormat(COleVariant(_T("@")));
	}
	pRange.SetValue2(_variant_t(string));
}

void cExcel::PutValue(int x, int y, CString string)
{
	SelectRange(x, y);
	PutValue(string);
}

CString cExcel::GetCString(int x, int y)
{
	CString result;
	COleVariant vResult;
	SYSTEMTIME st;
	CString stry, strm, strd;

	SelectRange(x, y);
	////////////////////////
	assert(pRange != NULL);
	///////////////////////
	vResult = pRange.GetValue2();
	//_variant_t vart = pRange.GetItem(_variant_t((long)y), _variant_t((long)x));
	vResult.ChangeType(VT_BSTR, NULL);
	switch (vResult.vt) 
	{

	case VT_BSTR:    //OLE Automation string
	{
		result = vResult.bstrVal;
		break;
	}
	case VT_R8: // 8 byte real
	{
		result.Format(_T("%.f"), vResult.dblVal);
		break;
	}
	case VT_DATE: //date
	{
		VariantTimeToSystemTime(vResult.date, &st);
		stry.Format(_T("%d"), st.wYear);
		strm.Format(_T("%d"), st.wMonth);
		strd.Format(_T("%d"), st.wDay);
		result = stry + _T("-") + strm + _T("-") + strd;
		break;
	}
	case VT_EMPTY: //empty
	{
		result.Empty();
		break;
	}
	default:
	{
		//AfxMessageBox(_T("not string "));
		result.Empty();
		break;
	}
	}
	return result;
}

void cExcel::SaveAs(CString filename)
{
	////////////////////////
	assert(pBook != NULL);
	///////////////////////
	pApp.SetDisplayAlerts(FALSE);
	pBook.SaveAs(COleVariant(filename), covOptional, covOptional,
		covOptional, covOptional, covOptional, 0,
		covOptional, covOptional, covOptional, covOptional);// , covOptional);
}

void cExcel::SaveAs(CString path, CString filename)
{
	assert(pBook != NULL);
	path += filename;
	pBook.SaveAs(COleVariant(path), covOptional, covOptional,
		covOptional, covOptional, covOptional, 0,
		covOptional, covOptional, covOptional, covOptional);// , covOptional);
}

double cExcel::GetValue(int x, int y)
{
	double result;
	COleVariant vResult;

	SelectRange(x, y);
	vResult = pRange.GetValue2();

	if (VT_R8 == vResult.vt)
	{
		result = vResult.dblVal;
	}
	else
	{
		AfxMessageBox(_T(" not VT_R8 "));
	}

	return result;
}

void cExcel::Quit(void)
{
	assert(pBook != NULL);
	assert(pBooks != NULL);
	assert(pApp != NULL);
	pApp.SetVisible(FALSE);
	pApp.SetDisplayAlerts(FALSE);
	pBook.SetSaved(0);//don't answer to save
	pBooks.Close();
	pApp.Quit();
	//pRange.ReleaseDispatch();
	if (pRange != NULL)
	{
		pRange.ReleaseDispatch();
	}
	if (pSheet != NULL)
	{
		pSheet.ReleaseDispatch();
	}
	if (pSheets != NULL)
	{
		pSheets.ReleaseDispatch();
	}
	if (pBook != NULL)
	{
		pBook.ReleaseDispatch();
	}
	if (pBooks != NULL)
	{
		pBooks.ReleaseDispatch();
	}
	if (pApp != NULL)
	{
		pApp.ReleaseDispatch();
	}
	CoUninitialize();
}

CString cExcel::GetCString0(int x, int y)
{
	CString result;
	COleVariant vResult;
	SYSTEMTIME st;
	CString stry, strm, strd;

	SelectRange(x, y);
	////////////////////////
	assert(pRange != NULL);
	///////////////////////
	vResult = pRange.GetValue2();
	vResult.ChangeType(VT_BSTR, NULL);
	switch (vResult.vt)
	{
	case VT_BSTR:    //OLE Automation string
	{
		result = vResult.bstrVal;
		break;
	}
	case VT_R8: // 8 byte real
	{
		result.Format(_T("%.0f"), vResult.dblVal);
		break;
	}
	case VT_DATE: //date
	{
		VariantTimeToSystemTime(vResult.date, &st);
		stry.Format(_T("%d"), st.wYear);
		strm.Format(_T("%d"), st.wMonth);
		strd.Format(_T("%d"), st.wDay);
		result = stry + _T("-") + strm + _T("-") + strd;
		break;
	}
	case VT_EMPTY: //empty
	{
		result.Empty();
		break;
	}
	default:
	{
		//AfxMessageBox(_T("not string "));
		result.Empty();
		break;
	}
	}
	return result;
}

void cExcel::InsertRow(int nRow)
{
	//SelectRange(2, 2);
	//SetSheetName(_T("CABLE SCHEDULE"));
	_ExRange rang;
	rang.AttachDispatch(pSheet.GetCells(), true);

	_ExRange copyFrom, copyTo;
	copyTo.AttachDispatch(rang.GetItem(COleVariant((long)(nRow)), COleVariant((long)1)).pdispVal);
	copyFrom.AttachDispatch(copyTo.GetEntireRow());

	copyFrom.Insert(vtMissing);// , vtMissing);    //插入空格
	copyFrom.ReleaseDispatch();
	copyTo.ReleaseDispatch();
	rang.ReleaseDispatch();
}

// void cExcel::SetBackColor( long color )
// {
// 	assert(pRange != NULL );
// 	pInterior = pRange.GetInterior();
// 	pInterior->SetColor((long )color);
// }

void cExcel::AddSheet(CString sSheetName)
{
// 	LPDISPATCH lpDisp;
// 	// Append a sheet in the end
// 	lpDisp = pSheets.Add(vtMissing, _variant_t(pSheets.GetItem(
// 		COleVariant((short)(pSheets.GetCount())))),
// 		_variant_t((long)1), vtMissing);
// 	pSheet.AttachDispatch(lpDisp);
// 	SetSheetName(sSheetName);

	LPDISPATCH lpDisp;
	// Append a sheet in the end
//	long i=pSheets.GetCount();
	lpDisp = pSheets.Add(vtMissing,vtMissing,_variant_t((long)1), vtMissing);
	pSheet.AttachDispatch(lpDisp);
	pSheet.SetName(sSheetName);
}

long cExcel::GetSheetNum()
{
	assert(pSheets != NULL);
	long i = pSheets.GetCount();
	return i;
}

void cExcel::RemoveSheet(CString sSheetName)
{
	SwitchToSheet(sSheetName);
	pSheet.Delete();
}

bool cExcel::isOpenExcel(CString filePath)
{
	fstream iofile(filePath, ios::in | ios::out | ios::binary);//打开文件
	if (!iofile)//如果打开失败
	{
		//MessageBox(_T("文件可能已经打开，请注意检查"));

		return true;
	}

	return false;
}

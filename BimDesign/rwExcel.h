#pragma once
#include "Stdafx.h"
#include "excel9.h"
#include <fstream>
using namespace std;

//excel 读写
//typedef LPDISPATCH Interior;

//const COleVariant covTrue((short)TRUE);
//const COleVariant covFalse((short)FALSE);

//COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);

class cExcel
{
public:
	cExcel();
	~cExcel();
	bool StartApp(CString strMoldName = _T(""));

	void CreateWorkBook(void);
	void OpenWorkBook(CString filename, CString sSheetName = _T(""));
	void SelectAcitveSheet(void);
	void SetSheetName(CString sheetname);
	void SelectRange(CString range);
	void SelectRange(int x, int y);
	void SetBackColor(long color);
	void GetFont(void);
	void SwitchToSheet(CString sSheetName);
	void AddSheet(CString sSheetName);
	long GetSheetNum();
	void RemoveSheet(CString sSheetName);
	// 	void SetFontName(CString fontname);
	// 	void SetFontSytle(CString fontsytle);
	// 	void SetFontSize(long size);
	void PutValue(CString string);
	void PutValue(int x, int y, CString string);
	//
	CString GetCString(int x, int y);
	CString GetCString0(int x, int y);
	double GetValue(int x, int y);

	void InsertRow(int nRow);
	void SaveAs(CString filename);
	void SaveAs(CString path, CString filename);
	void Quit(void);
	bool isOpenExcel(CString filePath);
	_Worksheet GetSheet(){return pSheet;}
	_ExSheets GetExSheet(){ return pSheets; }
//	friend class CAutoRank;//将CAutoRank定义成cExcel类的友元类，则CAutoRank可以使用cExcel类的私有变量
//	friend class CBudgetView;//预算配套信息预览
//	friend class CAlterPrices;
private:
	_ExApplication pApp;
	Workbooks pBooks;
	_Workbook pBook;
	_ExSheets  pSheets;
	_Worksheet pSheet;
	_ExRange  pRange;
	Interior  pInterior;
	FontPtr   pFont;
};
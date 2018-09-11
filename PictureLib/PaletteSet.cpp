// PaletteSet.cpp : 实现文件
//

#include "stdafx.h"
#include "PaletteSet.h"
#include "afxdialogex.h"


// CPaletteSet 对话框

IMPLEMENT_DYNAMIC(CPaletteSet, CAdUiPaletteSet)

CPaletteSet::CPaletteSet(CWnd* pParent /*=NULL*/)
: CAdUiPaletteSet()
{

}

CPaletteSet::~CPaletteSet()
{
}

void CPaletteSet::DoDataExchange(CDataExchange* pDX)
{
	CAdUiPaletteSet::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaletteSet, CAdUiPaletteSet)
END_MESSAGE_MAP()


// CPaletteSet 消息处理程序

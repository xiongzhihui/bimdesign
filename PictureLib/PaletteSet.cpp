// PaletteSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PaletteSet.h"
#include "afxdialogex.h"


// CPaletteSet �Ի���

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


// CPaletteSet ��Ϣ�������

// CreatGuard.cpp : 实现文件
//

#include "stdafx.h"
#include "CreatGuard.h"
#include "afxdialogex.h"
#include "LineJig.h"


// CCreatGuard 对话框

IMPLEMENT_DYNAMIC(CCreatGuard, CAcUiDialog)

CCreatGuard::CCreatGuard(CWnd* pParent /*=NULL*/)
: CAcUiDialog(CCreatGuard::IDD, pParent)
{
	m_sLen = _T("1300");
	m_sNum = _T("50");
}

CCreatGuard::~CCreatGuard()
{
}

void CCreatGuard::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEN,m_sLen);
	DDX_Text(pDX, IDC_EDIT_NUM,m_sNum);
}


BEGIN_MESSAGE_MAP(CCreatGuard, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CCreatGuard::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_DRAWMID, &CCreatGuard::OnBnClickedBtnDrawmid)
//ON_BN_CLICKED(IDC_BTN_SELECT, &CCreatGuard::OnBnClickedBtnSelect)
ON_BN_CLICKED(IDC_BUTTON3, &CCreatGuard::OnBnClickedButton3)
END_MESSAGE_MAP()


// CCreatGuard 消息处理程序


void CCreatGuard::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BeginEditorCommand();
	bool b;
	AcGePoint3d ptStart, ptEnd;
	b=CGetInputUtil::GetPoint(_T("\n选取起始分界线第一点"), ptStart);
	if (!b)
	{
		CompleteEditorCommand(TRUE);
		return;
	}
	b=CGetInputUtil::GetPoint(ptStart, _T("\n选取起始分界线第二点"), ptEnd);
	if (!b)
	{
		CompleteEditorCommand(TRUE);
		return;
	}
//  可根据直线穿过的实体检索出相应边框
// 	struct resbuf *pointlist=acutBuildList(RTPOINT,asDblArray(ptStart),RTPOINT,asDblArray(ptEnd),RTNONE);
// 	struct resbuf *rb = acutBuildList(8, TEXT("00-Y-JG主体结构外边线"), RTNONE);
// 	ads_name ssname;
// 	auto rs = acedSSGet(_T("F"), pointlist, NULL, rb, ssname);
// 	acutRelRb(pointlist);
// 	acutRelRb(rb);
// 	long lenth = 0;
// 	if (acedSSLength(ssname,&lenth)!=RTNORM)
// 	{
// 		return;
// 	}
// 	ads_name e;
// 	AcDbObjectId id;
// 	acedSSName(ssname, 0, e);
// 	acdbGetObjectId(id, e);
// 	acedSSFree(ssname);

	AcDbEntity *pEnt = NULL;
	AcGePoint3d ptPick;
	b=CSelectUtil::PromptSelectEntity(_T("选择多段线"), AcDbPolyline::desc(), pEnt, ptPick);
	if (!b)
	{
		CompleteEditorCommand(TRUE);
		return;
	}
	CompleteEditorCommand();
	AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
	AcDbLine *pline = new AcDbLine(ptStart, ptEnd);
	AcGePoint3d pPolyStr, pPolyEnd;
	pPoly->getStartPoint(pPolyStr);
	pPoly->getEndPoint(pPolyEnd);
	if (pPolyStr == pPolyEnd)
	{
		acutPrintf(_T("\n多段线首尾相连"));
	}
	//计算出选择直线与多段线的交点 此焦点用于绘制圆孔的起点 
	AcGePoint3dArray ptArr;
	pPoly->intersectWith(pline, AcDb::kOnBothOperands, ptArr);
	AcGePoint3d p1, p2;

	if (ptArr.length() == 2)
	{
		p1 = ptArr[0];
		p2 = ptArr[1];
	}
	//现阶段不用判断起始到终止，无论起点终点 都是向左绘制
// 	double dLeft = GetPolylineLeftX(pPoly, TRUE);
// 	double dRight = GetPolylineLeftX(pPoly, FALSE);
// 
// 	AcGePoint3d pCirStr, pCirEnd;
// 	if (p1.y > p2.y)
// 	{
// 		if (p1.x - dLeft < dRight - p1.x)
// 		{
// 			pCirEnd = p1;
// 			pCirStr = p2;
// 		}
// 		else
// 		{
// 			pCirEnd = p2;
// 			pCirStr = p1;
// 		}
// 	}
// 	else
// 	{
// 		if (p2.x - dLeft < dRight - p2.x)
// 		{
// 			pCirEnd = p2;
// 			pCirStr = p1;
// 		}
// 		else
// 		{
// 			pCirEnd = p1;
// 			pCirStr = p2;
// 		}
// 	}
//	
//	AcDbText *pText = new AcDbText(pCirStr, _T("起始"), AcDbObjectId::kNull, 1000);
//	AcDbText *pText2 = new AcDbText(pCirEnd, _T("结束"), AcDbObjectId::kNull, 1000);

	double dOffset=-500;
	int iTemp = pPoly->numVerts();
	if (iTemp>2)
	{
		AcGePoint3d ptS, pt2;
		pPoly->getStartPoint(ptS);
		pPoly->getPointAt(1, pt2);
		if (pt2.x<ptS.x||pt2.y>ptS.y)
		{
			dOffset = -500;
		}
		else
		{
			dOffset = 500;
		}
	}
	AcDbVoidPtrArray ptarr;
	pPoly->getOffsetCurves(dOffset, ptarr);
	//多段线偏移后还是多段线 并且只有一根 此根多段线为 围护桩轴线
	AcDbPolyline *pPolyOffset = static_cast<AcDbPolyline*>(ptarr[0]);
	AcGePoint3d pCirStr2, pCirEnd2;
	AcGePoint3d p3;
	if (p1.y>p2.y)
	{
	}
	else
	{
		p3 = p1;
		p1 = p2;
		p2 = p3;
	}
	pPolyOffset->getClosestPointTo(p1, pCirStr2);
	pPolyOffset->getClosestPointTo(p2, pCirEnd2);

	WritePoint(pCirStr2, pCirEnd2);

	double AllLen;
	AcGePoint3d ptt, ptt2;
	pPolyOffset->getPointAt(pPolyOffset->numVerts() - 1, ptt);
	pPolyOffset->getEndPoint(ptt2);
	pPolyOffset->getDistAtPoint(ptt, AllLen);
	AllLen += ptt.distanceTo(ptt2);

	double dLen, dNum;
	dLen = _wtof(m_sLen);
	dNum = _wtof(m_sNum);

	std::vector<AcGePoint3d> vecPt;
	vecPt = GetDrawCirPoint(pPolyOffset, pCirStr2, pCirEnd2, dLen, dNum/*, AllLen*/);
	pPolyOffset->close();
	delete pPolyOffset;
	ptarr.removeAll();
	for (int i = 0; i < vecPt.size(); i++)
	{
		AcGePoint3d pt = vecPt[i];
		AcDbCircle*pcir = new AcDbCircle(pt, AcGeVector3d(0, 0, 1), 500);
		CDwgDatabaseUtil::PostToModelSpace(pcir);
	}

	pPoly->close();
	pline->close();
	delete pline;
// 	CDwgDatabaseUtil::PostToModelSpace(pText);
// 	CDwgDatabaseUtil::PostToModelSpace(pText2);
	CAcUiDialog::OnOK();
}

void CCreatGuard::OnBnClickedBtnDrawmid()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BeginEditorCommand();
	bool b;
	AcDbEntity *pEnt = NULL;
	AcGePoint3d ptPick;
	b = CSelectUtil::PromptSelectEntity(_T("选择多段线"), AcDbPolyline::desc(), pEnt, ptPick);
	if (!b)
	{
		CompleteEditorCommand(TRUE);
		return;
	}
	CompleteEditorCommand();
	AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);

	double dOffset = -500;
	int iTemp = pPoly->numVerts();
	if (iTemp > 2)
	{
		AcGePoint3d ptS, pt2;
		pPoly->getStartPoint(ptS);
		pPoly->getPointAt(1, pt2);
		if (pt2.x<ptS.x || pt2.y>ptS.y)
		{
			dOffset = -500;
		}
		else
		{
			dOffset = 500;
		}
	}
	AcDbVoidPtrArray ptarr;
	pPoly->getOffsetCurves(dOffset, ptarr);
	/*int iLen = ptarr.length();*/
	AcDbPolyline *pPolyOffset = static_cast<AcDbPolyline*>(ptarr[0]);
	AcGePoint3d ps1,ps2;

	ReadPoint(ps1, ps2);
/*	WritePoint(pCirStr2, pCirEnd2);*/

	AcGePoint3d pU1,pD1;
	if (ps1.y>ps2.y)
	{
		pU1 = ps1;
		pD1 = ps2;
	}
	else
	{
		pU1 = ps2;
		pD1 = ps1;
	}

	double dLen, dNum;
	dLen = _wtof(m_sLen);
	dNum = _wtof(m_sNum);

	std::vector<AcGePoint3d> vecPt,vecPt2;
	vecPt = GetDrawCirMidPoint(pPolyOffset, pU1,  dLen, dNum/2.0);
	vecPt2 = GetDrawCirMidPoint(pPolyOffset, pD1, dLen, dNum/2.0);
	pPolyOffset->close();
	delete pPolyOffset;
	ptarr.removeAll();
	AcGePoint3d pW1, pW2;
	for (int i = 0; i < vecPt.size(); i++)
	{
		AcGePoint3d pt = vecPt[i];
		if (i==vecPt.size()-1)
		{
			pW1 = pt;
			break;
		}
		AcDbCircle*pcir = new AcDbCircle(pt, AcGeVector3d(0, 0, 1), 500);
		CDwgDatabaseUtil::PostToModelSpace(pcir);
	}
	for (int i = 0; i < vecPt2.size(); i++)
	{
		AcGePoint3d pt = vecPt2[i];
		if (i == vecPt2.size() - 1)
		{
			pW2 = pt;
			break;
		}
		AcDbCircle*pcir = new AcDbCircle(pt, AcGeVector3d(0, 0, 1), 500);
		CDwgDatabaseUtil::PostToModelSpace(pcir);
	}
	WritePoint(pW1, pW2);
	pPoly->close();
	CAcUiDialog::OnOK();
}

BOOL CCreatGuard::WritePoint(const AcGePoint3d &ptS, const AcGePoint3d &ptE)
{
	CIniFile ini(GetIniPath());
	double ptSx = ptS.x;
	double ptSy = ptS.y;
	double ptEx = ptE.x;
	double ptEy = ptE.y;

	ini.SetValueOfKey(_T("DRAWCIRCLE"), _T("PSTRX"), ptSx);
	ini.SetValueOfKey(_T("DRAWCIRCLE"), _T("PSTRY"), ptSy);
	ini.SetValueOfKey(_T("DRAWCIRCLE"), _T("PENDX"), ptEx);
	ini.SetValueOfKey(_T("DRAWCIRCLE"), _T("PENDY"), ptEy);

	return TRUE;
}

BOOL CCreatGuard::ReadPoint(AcGePoint3d &ptS,AcGePoint3d &ptE)
{
	CIniFile ini(GetIniPath());	
	double ptSx;
	double ptSy;
	double ptEx;
	double ptEy;
	ini.GetValueOfKey(_T("DRAWCIRCLE"), _T("PSTRX"), ptSx);
	ini.GetValueOfKey(_T("DRAWCIRCLE"), _T("PSTRY"), ptSy);
	ini.GetValueOfKey(_T("DRAWCIRCLE"), _T("PENDX"), ptEx);
	ini.GetValueOfKey(_T("DRAWCIRCLE"), _T("PENDY"), ptEy);
	ptS.x = ptSx;
	ptS.y = ptSy;
	ptE.x = ptEx;
	ptE.y = ptEy;
	return TRUE;
}

//void CCreatGuard::OnBnClickedBtnSelect()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	BeginEditorCommand();
//	struct resbuf *rb;
//	rb = acutBuildList(8, TEXT("00-Y-JG主体结构外边线"), RTNONE);
//	ads_point pt1, pt2;
//	ads_name ssname;
//	acedSSGet(_T("w"), pt1, pt2, rb, ssname);
//	acutRelRb(rb);
//	acedSSFree(ssname);
//	CompleteEditorCommand(TRUE);
//}

void CCreatGuard::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BeginEditorCommand();
	ads_name ename; ads_point pt;
	double dLen; int iNum;
	dLen = _wtof(m_sLen);
	iNum = _wtoi(m_sNum);
	if (acedEntSel(_T("选择多段线"), ename, pt) != RTNORM)
	{
		CompleteEditorCommand();
		return;
	}
	AcDbObjectId polylineid;
	acdbGetObjectId(polylineid, ename);
	CLineJig jig;
	jig.doIt(polylineid,dLen,iNum);

	CAcUiDialog::OnOK();
}


BOOL CCreatGuard::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

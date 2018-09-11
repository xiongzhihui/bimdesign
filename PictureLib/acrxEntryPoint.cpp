//////////////////////////////////////////////////////////////////////////
// 本文件由向导生成
// 

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "SetLib.h"
#include "MyPalette.h"
#include "PaletteSet.h"
#include "FlDlg.h"
#include "CreatGuard.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")


CPaletteSet *g_pPalettSet = NULL;
CMyPalette *g_pPalett = NULL;
CAdUiPalette *g_pPalett2 = NULL;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CPictureLibApp : public AcRxArxApp{

public:
	CPictureLibApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: 如果有其他依赖项，请在这里加载

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: 初始化

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: 退出
		// You *must* call On_kUnloadAppMsg here

		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		Cmds_DestroyPalette();

		// TODO: 卸载依赖

		return (retCode) ;
	}

	static void Cmds_AddTy()
	{
		CAcModuleResourceOverride rs;
		CSetLib dlg;
		dlg.DoModal();
	}

	static void Cmds_ShowPalette()
	{
		if (!g_pPalettSet)
		{
			g_pPalettSet = new CPaletteSet;
			CRect rect(0, 0, 220, 400);
			g_pPalettSet->Create(TEXT("图库选项版"), WS_OVERLAPPED | WS_DLGFRAME, rect, acedGetAcadFrame()/*,PSS_EDIT_NAME|PSS_PROPERTIES_MENU|PSS_AUTO_ROLLUP|PSS_CLOSE_BUTTON*/);
			g_pPalett = new CMyPalette();
			g_pPalett2 = new CAdUiPalette();
			g_pPalett->Create(WS_CHILD | WS_VISIBLE, _T("图库面板"), g_pPalettSet);		
			g_pPalettSet->AddPalette(g_pPalett);
			/*g_pPalett->CreatChild();*/
			
			g_pPalett2->Create(WS_CHILD | WS_VISIBLE, _T("选项版2"), g_pPalettSet);
			g_pPalettSet->AddPalette(g_pPalett2);

			g_pPalettSet->EnableDocking(CBRS_ALIGN_ANY);
			g_pPalettSet->RestoreControlBar();
		}
		acedGetAcadFrame()->ShowControlBar(g_pPalettSet, TRUE, FALSE);
		if (g_pPalettSet->GetOpacity()!=100)
		{
			g_pPalettSet->SetOpacity(100);
		}
	}

	static void Cmds_DestroyPalette()
	{
		if (g_pPalettSet != NULL)
		{
			g_pPalettSet->DestroyWindow();
			delete g_pPalettSet;
			g_pPalettSet = NULL;
		}
// 		if (g_pPalett!=NULL)
// 		{
// 			g_pPalett->DestroyWindow();
// 			delete g_pPalett;
// 			g_pPalett = NULL;
// 		}
// 		if (g_pPalett2!=NULL)
// 		{
// 			g_pPalett2->DestroyWindow();
// 			delete g_pPalett2;
// 			g_pPalett2 = NULL;
// 		}
	}

	static void Cmds_RefreshPalette()
	{
		if (g_pPalett!=NULL)
		{
			g_pPalett->m_childDlg->RefreshPlaette();
		}		
	}

	static void Cmds_FlManage()
	{
		CAcModuleResourceOverride rs;
		CFlDlg dlg;
		dlg.DoModal();
	}

	static void Cmds_GetLineTest()
	{
// 		AcGePoint3d ptStart, ptEnd;
// 		CGetInputUtil::GetPoint(_T("选取第一点"), ptStart);
// 		CGetInputUtil::GetPoint(ptStart,_T("选取第二点"), ptEnd);
// 		AcDbEntity *pEnt = NULL;
// 		AcGePoint3d ptPick;
// 		CSelectUtil::PromptSelectEntity(_T("选择多段线"), AcDbPolyline::desc(), pEnt, ptPick);
// 		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
// 		AcDbLine *pline = new AcDbLine(ptStart, ptEnd);
// 		AcGePoint3d pPolyStr, pPolyEnd;
// 		pPoly->getStartPoint(pPolyStr);
// 		pPoly->getEndPoint(pPolyEnd);
// 		if (pPolyStr==pPolyEnd)
// 		{
// 			acutPrintf(_T("\n相等"));
// 		}
// 		AcGePoint3dArray ptArr;
// 		pPoly->intersectWith(pline, AcDb::kOnBothOperands, ptArr);
// 		AcGePoint3d p1, p2; double d1, d2;
// 		if (ptArr.length()==2)
// 		{
// 			p1 = ptArr[0];
// 			p2 = ptArr[1];
// 		}
// 		double dLeft = GetPolylineLeftX(pPoly, TRUE);
// 		double dRight = GetPolylineLeftX(pPoly, FALSE);
// 
// 		AcGePoint3d pCirStr, pCirEnd;
// 		if (p1.y>p2.y)
// 		{
// 			if (p1.x-dLeft<dRight-p1.x)
// 			{
// 				pCirEnd = p1;
// 				pCirStr = p2;
// 			}
// 			else
// 			{
// 				pCirEnd = p2;
// 				pCirStr = p1;
// 			}
// 		}
// 		else
// 		{
// 			if (p2.x - dLeft < dRight - p2.x)
// 			{
// 				pCirEnd = p2;
// 				pCirStr = p1;
// 			}
// 			else
// 			{
// 				pCirEnd = p1;
// 				pCirStr = p2;
// 			}
// 		}
// 
// 		AcDbText *pText = new AcDbText(pCirStr, _T("起始"),AcDbObjectId::kNull,1000);
// 		AcDbText *pText2 = new AcDbText(pCirEnd, _T("结束"), AcDbObjectId::kNull, 1000);
// 
// // 		AcGePoint3d ptest1, ptest2;
// // 		
// // 		pPoly->getPointAtDist(2000, ptest2);
// // 		pPoly->getPointAtParam(1000, ptest2);
// 
// 		AcDbVoidPtrArray ptarr;
// 		pPoly->getOffsetCurves(-500, ptarr);
// 		int iLen = ptarr.length();
// 		AcDbPolyline *pPolyOffset = static_cast<AcDbPolyline*>(ptarr[0]);
// 		AcGePoint3d pCirStr2, pCirEnd2;
// 		pPolyOffset->getClosestPointTo(pCirStr, pCirStr2);
// 		pPolyOffset->getClosestPointTo(pCirEnd, pCirEnd2);
// 
//  		double AllLen;
// 		AcGePoint3d ptt,ptt2;
// 		pPolyOffset->getPointAt(pPolyOffset->numVerts() - 1, ptt);
// 		pPolyOffset->getEndPoint(ptt2);
// 		pPolyOffset->getDistAtPoint(ptt, AllLen);
// 		AllLen += ptt.distanceTo(ptt2);
// 
// 		std::vector<AcGePoint3d> vecPt;
// 		vecPt = GetDrawCirPoint(pPolyOffset, pCirStr2, pCirEnd2, 1200, 500, AllLen);
// 		pPolyOffset->close();
// 		delete pPolyOffset; ptarr.removeAll();
// 		for (int i = 0; i < vecPt.size();i++)
// 		{
// 			AcGePoint3d pt=vecPt[i];
// 			AcDbCircle*pcir = new AcDbCircle(pt, AcGeVector3d(0, 0, 1), 500);
// 			CDwgDatabaseUtil::PostToModelSpace(pcir);
// 		}
// 
// 		pPoly->close();
// 		pline->close();
// 		CDwgDatabaseUtil::PostToModelSpace(pText);
// 		CDwgDatabaseUtil::PostToModelSpace(pText2);		
		CAcModuleResourceOverride rs;
		CCreatGuard dlg;
		dlg.DoModal();
	}

	virtual void RegisterServerComponents () {
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CPictureLibApp)
ACED_ARXCOMMAND_ENTRY_AUTO(CPictureLibApp, Cmds, _AddTy, AddTy, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CPictureLibApp, Cmds, _ShowPalette, ShowPalette, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CPictureLibApp, Cmds, _DestroyPalette, DestroyPalette, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CPictureLibApp, Cmds, _RefreshPalette, RefreshPalette, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CPictureLibApp, Cmds, _FlManage, FlManage, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CPictureLibApp, Cmds, _GetLineTest, GetLineTest, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
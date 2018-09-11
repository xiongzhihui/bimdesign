//////////////////////////////////////////////////////////////////////////
// 本文件由向导生成
// 

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "AllHead.h"
#include "BimDlg.h"
#include "DoubleClickEdit.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CBimDesignApp : public AcRxArxApp{

public:
	CBimDesignApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: 如果有其他依赖项，请在这里加载

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: 初始化
		/*AcDbDoubleClickEdit::rxInit();*/
		CDoubleClickEdit *pCli = new CDoubleClickEdit;
		AcDbBlockReference::desc()->addX(AcDbDoubleClickEdit::desc(), pCli);
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: 退出
		AcDbBlockReference::desc()->delX(AcDbDoubleClickEdit::desc());
		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: 卸载依赖
		return (retCode) ;
	}

	static void Cmds_Test()
	{
		
// 		AcDbViewTableRecord view;
// 		view.setCenterPoint(AcGePoint2d(0,0));
// 		view.setHeight(1000);
// 		view.setWidth(1000);
// 		acedSetCurrentView(&view,NULL);
// 		acdbHostApplicationServices()->workingDatabase()->updateExt(TRUE);

// 		AcGePoint3d pt(0, 0, 0);
// 		for (int i = 0; i < 2000;i++)
// 		{
// 			CString sTemp;
// 			sTemp.Format(_T("%d"), i);
// 			AcGePoint3d ptTemp(pt.x + i * 100, pt.y, pt.z);
// 			AcDbCircle *pCir = new AcDbCircle(ptTemp, AcGeVector3d(0, 0, 1), 30);
// 			CDwgDatabaseUtil::SetXdata(_T("test"), sTemp, pCir);
// 			CDwgDatabaseUtil::PostToModelSpace(pCir);
// 		}
	}
	static void Cmds_Test2()
	{
		AcDbObjectIdArray idarr;
		idarr=CDwgDatabaseUtil::GetAllEntityIds();
		int iLen = idarr.length();
		for (int i = 0; i < iLen;i++)
		{
			AcDbObjectId id = idarr[i];
			AcDbEntity *pEnt = NULL;
			Acad::ErrorStatus es;
			es=acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead);
			if (es!=eOk)
			{
				continue;
			}
			CString sVale;
			CDwgDatabaseUtil::getXdata(_T("test"), sVale, pEnt);
			acutPrintf(sVale);
			pEnt->close();
		}
	}	
	static void	Cmds_Test3()
	{
		AcDbObjectIdArray idArr;
		std::vector<AcGePoint3d> vecPt;
		idArr = CDwgDatabaseUtil::GetAllEntityIds();
		int iNum = idArr.length();
		for (int i = 0; i < iNum;i++)
		{
			AcDbEntity *pEnt = NULL;
			AcDbObjectId idTemp = NULL;
			idTemp = idArr[i];
			Acad::ErrorStatus es;
			es = acdbOpenObject(pEnt,idTemp,AcDb::kForWrite);
			if (es!=eOk)
			{
				continue;
			}
			if (pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *pCir = static_cast<AcDbCircle*>(pEnt);
				AcGePoint3d ptTemp = pCir->center();
				vecPt.push_back(ptTemp);
			}
			pEnt->close();
		}
		std::sort(vecPt.begin(), vecPt.end(),cmp);
		size_t iL = vecPt.size();
		for (int i = 1; i < iL + 1;i++)
		{
			AcGePoint3d pt = vecPt[i - 1];
			CString sI;
			sI.Format(_T("%d"), i);
			AcDbText *pText = new AcDbText(pt, sI, AcDbObjectId::kNull, 500);
			CDwgDatabaseUtil::PostToModelSpace(pText);
		}
	}
	
	static void Cmds_AddBimInfo()
	{
		CLayerUtil::Add(_T("BOM信息"));
		AcDbObjectId LayerId=CLayerUtil::GetLayerId(_T("BOM信息"));
		acdbHostApplicationServices()->workingDatabase()->setClayer(LayerId);
		AcDbEntity*pEnt = NULL;
		AcGePoint3d pickPoint;
		bool b=CSelectUtil::PromptSelectEntity(_T("\n请选择需要添加bim信息的实体"), AcDbEntity::desc(), pEnt, pickPoint);
		if (!b)
		{
			AfxMessageBox(_T("未选择对象"));
			return;
		}
		pEnt->close();
		//添加进引线及序号 
		AcDbObjectId idBim=CBlockUtil::CopyBlockDefFromOtherDwg(GetDwgPath(), _T("BimInfo"));
		AcDbObjectId idInsert=CBlockUtil::InsertBlockRefWithAttribute(idBim, pickPoint);
//		CBlockUtil::InsertBlockRef(idBim, pickPoint);

		//打开对话框对信息进行处理
		CAcModuleResourceOverride es;
		CBimDlg dlg(idInsert,FALSE);
		dlg.DoModal();
	}

	static void Cmds_ModBimInfo()
	{
		ads_name ename;
		ads_point pt;
		if (acedEntSel(_T("\n请选择需要修改bim信息的引线"), ename,pt) != RTNORM)
		{
			AfxMessageBox(_T("未选中实体"));
			return;
		}
		AcDbObjectId entId;
		acdbGetObjectId(entId, ename);

		CAcModuleResourceOverride es;
		CBimDlg dlg(entId, TRUE);
		dlg.DoModal();
	}

	struct bimInfo
	{
		int iItem;
		AcDbObjectId id;
	};
	struct OutInfo
	{
		CString LOC;
		CString CAT;
		CString TYP;
		CString DES;
		CString NOT;
		CString QTY;
	};

	static void Cmds_StatBimInfo()
	{
		AcDbObjectIdArray idArr=CDwgDatabaseUtil::GetAllEntityIds(_T("BOM信息"));
		std::vector<bimInfo> vecInfo;
		std::vector<OutInfo> vecOut;
		for (int i = 0; i < idArr.length();i++)
		{
			AcDbObjectId idTemp = idArr[i];
			AcDbEntity *pEnt = NULL;
			Acad::ErrorStatus es=acdbOpenAcDbEntity(pEnt, idTemp, AcDb::kForWrite);
			if (es!=eOk)
			{
				continue;
			}
			CString sNum; int iNum;
			CDwgDatabaseUtil::getXdata(_T("Num"), sNum, pEnt);
			pEnt->close();
			iNum = _wtoi(sNum);
			bimInfo infoTemp;
			infoTemp.iItem = iNum;
			infoTemp.id = idTemp;
			vecInfo.push_back(infoTemp);
		}
		//排序后生成table
		std::sort(vecInfo.begin(), vecInfo.end(), [](const bimInfo &a, const bimInfo &b){return a.iItem < b.iItem; });

		//将信息筛选出并存入vec
		for (int i = 0; i<vecInfo.size();i++)
		{
			bimInfo infoTemp = vecInfo[i];
			AcDbObjectId idTemp = infoTemp.id;
			AcDbEntity *pEnt = NULL;
			Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, idTemp, AcDb::kForWrite);
			if (es != eOk)
			{
				continue;
			}
			CString sAllItemNum; int iAllItemNum;
			CDwgDatabaseUtil::getXdata(_T("AllItemNum"), sAllItemNum, pEnt);
			iAllItemNum = _wtoi(sAllItemNum);
			for (int j = 0; j < iAllItemNum;j++)
			{
				CString sI; sI.Format(_T("%d"), j);
				CString sLoc,sCat, sTyp, sDes, sNot, sQty;

				CDwgDatabaseUtil::getXdata(_T("Loc") + sI, sLoc, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Cat") + sI, sCat, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Typ") + sI, sTyp, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Des") + sI, sDes, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Not") + sI, sNot, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Qty") + sI, sQty, pEnt);

				OutInfo ifTemp;
				ifTemp.LOC = sLoc;
				ifTemp.CAT = sCat;
				ifTemp.TYP = sTyp;
				ifTemp.DES = sDes;
				ifTemp.NOT = sNot;
				ifTemp.QTY = sQty;
				vecOut.push_back(ifTemp);
			}
			pEnt->close();
		}

		//将所有信息导出到tab
		AcDbTable* ptab = new AcDbTable;
		ptab->setNumColumns(7);
		int iRow=vecOut.size();
		ptab->setNumRows(iRow + 1);
		ptab->setPosition(AcGePoint3d::kOrigin);
		ptab->setRowHeight(20);
		ptab->setColumnWidth(0, 50);
		ptab->setColumnWidth(1, 120);
		ptab->setColumnWidth(2, 120);
		ptab->setColumnWidth(3, 120);
		ptab->setColumnWidth(4, 350);
		ptab->setColumnWidth(5, 100);
		ptab->setColumnWidth(6, 50);
		ptab->setTextHeight(8);
		ptab->setAlignment(AcDb::kMiddleCenter);
		ptab->setTextString(0, 0, _T("行号"));
		ptab->setTextString(0, 1, _T("Location"));
		ptab->setTextString(0, 2, _T("Category"));
		ptab->setTextString(0, 3, _T("Type-Size"));
		ptab->setTextString(0, 4, _T("Description"));
		ptab->setTextString(0, 5, _T("Notes"));
		ptab->setTextString(0, 6, _T("Qty"));

		for (int i = 1; i < iRow+1;i++)
		{
			OutInfo infoTemp = vecOut[i - 1];
			CString sI; sI.Format(_T("%d"),i);
			ptab->setTextString(i, 0, sI);
			ptab->setTextString(i, 1, infoTemp.LOC);
			ptab->setTextString(i, 2, infoTemp.CAT);
			ptab->setTextString(i, 3, infoTemp.TYP);
			ptab->setTextString(i, 4, infoTemp.DES);
			ptab->setTextString(i, 5, infoTemp.NOT);
			ptab->setTextString(i, 6, infoTemp.QTY);
		}
		CDwgDatabaseUtil::PostToModelSpace(ptab);
	}

	static void Cmds_StatBimExcel()
	{
		//获取名称
		CString sInName = _T("");
		int iRet = acedGetString(0, _T("\n请填写excel名:"), sInName.GetBufferSetLength(1000));
		if (iRet!=RTNORM)
		{
			return;
		}
		CString sExcName = GetSaveExcPath();
		sExcName = sExcName + sInName + _T(".xlsx");

		AcDbObjectIdArray idArr = CDwgDatabaseUtil::GetAllEntityIds(_T("BOM信息"));
		std::vector<bimInfo> vecInfo;
		std::vector<OutInfo> vecOut;
		for (int i = 0; i < idArr.length(); i++)
		{
			AcDbObjectId idTemp = idArr[i];
			AcDbEntity *pEnt = NULL;
			Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, idTemp, AcDb::kForWrite);
			if (es != eOk)
			{
				continue;
			}
			CString sNum; int iNum;
			CDwgDatabaseUtil::getXdata(_T("Num"), sNum, pEnt);
			pEnt->close();
			iNum = _wtoi(sNum);
			bimInfo infoTemp;
			infoTemp.iItem = iNum;
			infoTemp.id = idTemp;
			vecInfo.push_back(infoTemp);
		}
		//排序后生成table
		std::sort(vecInfo.begin(), vecInfo.end(), [](const bimInfo &a, const bimInfo &b){return a.iItem < b.iItem; });

		//将信息筛选出并存入vec
		for (int i = 0; i < vecInfo.size(); i++)
		{
			bimInfo infoTemp = vecInfo[i];
			AcDbObjectId idTemp = infoTemp.id;
			AcDbEntity *pEnt = NULL;
			Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, idTemp, AcDb::kForWrite);
			if (es != eOk)
			{
				continue;
			}
			CString sAllItemNum; int iAllItemNum;
			CDwgDatabaseUtil::getXdata(_T("AllItemNum"), sAllItemNum, pEnt);
			iAllItemNum = _wtoi(sAllItemNum);
			for (int j = 0; j < iAllItemNum; j++)
			{
				CString sI; sI.Format(_T("%d"), j);
				CString sLoc,sCat, sTyp, sDes, sNot, sQty;

				CDwgDatabaseUtil::getXdata(_T("Loc") + sI, sLoc, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Cat") + sI, sCat, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Typ") + sI, sTyp, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Des") + sI, sDes, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Not") + sI, sNot, pEnt);
				CDwgDatabaseUtil::getXdata(_T("Qty") + sI, sQty, pEnt);

				OutInfo ifTemp;
				ifTemp.LOC = sLoc;
				ifTemp.CAT = sCat;
				ifTemp.TYP = sTyp;
				ifTemp.DES = sDes;
				ifTemp.NOT = sNot;
				ifTemp.QTY = sQty;
				vecOut.push_back(ifTemp);
			}
			pEnt->close();
		}

		cExcel excel;
		if (!excel.StartApp())
		{
			AfxMessageBox(_T("打开excel失败"));
			return;
		}
		excel.OpenWorkBook(GetExcPath(), _T("Input"));
		int iCount = vecOut.size();
		int iMark = 1;
		for (int i = 0; i < iCount;i++)
		{
			if (i==iMark*100)
			{
				acutPrintf(_T("\n完成：%d 条信息"), i);
				iMark++;
			}		
			OutInfo Temp = vecOut[i];
			excel.PutValue(1, 7 + i, Temp.LOC);
			excel.PutValue(2, 7 + i, Temp.CAT);
			excel.PutValue(3, 7 + i, Temp.TYP);
			excel.PutValue(4, 7 + i, Temp.DES);
			excel.PutValue(5, 7 + i, Temp.NOT);
			excel.PutValue(6, 7 + i, Temp.QTY);
		}
		acutPrintf(_T("\n已全部完成。保存excel路径为%s"), sExcName);
		excel.SaveAs(sExcName);
		excel.Quit();
	}

	virtual void RegisterServerComponents () {
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CBimDesignApp)
ACED_ARXCOMMAND_ENTRY_AUTO(CBimDesignApp, Cmds, _Test, Test, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CBimDesignApp, Cmds, _Test2, Test2, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CBimDesignApp, Cmds, _Test3, Test3, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CBimDesignApp, Cmds, _AddBimInfo, AddBimInfo, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CBimDesignApp, Cmds, _ModBimInfo, ModBimInfo, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CBimDesignApp, Cmds, _StatBimInfo, StatBimInfo, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CBimDesignApp, Cmds, _StatBimExcel, StatBimExcel, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)






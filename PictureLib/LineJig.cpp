#include "StdAfx.h"
#include "LineJig.h"
#include "AllHead.h"

CLineJigEntity::CLineJigEntity(AcDbObjectId Polylineid,double dLen,int iNum)
{
	m_polylineid = Polylineid;
	m_dLen = dLen;
	m_iNum = iNum;
}

CLineJigEntity::~CLineJigEntity()
{

}

void CLineJigEntity::DrawOrAddSubEnts(AcGiWorldDraw* mode)
{
	Acad::ErrorStatus es;
	es = acdbOpenObject(m_pPolyline, m_polylineid, AcDb::kForWrite);
	if (es!=eOk)
	{
		return;
	}
	double dOffset = -500;
	int iTemp = m_pPolyline->numVerts();
	if (iTemp > 2)
	{
		AcGePoint3d ptS, pt2;
		m_pPolyline->getStartPoint(ptS);
		m_pPolyline->getPointAt(1, pt2);
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
	m_pPolyline->getOffsetCurves(dOffset, ptarr);
	AcDbPolyline *pPolyOffset = static_cast<AcDbPolyline*>(ptarr[0]);
	m_pPolyline->close();

	AcDbLine *pLineTest = new AcDbLine;
	pLineTest->setStartPoint(AcGePoint3d(m_ptLinePath.x, m_ptLinePath.y + 100000, m_ptLinePath.z));
	pLineTest->setEndPoint(AcGePoint3d(m_ptLinePath.x, m_ptLinePath.y - 100000, m_ptLinePath.z));

	AcGePoint3dArray InsertPtAtt;
	pLineTest->intersectWith(pPolyOffset, AcDb::kOnBothOperands, InsertPtAtt);

	AcGePoint3d p1, p2;

	if (InsertPtAtt.length() == 2)
	{
		p1 = InsertPtAtt[0];
		p2 = InsertPtAtt[1];
	}
	else
	{
		delete pLineTest;
		delete pPolyOffset;
		return;
	}
	AcGePoint3d p3;
	if (p1.y > p2.y)
	{
	}
	else
	{
		p3 = p1;
		p1 = p2;
		p2 = p3;
	}
	std::vector<AcGePoint3d> vecPt;
	vecPt = GetDrawCirPoint(pPolyOffset, p1, p2, m_dLen, m_iNum/*, AllLen*/);

	delete pPolyOffset;
	if (mode!=NULL)
	{
		pLineTest->worldDraw(mode);
		delete pLineTest;
		for (size_t i = 0; i < vecPt.size(); i++)
		{
			AcGePoint3d pt = vecPt[i];
			AcDbCircle*pcir = new AcDbCircle(pt, AcGeVector3d(0, 0, 1), 500);
			pcir->worldDraw(mode);
			delete pcir;
		}
	}
	else
	{
		delete pLineTest;
		for (size_t i = 0; i < vecPt.size(); i++)
		{
			AcGePoint3d pt = vecPt[i];
			AcDbCircle*pcir = new AcDbCircle(pt, AcGeVector3d(0, 0, 1), 500);
			CDwgDatabaseUtil::PostToModelSpace(pcir);
		}
	}
	return;
}

Adesk::Boolean CLineJigEntity::worldDraw(AcGiWorldDraw * mode)
{
	DrawOrAddSubEnts(mode);
	return Adesk::kTrue;
}

void CLineJigEntity::PostToModelSpace()
{
	DrawOrAddSubEnts(NULL);
}

void CLineJigEntity::SetLinePath(AcGePoint3d pt)
{		
	assertWriteEnabled();
	m_ptLinePath = pt;
}

AcDbObjectId CLineJigEntity::GetBlkRefIds()
{
	return m_polylineid;
}

CLineJig::CLineJig()
{
	m_pLineJigEntity = NULL;
}

CLineJig::~CLineJig()
{
	if (m_pLineJigEntity)
	{
		delete m_pLineJigEntity;
		m_pLineJigEntity = NULL;
	}
}

bool CLineJig::doIt(AcDbObjectId Polylineid, double dLen, int iNum)
{
	if (m_pLineJigEntity!=NULL)
	{
		delete m_pLineJigEntity;
		m_pLineJigEntity = NULL;
	}
	m_pLineJigEntity = new CLineJigEntity(Polylineid,dLen,iNum);
	CString prompt = TEXT("\nÍÏ¶¯Ö±Ïß");
	setDispPrompt(prompt);
	AcEdJig::DragStatus stat = drag();
	if (stat==kNormal)
	{
		m_pLineJigEntity->PostToModelSpace();
		return true;
	}
	else
	{
		return false;
	}
}

AcEdJig::DragStatus CLineJig::sampler()
{
	setUserInputControls((UserInputControls)(AcEdJig::kAccept3dCoordinates | AcEdJig::kNoNegativeResponseAccepted | AcEdJig::kNullResponseAccepted));
	static AcGePoint3d pointTemp;
	DragStatus stat = acquirePoint(m_currentPt);
	if (pointTemp != m_currentPt)
	{
		pointTemp = m_currentPt;
	}
	else if (stat == AcEdJig::kNormal)
	{
		return AcEdJig::kNoChange;
	}
	return stat;
}

Adesk::Boolean CLineJig::update()
{
// 	AcGePoint3d pt1=m_pLine->startPoint();
// 	AcGePoint3d pt2=m_pLine->endPoint();
// 	m_pLine->setStartPoint(AcGePoint3d(m_currentPt.x, pt1.y, pt1.z));
// 	m_pLine->setEndPoint(AcGePoint3d(m_currentPt.x, pt2.y, pt2.z));
	m_pLineJigEntity->SetLinePath(m_currentPt);
	return Adesk::kTrue;
}

AcDbEntity * CLineJig::entity() const
{
	return m_pLineJigEntity;
}
#include "StdAfx.h"
#include "BlockJig.h"
#include "AllHead.h"


CBlockJig::CBlockJig()
{
	m_pRef = NULL;
}


CBlockJig::~CBlockJig()
{
}

bool CBlockJig::doIt(AcDbObjectId &idBlock)
{
	m_pRef = new AcDbBlockReference(AcGePoint3d(0, 0, 0), idBlock);
	CString prompt = TEXT("\n指定插入点:");
	AcEdJig::DragStatus stat = drag();
	if (stat==kNormal)
	{
		CDwgDatabaseUtil::PostToModelSpace(m_pRef);
		return true;
	}
	else
	{
		m_pRef->close();
		delete m_pRef;
		return false;
	}
}

AcEdJig::DragStatus CBlockJig::sampler()
{
	setUserInputControls((UserInputControls)(AcEdJig::kAccept3dCoordinates | AcEdJig::kNoNegativeResponseAccepted | AcEdJig::kNullResponseAccepted));
	static AcGePoint3d pointTemp;
	DragStatus stat = acquirePoint(m_currentPt);
	if (pointTemp!=m_currentPt)
	{
		pointTemp = m_currentPt;
	}
	else if (stat==AcEdJig::kNormal)
	{
		return AcEdJig::kNoChange;
	}
	return stat;
}

Adesk::Boolean CBlockJig::update()
{
	m_pRef->setPosition(m_currentPt);
	return Adesk::kTrue;
}

AcDbEntity * CBlockJig::entity() const
{
	return m_pRef;
}
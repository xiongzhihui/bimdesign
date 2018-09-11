#pragma once
//#include "C:\Program Files (x86)\ZrxWizard\ZRXSDK\2015\inc\zdbjig.h"
class CBlockJig :
	public AcEdJig
{
public:
	CBlockJig();
	virtual~CBlockJig();

	bool doIt(AcDbObjectId &idBlock);

	virtual AcEdJig::DragStatus sampler();

	virtual Adesk::Boolean update();

	virtual AcDbEntity * entity() const;
private:
	AcDbBlockReference *m_pRef;
	AcGePoint3d m_currentPt;
};


#pragma once

class CLineJigEntity: public AcDbEntity
{
public:
	CLineJigEntity(AcDbObjectId Polylineid,double dLen,int iNum);
	virtual~CLineJigEntity();

	virtual Adesk::Boolean worldDraw(AcGiWorldDraw * mode);

	void SetLinePath(AcGePoint3d pt);

	void PostToModelSpace();

	AcDbObjectId GetBlkRefIds();

private:
	void DrawOrAddSubEnts(AcGiWorldDraw* mode);

	AcGePoint3d m_ptLinePath;

	AcDbObjectId m_polylineid;
	AcDbPolyline *m_pPolyline;
	double m_dLen;
	int m_iNum;
};


class CLineJig : public AcEdJig 
{
public:
	CLineJig();
	virtual~CLineJig();

	bool doIt(AcDbObjectId Polylineid,double dLen,int iNum);

	virtual AcEdJig::DragStatus sampler();

	virtual Adesk::Boolean update();

	virtual AcDbEntity * entity() const;
private:

	AcGePoint3d m_currentPt;
	CLineJigEntity *m_pLineJigEntity;
};


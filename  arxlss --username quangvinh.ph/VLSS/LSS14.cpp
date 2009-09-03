#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"

#include "Logger.h"

void LSS14()
{
	createPolyline(10);

	AcDbEntity* pEntity= NULL;
	if ((pEntity = selectEntity(AcDb::kForRead))) {
		AcDbObjectId idEntity = pEntity->objectId();
		pEntity->close();
		iteratePolyline(idEntity);
	}
}

void createPolyline(const int nPoints)
{
	AcGePoint3dArray p3aPolyline;
	p3aPolyline.setLogicalLength(nPoints);
	for (int i = 0; i < nPoints; i++) {
		p3aPolyline[i].set((double)(i/2), (double)(i%2), 0.0);
	}

	AcDb2dPolyline* pPolyline = new AcDb2dPolyline(AcDb::k2dSimplePoly, p3aPolyline, 0.0, Adesk::kTrue);
	pPolyline->setColorIndex(13);

	AcDbObjectId idPolyline;
	if (!addToModelSpace(idPolyline, pPolyline)) {
		delete pPolyline;
		return;
	}

	pPolyline->setLayer(L"0");
	pPolyline->close();
}

void iteratePolyline(AcDbObjectId idPLine)
{
	AcDb2dPolyline* pPline = NULL;
	acdbOpenObject(pPline, idPLine, AcDb::kForRead);
	
	AcDbObjectIterator* pIter = pPline->vertexIterator();
	pPline->close();
	
	for (int nIndex = 0; !pIter->done(); nIndex++, pIter->step()) {
		AcDbObjectId idVertex = pIter->objectId();
		AcDb2dVertex *p2DVertex = NULL;
		acdbOpenObject(p2DVertex, idVertex, AcDb::kForRead);
		AcGePoint3d pt3Location = p2DVertex->position();
		p2DVertex->close();
		acutPrintf(L"%d. [%0.3f, %0.3f, %0.3f]", nIndex, pt3Location.x, pt3Location.y, pt3Location.z);
		CLogger::Print(L"%d. [%0.3f, %0.3f, %0.3f]", nIndex, pt3Location.x, pt3Location.y, pt3Location.z);
	}
	delete pIter;
}

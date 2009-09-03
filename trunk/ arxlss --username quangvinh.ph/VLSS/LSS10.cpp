#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"

#include "Logger.h"

void LSS10()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 10 |--------------"));
	AcDbObjectId idCircle;
	Acad::ErrorStatus es;
	if (Acad::eOk != (es = createCircle(idCircle))) {
		acutPrintf(ACRX_T("Fail to call createCircle() function - Error: %s")
									, acadErrorStatusText(es));
	}
}

Acad::ErrorStatus createCircle(AcDbObjectId & idCircle)
{
	CLogger::Print(L"*Call: createCircle()");
	Acad::ErrorStatus es, esTmp;

	AcDbBlockTable* pBlockTable = NULL;
	es = acdbHostApplicationServices()->workingDatabase()
				->getSymbolTable(pBlockTable, AcDb::kForRead);
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: createCircle() - Fail to get the BlockTable.");
		return es;
	}

	AcDbBlockTableRecord* pModelSpace = NULL;
	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForWrite);
	if (Acad::eOk != (esTmp =  pBlockTable->close())) {
		CLogger::Print(L"Warn: Fail to close the BlockTable!");
		acrx_abort(ACRX_T("\nThere is an error occured when close the BlockTable. Message: %s")
									, acadErrorStatusText(esTmp));
	}
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: createCircle() - Fail to get the Model Space! Error: %s", acadErrorStatusText(es));
		return es;
	}

	idCircle = AcDbObjectId::kNull;
	AcGePoint3d pt3Center(9.0, 3.0, 0.0);
	AcGeVector3d vt3Normal(0.0, 0.0, 1.0);
	AcDbCircle* pCircle = new AcDbCircle(pt3Center, vt3Normal, 10.0);

	if (!pCircle) {
		if (Acad::eOk != (esTmp = pModelSpace->close())) {
			CLogger::Print(L"Warn: Fail to create new circle object!");
			acrx_abort(ACRX_T("\nThere is an error occured. Error: %s")
											, acadErrorStatusText(esTmp));
		}
		return Acad::eOutOfMemory;
	}

	es = pModelSpace->appendAcDbEntity(idCircle, pCircle);
	if (Acad::eOk != (esTmp = pModelSpace->close())) {
		CLogger::Print(L"Warn: Fail to close the Model Space!");
		acrx_abort(ACRX_T("\nThere is an error occured when close the Model Space! Error: %s")
										, acadErrorStatusText(esTmp));
	}
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: createCircle() - Fail to append new circle in to Model Space!");
		delete pCircle;
		return es;
	}

	if (Acad::eOk != (esTmp = pCircle->close())) {
		CLogger::Print(L"Warn: Fail to close the circle object.");
		acrx_abort(ACRX_T("\nFail to close the circle entity!, Error: %s")
										, acadErrorStatusText(esTmp));
	}

	CLogger::Print(L"*Exit: createCircle()");
	return Acad::eOk;
}

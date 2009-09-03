/********************************************************************
	File Name:		LSS13.h
	Author:			Pham Quang Vinh
	Purpose:		
*********************************************************************/

#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"
#include "Logger.h"

void LSS13()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 13 |--------------"));

	Acad::ErrorStatus es;
	ACHAR szBlockName[50] = L"\0";
	
	//------------
	// Require to enter the BlockTable name.
	if (RTNORM != acedGetString(Adesk::kTrue, L"\nEnter block name <CR for current space>", szBlockName)) {
		return;
	}

	//------------
	// If didn't enter the BlockTableRecord name get the current viewing space name.
	if (szBlockName[0] == L'\0' || 0 == lstrcmp(szBlockName, L"cr")) {
		if (Adesk::kFalse == acdbHostApplicationServices()->workingDatabase()->tilemode()) {
			CLogger::Print(L"Inform: TILEMODE = FALSE - Select the current viewing workspace.");
			struct resbuf rsb;
			acedGetVar(L"cvport", &rsb);
			if (rsb.resval.rint == 1)
				lstrcpy(szBlockName, ACDB_PAPER_SPACE);
			else
				lstrcpy(szBlockName, ACDB_MODEL_SPACE);
		} else {
			lstrcpy(szBlockName, ACDB_MODEL_SPACE);
		}
	}

	//------------
	// Open the specified BlockTableRecord for read.
	AcDbBlockTable* pBlockTable = NULL;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: Fail to get the BlockTable!");
		return;
	}
	
	AcDbBlockTableRecord* pBlockTableRecord = NULL;
	es = pBlockTable->getAt(szBlockName, pBlockTableRecord, AcDb::kForRead);
	pBlockTable->close();
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: Fail to get the BlockTableRecord!");
		return;
	}

	//------------
	// Get BlockTableRecord's Iterator then steps through BlockTableRecords.
	AcDbBlockTableRecordIterator* pBlockTableRecordIter = NULL;
	es = pBlockTableRecord->newIterator(pBlockTableRecordIter);
	pBlockTableRecord->close();
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: Fail to get the BlockTableRecord's Iterator.");
		return;
	}

	for (int nCount = 0; !pBlockTableRecordIter->done(); pBlockTableRecordIter->step(), nCount++) {
		AcDbEntity* pEntity = NULL;
		es = pBlockTableRecordIter->getEntity(pEntity, AcDb::kForRead);
		
		AcDbHandle hndEntity;
		pEntity->getAcDbHandle(hndEntity);

		ACHAR szHandle[20] = L"\0";
		hndEntity.getIntoAsciiBuffer(szHandle);
		
		const ACHAR* pszName = pEntity->isA()->name();
		acutPrintf(L"\nId = %lx, Handle = %s, Class = %s", pEntity->objectId(), szHandle, pszName);
		CLogger::Print(L"%d. Id = %lx, Handle = %s, Class = %s", nCount, pEntity->objectId(), szHandle, pszName);
		pEntity->close();
	}

	delete pBlockTableRecordIter;
	CLogger::Print(L"*Exit");
}

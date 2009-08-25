#ifndef LSS13_H
#define LSS13_H

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

void LSS13()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 13 |--------------"));

	Acad::ErrorStatus es;
	ACHAR szBlockName[50] = L"\0";
	
	if (RTNORM != acedGetString(Adesk::kTrue, L"\nEnter block name <CR for current space>", szBlockName)) {
		return;
	}

	if (szBlockName[0] == L'\0') {
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

	CLogger::Print(L"Infrom: selected workspace '%s'", szBlockName);
	AcDbBlockTable* pBlockTable = NULL;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	
	AcDbBlockTableRecord* pBlockTableRecord = NULL;
	es = pBlockTable->getAt(szBlockName, pBlockTableRecord, AcDb::kForRead);
	pBlockTable->close();

	AcDbBlockTableRecordIterator* pBlockTableRecordIter = NULL;
	es = pBlockTableRecord->newIterator(pBlockTableRecordIter);

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
	pBlockTableRecord->close();
	CLogger::Print(L"*Exit");
}

#endif
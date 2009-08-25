#ifndef __LSS07_H
#define __LSS07_H

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

void browseAllAttributes(AcDbDatabase* pDb);

void LSS07()
{
	CLogger::Print(_T("--------------------------| START LOGGING LESSON 07 |-----------------------------"));
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	browseAllAttributes(pDb);
}

void browseAllAttributes(AcDbDatabase* pDb)
{
	CLogger::Print(_T("*Call: browseAllAttributes()"));
	CLogger::Print(_T("Inform: Lay het tat ca block reference's objectId."));
	Acad::ErrorStatus es;
	AcDbObjectIdArray idaBlockRefs;
	int nBlockRefCount = getBlockRefAll(pDb, idaBlockRefs);

	CLogger::Print(_T("Inform: Duyet qua het tat ca Ids lay duoc. [%d]"), nBlockRefCount);
	for (int nIdx = 0; nIdx < nBlockRefCount; nIdx++) {
		CLogger::Print(_T("---------[STT = %d]----------"), nIdx);
		AcDbBlockReference* pBlockRef = NULL;
		AcDbObjectId idBlockRef = idaBlockRefs[nIdx];

		es = acdbOpenObject(pBlockRef, idBlockRef, AcDb::kForRead);
		if (Acad::eOk != es) {
			CLogger::Print(ACRX_T("Error: Mo BlockReference bi loi! > Bo Qua"), nIdx);
			continue;
		}
		
		CLogger::Print(ACRX_T("Inform: Lay tat ca attribute iterator."));
		AcDbObjectIterator* pBlockRefIter = pBlockRef->attributeIterator();
		for (; !pBlockRefIter->done(); pBlockRefIter->step()) 
		{
			AcDbObjectId idObject = pBlockRefIter->objectId();
			AcDbObject* pObject = NULL;
			es = acdbOpenAcDbObject(pObject, idObject, AcDb::kForRead);
			if (Acad::eOk != es) {
				CLogger::Print(ACRX_T("Warn: Open Attribute bi loi > Bo Qua."));
				continue;
			}

			AcDbAttribute* pAttribute = AcDbAttribute::cast(pObject);
			if (pAttribute) {
				ACHAR* szTag = pAttribute->tag();
				CLogger::Print(ACRX_T("Tag = %s"), szTag);
			} else {
				CLogger::Print(ACRX_T("Warn: Convert object thanh attribute khong duoc!"));
			}

			pObject->close();
		}

		delete pBlockRefIter;
		pBlockRef->close();
	}

	CLogger::Print(ACRX_T("*Exit: browseAllAttributes()"));
}

#endif
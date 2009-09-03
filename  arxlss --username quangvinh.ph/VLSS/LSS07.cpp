#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"

#include "Logger.h"

void LSS07()
{
	CLogger::Print(_T("--------------------------| START LOGGING LESSON 07 |-----------------------------"));
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	browseAllAttributes(pDb);
}

void browseAllAttributes(AcDbDatabase* pDb)
{
	CLogger::Print(_T("*Call: browseAllAttributes()"));
	Acad::ErrorStatus es;
	AcDbObjectIdArray idaBlockRefs;
	int nBlockRefCount = getBlockRefAll(pDb, idaBlockRefs);

	for (int nIdx = 0; nIdx < nBlockRefCount; nIdx++) {
		CLogger::Print(_T("---------[STT = %d]----------"), nIdx);
		AcDbBlockReference* pBlockRef = NULL;
		AcDbObjectId idBlockRef = idaBlockRefs[nIdx];

		es = acdbOpenObject(pBlockRef, idBlockRef, AcDb::kForRead);
		if (Acad::eOk != es) {
			CLogger::Print(ACRX_T("Warn: Fail to open BlockReference! > Ignore"), nIdx);
			continue;
		}
		
		AcDbObjectIterator* pBlockRefIter = pBlockRef->attributeIterator();
		for (; !pBlockRefIter->done(); pBlockRefIter->step()) 
		{
			AcDbObjectId idObject = pBlockRefIter->objectId();
			AcDbObject* pObject = NULL;
			es = acdbOpenAcDbObject(pObject, idObject, AcDb::kForRead);
			if (Acad::eOk != es) {
				CLogger::Print(ACRX_T("Warn: Fail to open the BlockReference's Attribute. > Ignore."));
				continue;
			}

			AcDbAttribute* pAttribute = AcDbAttribute::cast(pObject);
			if (pAttribute) {
				ACHAR* szTag = pAttribute->tag();
				CLogger::Print(ACRX_T("Tag = %s"), szTag);
			} else {
				CLogger::Print(ACRX_T("Warn: Fail to convert object to Attribute object.!"));
			}

			pObject->close();
		}

		delete pBlockRefIter;
		pBlockRef->close();
	}

	CLogger::Print(ACRX_T("*Exit: browseAllAttributes()"));
}

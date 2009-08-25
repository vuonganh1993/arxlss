
#ifndef FUNCS_H
#define FUNCS_H

#include "StdAfx.h"
#include "Resource.h"
#include "Logger.h"

/************************************************************************/
/* PROTOTYPES ARE DECLARED HERE
/************************************************************************/

int getBlockRefAll(AcDbObjectIdArray& idaAll);
int getBlockRefAll(AcDbDatabase* pDb, AcDbObjectIdArray& idaAll);
AcDbObject* selectObject(AcDb::OpenMode mode);
AcDbEntity* selectEntity(AcDb::OpenMode mode);
bool selectPoint(AcGePoint3d&);
void printList(struct resbuf*  pRb);
AcApDocument* findDocument(LPCASTR pszFileName);
bool addToModelSpace(AcDbObjectId&, AcDbEntity*);

/************************************************************************/
/*  FUNCTIONS ARE IMPLEMENTED HERE
/************************************************************************/

int getBlockRefAll(AcDbObjectIdArray& idaAll)
{
	Acad::ErrorStatus esResult;
	idaAll.setLogicalLength(0);

	// Get the current working database.
	AcDbDatabase* pDb = acDocManager->curDocument()->database();

	// Get the BlockTable.
	AcDbBlockTable* pBlockTable = NULL;
	esResult = pDb->getBlockTable(pBlockTable, AcDb::kForRead);
	if (Acad::eOk != esResult)
		return 0;

	// Get the Model Space.
	AcDbBlockTableRecord* pModelSpace = NULL;
	esResult = pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForRead);
	pBlockTable->close();
	if (Acad::eOk != esResult)
		return 0;

	// Get the Iterator of ModelSpace
	AcDbBlockTableRecordIterator* pRcdIter = NULL;
	esResult = pModelSpace->newIterator(pRcdIter);
	pModelSpace->close();
	if (Acad::eOk != esResult)
		return 0;

	for (; !pRcdIter->done(); pRcdIter->step()) {
		AcDbEntity* pEntity = NULL;
		if (Acad::eOk != (pRcdIter->getEntity(pEntity, AcDb::kForRead)))
			continue;

		if (pEntity->isKindOf(AcDbBlockReference::desc())) {
			AcDbObjectId idObj = pEntity->objectId();
			idaAll.append(idObj);
		}

		pEntity->close();
	}

	delete pRcdIter;
	return idaAll.length();
}



AcDbObject* selectObject(AcDb::OpenMode mode)
{
	ads_name adsName;
	ads_point adsPoint;
	if (RTNORM != acedEntSel(ACRX_T("\nSpecify an entity: "), adsName, adsPoint))
		return NULL;

	AcDbObject* pObj = NULL;
	AcDbObjectId idObj = AcDbObjectId::kNull;

	if (Acad::eOk != acdbGetObjectId(idObj, adsName))
		return NULL;

	if( Acad::eOk != acdbOpenObject(pObj, idObj, mode))
		return NULL;

	return pObj;
}

AcDbEntity* selectEntity(AcDb::OpenMode mode)
{
	ads_name adsName;
	ads_point adsPoint;
	if (RTNORM != acedEntSel(L"\nSpecify an entity:", adsName, adsPoint)) {
		return NULL;
	}

	AcDbEntity *pEntity = NULL;
	AcDbObjectId idEntity = AcDbObjectId::kNull;

	if (Acad::eOk != acdbGetObjectId(idEntity, adsName)) {
		return NULL;
	}

	if (Acad::eOk != acdbOpenObject(pEntity, idEntity, mode)) {
		return NULL;
	}

	return pEntity;
}

void printList(struct resbuf*  pRb)
{
	resbuf* pRbTmp = pRb;
	while (pRbTmp && pRbTmp->resval.rstring) {
		CLogger::Print(_T("Inform: '%s'"), pRbTmp->resval.rstring);
		acutPrintf(ACRX_T("\n%s"), pRbTmp->resval.rstring);
		pRbTmp = pRbTmp->rbnext;
	}
}

AcApDocument* findDocument(LPCASTR pszFileName)
{
	AcApDocument* pCurrDoc = acDocManager->curDocument();
	if (pCurrDoc) {
		LPCASTR pszTmp = pCurrDoc->fileName();
		if (0 == lstrcmp(pszFileName, pszTmp))
			return pCurrDoc;
	}

	AcApDocumentIterator* pDocIter = acDocManager->newAcApDocumentIterator();
	for (; !pDocIter->done(); pDocIter->step()) {
		AcApDocument* pDoc = pDocIter->document();
		LPCASTR pszTmp = pDoc->fileName();
		if (0 == lstrcmp(pszFileName, pszTmp)) {
			delete pDocIter;
			return pDoc;
		}
	}

	delete pDocIter;
	return NULL;
}

int getBlockRefAll(AcDbDatabase* pDb,	 AcDbObjectIdArray& idaAll)
{
	CLogger::Print(L"*Call: getBlockRefAll()");
	if (!pDb) return 0;

	Acad::ErrorStatus esResult;
	idaAll.setLogicalLength(0);

	CLogger::Print(L"Inform: get the block table.");
	AcDbBlockTable* pBlockTable = NULL;
	esResult = pDb->getBlockTable(pBlockTable, AcDb::kForRead);
	if (Acad::eOk != esResult) {
		CLogger::Print(L"*Exit: getBlockRefAll() - Fail to get the block table.");
		return 0;
	}

	CLogger::Print(L"Inform: get the Model Space.");
	AcDbBlockTableRecord* pModelSpace = NULL;
	esResult = pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForRead);
	pBlockTable->close();
	if (Acad::eOk != esResult) {
		CLogger::Print(L"*Exit: getBlockRefAll() - Fail to get the model space!");
		return 0;
	}

	CLogger::Print(L"Inform: get the Model Space Iterator.");
	AcDbBlockTableRecordIterator* pRecordIter = NULL;
	esResult = pModelSpace->newIterator(pRecordIter);
	pModelSpace->close();
	if (Acad::eOk != esResult) {
		CLogger::Print(L"*Exit: getBlockRefAll() - Fail to get Model Space Iterator!");
		return 0;
	}

	CLogger::Print(L"Inform: browse all block reference to get its ObjectId");
	int nIndex = 0;
	for (; !pRecordIter->done(); pRecordIter->step()) {
		AcDbEntity* pEntity = NULL;
		esResult = pRecordIter->getEntity(pEntity, AcDb::kForRead);
		if (Acad::eOk != esResult) {
			CLogger::Print(L"Warn: fail to get the entity. > Ignore");
			continue;
		}

		if (pEntity->isKindOf(AcDbBlockReference::desc())) {
			idaAll.append(pEntity->objectId());
		} else {
			//CLogger::Print(L"Warn: %d. the entity is not an Block Reference!", nIndex);
		}
		nIndex++;
		pEntity->close();
	}

	delete pRecordIter;
	CLogger::Print(L"*Exit: getBlockRefAll()");
	return idaAll.length();
}


bool addToModelSpace(AcDbObjectId& idEntity, AcDbEntity* pEntity)
{
	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != acdbHostApplicationServices()->workingDatabase()
							->getSymbolTable(pBlockTable, AcDb::kForRead)) 
	{
		return false;
	}

	AcDbBlockTableRecord *pModelSpace = NULL;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForWrite)) {
		pBlockTable->close();
		return false;
	}
	pBlockTable->close();

	if (Acad::eOk != pModelSpace->appendAcDbEntity(idEntity, pEntity)) {
		pModelSpace->close();
		return false;
	}
	pModelSpace->close();

	return true;
}

bool selectPoint(AcGePoint3d& pt3Out)
{
	ads_point pnt;
	if (RTNORM != acedGetPoint(NULL, L"Please specify a position.", pnt)) {
		return false;
	}
	pt3Out = AcGePoint3d(pnt[0], pnt[1], pnt[2]);
	return true;
}
#endif
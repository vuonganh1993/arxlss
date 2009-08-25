#ifndef LSS11_H_
#define LSS11_H_

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

Acad::ErrorStatus
getObjectAndGsMarker(AcDbObjectId& idObj, int& nMaker);

void
highlightEdge(const AcDbObjectId& idObj, const int nMaker);


void LSS11()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 11 |--------------"));
	AcDbObjectId idObj;
	int nMaker;
	if (Acad::eOk == getObjectAndGsMarker(idObj, nMaker)) {
		highlightEdge(idObj, nMaker);
	}
}

Acad::ErrorStatus
getObjectAndGsMarker(AcDbObjectId& idObj, int& nMaker)
{
	CLogger::Print(L"*Call: getObjectAndGsMarker()");
	ads_name nameSSet;

	CLogger::Print(L"Inform: require to select a subset, then get the subset name.");
	if (RTNORM != acedSSGet(L"_:S", NULL, NULL, NULL, nameSSet)) {
		CLogger::Print(L"Warn: Fail to select subset!");
		return Acad::eInvalidAdsName;
	}

	CLogger::Print(L"Inform: get the information of the set into an resbuf!");
	struct resbuf *pRb;
	if (RTNORM != acedSSNameX(&pRb, nameSSet, 0)) {
		acedSSFree(nameSSet);
		return Acad::eAmbiguousOutput;
	}
	acedSSFree(nameSSet); // release the subset name now.
	
	CLogger::Print(L"Inform: get the pointer to next 3 steps of resbuf.");
	struct resbuf *pRbTmp;
	int nIdx;
	for (nIdx = 0, pRbTmp = pRb; nIdx < 3; nIdx++, pRbTmp = pRbTmp->rbnext) {}

	CLogger::Print(L"Inform: get the entity name!");
	ads_name adsEName;
	ads_name_set(pRbTmp->resval.rlname, adsEName);
	pRbTmp = pRbTmp->rbnext;
	nMaker = pRbTmp->resval.rint;
	acutRelRb(pRb);
	acdbGetObjectId(idObj, adsEName);

	CLogger::Print(L"*Exit: getObjectAndGsMarker() - Entity Name: %d - GS Maker: %d", adsEName, nMaker);
	return Acad::eOk;
}

void
highlightEdge(const AcDbObjectId& idObj, const int nMaker)
{
	CLogger::Print(L"*Call: highlightEdge()");
	ACHAR dummy[133];
	AcDbEntity* pEntity;
	CLogger::Print(L"Inform: Open entity.");
	acdbOpenAcDbEntity(pEntity, idObj, AcDb::kForRead);

	AcGePoint3d pt3Pick;
	AcGeMatrix3d mx3Formal;
	int nIdNums;
	AcDbFullSubentPath* pSubentIds;

	CLogger::Print(L"Inform: get entity's subentities");
	pEntity->getSubentPathsAtGsMarker(AcDb::kEdgeSubentType
		, nMaker, pt3Pick, mx3Formal, nIdNums, pSubentIds);

	CLogger::Print(L"Inform: high light the subentity has index 0.");
	if (nIdNums > 0) {
		pEntity->highlight(pSubentIds[0]);
		acedGetString(0, L"\nPress <return> to continue...", dummy);
		pEntity->unhighlight(pSubentIds[0]);
		AcDbEntity* pEntCopy = pEntity->subentPtr(pSubentIds[0]);
		AcDbObjectId idObj;
		addToModelSpace(idObj, pEntCopy);
	}

	delete [] pSubentIds;
	pEntity->close();
	CLogger::Print(L"*Exit: highlightEdge()");
}
#endif
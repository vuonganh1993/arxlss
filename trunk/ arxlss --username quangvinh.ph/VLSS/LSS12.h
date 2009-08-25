/********************************************************************
	File Name:		LSS12.h
	Author:			Pham Quang Vinh
	Purpose:		Practice for creating BlockTableRecord, and copying BlockReference
*********************************************************************/

#ifndef LSS12_H_
#define LSS12_H_

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

/****************************
 *	Create a simple BlockTableRecord, and append an entity
 * into it.
 */
bool makeABlock(void);

/****************************
 *	Copy a BlockReference.
 */
bool copyBlockRef(AcDbBlockReference*, AcGePoint3d, AcDbObjectId&);

void LSS12(void)
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 12 |--------------"));
	//makeABlock();

	AcDbEntity* pEntity = selectEntity(AcDb::kForRead);
	if (!pEntity) {
		CLogger::Print(L"*Exit: Fail to select an entity.");
		return;
	}

	AcGePoint3d pt3To;
	if (!selectPoint(pt3To)) {
		CLogger::Print(L"*Exit: Fail to specify a position for copying to!");
		pEntity->close();
		return;
	}

	AcDbBlockReference* pBlockRefSrc = NULL;
	if (!(pBlockRefSrc = AcDbBlockReference::cast(pEntity))) {
		CLogger::Print(L"*Exit: Fail to cast entity to BlockReference object.");
		pEntity->close();
		return;
	}

	AcDbObjectId idBlockRefDsc;
	if (!copyBlockRef(pBlockRefSrc, pt3To, idBlockRefDsc)) {
		CLogger::Print(L"*Exit: Fail to call copyBlockRef()");
		pEntity->close();
		return;
	}

	pEntity->close();
}

bool makeABlock(void)
{
	CLogger::Print(L"*Call: makeABlock()");

	// Get current working database's BlockTable.
	Acad::ErrorStatus es;
	AcDbBlockTable* pBlockTable = NULL;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForWrite);
	if (Acad::eOk != es) {
		return false;
	}

	// Create new BlockTableRecord object.
	AcDbBlockTableRecord* pNewRecord = new AcDbBlockTableRecord;
	pNewRecord->setName(L"SGN-NEW-BLOCK");

	// Append new BlockTableRecord into BlockTable object.
	AcDbObjectId idNewRecord = AcDbObjectId::kNull;
	es = pBlockTable->add(idNewRecord, pNewRecord);
	pBlockTable->close();
	if (Acad::eOk != es) {
		return false;
	}

	// Create new Line object.
	AcDbLine* pLine = new AcDbLine;
	pLine->setStartPoint(AcGePoint3d(3, 3, 0));
	pLine->setEndPoint(AcGePoint3d(20, 60, 0));
	pLine->setColorIndex(13);

	// Append new Line into new BlockTableRecord!
	AcDbObjectId idLine;
	es = pNewRecord->appendAcDbEntity(idLine, pLine);
	pLine->close();
	pNewRecord->close();
	if (Acad::eOk != es) {
		return false;
	}

	CLogger::Print(L"*Exit: makeABlock()");
	return true;
}

bool
copyBlockRef(AcDbBlockReference* pBlockRefSrc, AcGePoint3d pt3To, AcDbObjectId& idBlockRefDsc)
{
	CLogger::Print(L"*Call: copyBlockRef().");
	if (!pBlockRefSrc) {
		return false;
	}

	Acad::ErrorStatus es;
	AcDbObjectId idDefRecord = pBlockRefSrc->blockTableRecord();

	AcDbBlockReference* pBlockRefDsc = new AcDbBlockReference;
	if ((Acad::eOk != pBlockRefDsc->setPosition(pBlockRefSrc->position()))
		|| (Acad::eOk != pBlockRefDsc->setRotation(pBlockRefSrc->rotation()))
		|| (Acad::eOk != pBlockRefDsc->setBlockTableRecord(idDefRecord))
		|| (Acad::eOk != pBlockRefDsc->setPropertiesFrom(pBlockRefSrc))
		|| (Acad::eOk != pBlockRefDsc->setScaleFactors(pBlockRefSrc->scaleFactors()))
		|| (Acad::eOk != pBlockRefDsc->setColor(pBlockRefSrc->color()))) 
	{
		CLogger::Print(L"*Exit: copyBlockRef() - Fail to set up the new BlockReference's properties!");
		delete pBlockRefDsc;
		return false;
	}

	AcGeMatrix3d mx3Transform = pBlockRefDsc->blockTransform();
	AcGeVector3d vc3Translate(pt3To.x, pt3To.y, pt3To.z);
	mx3Transform.setTranslation(vc3Translate);
	pBlockRefDsc->setBlockTransform(mx3Transform);

	AcDbBlockTableRecord* pDefRecord = NULL;
	es = acdbOpenObject(pDefRecord, idDefRecord, AcDb::kForRead);
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: copyBlockRef() - Fail to open the Definition Record!");
		delete pBlockRefDsc;
		return false;
	}

	AcDbBlockTableRecordIterator* pDefRecordIter = NULL;
	es = pDefRecord->newIterator(pDefRecordIter);
	pDefRecord->close();
	if (Acad::eOk != es) {
		CLogger::Print(L"*Exit: copyBlockRef() - Fail to get the Definition Record Iterator.");
		delete pBlockRefDsc;
		return false;
	}

	for (; !pDefRecordIter->done(); pDefRecordIter->step()) {
		AcDbEntity* pEntity = NULL;
		AcDbAttributeDefinition* pAttDef = NULL;
		if (Acad::eOk != pDefRecordIter->getEntity(pEntity, AcDb::kForRead)) {
			CLogger::Print(L"Warn: Fail to get the entity from the Iterator. > Ignore");
			continue;
		}

		if (!(pAttDef = AcDbAttributeDefinition::cast(pEntity))) {
			CLogger::Print(L"Warn: Fail to cast the entity to AttributeDefinition object. > Ignore");
			pEntity->close();
			continue;
		}

		AcDbAttribute* pAttr = new AcDbAttribute;
		ACHAR* pszTag = pAttDef->tag();
		if ((Acad::eOk != pAttr->setPropertiesFrom(pAttDef))
			|| (Acad::eOk != pAttr->setInvisible(pAttDef->isInvisible()))
			|| (Acad::eOk != pAttr->setVerticalMode(pAttDef->verticalMode()))
			|| (Acad::eOk != pAttr->setHorizontalMode(pAttDef->horizontalMode()))
			|| (Acad::eOk != pAttr->setHeight(pAttDef->height()))
			|| (Acad::eOk != pAttr->setRotation(pAttDef->rotation()))
			|| (Acad::eOk != pAttr->setFieldLength(pAttDef->fieldLength()))
			|| (Acad::eOk != pAttr->setTextStyle(pAttDef->textStyle()))
			|| (Acad::eOk != pAttr->setWidthFactor(pAttDef->widthFactor()))
			|| (Acad::eOk != pAttr->setTag(pszTag)))
		{
			CLogger::Print(L"Warn: Fail to setup the Attribute properties. > Ignore");
			acutDelString(pszTag);
			pEntity->close();
			delete pAttr;
			continue;
		}
		
		ACHAR* pszText = NULL;
		DenkiSymbolSnapshot dss;
		if (dss.open(pBlockRefSrc, L"*")) {
			if (dss.getExpandedValue(pszTag, pszText)) {
				pAttr->setTextString(pszText);
				dss.freeString(pszText);
			}
			dss.close();
		} else {
			CLogger::Print(L"Warn: Cannot open DenkiSymbolSnapshot object.");
		}
		acutDelString(pszTag);

		if (Acad::eOk != pBlockRefDsc->appendAttribute(pAttr)) {
			CLogger::Print(L"Warn: Fail to append the new Attribute into the new BlockReference.");
			delete pAttr;
			pEntity->close();
			continue;
		}

		if (Acad::eOk != pAttr->setPosition(pAttDef->position())) {
			CLogger::Print(L"Warn: Fail to set attribute position.");
		}
		if (Acad::eOk != pAttr->setAlignmentPoint(pAttDef->alignmentPoint())) {
			CLogger::Print(L"Warn: Fail to set attribute AlignmentPoint");
		}
		if (Acad::eOk != pAttr->transformBy(pBlockRefDsc->blockTransform())) {
			CLogger::Print(L"Warn: Fail to call transformBy function!");
		}

		pAttr->close();
		pEntity->close();
	}
	delete pDefRecordIter;

	if (!addToModelSpace(idBlockRefDsc, pBlockRefDsc)) {
		CLogger::Print(L"*Exit: Fail to add the new Block Reference into Model Space!");
		delete pBlockRefDsc;
		return false;
	}

	pBlockRefDsc->close();
	CLogger::Print(L"*Exit: copyBlockRef().");
	return true;
}
#endif
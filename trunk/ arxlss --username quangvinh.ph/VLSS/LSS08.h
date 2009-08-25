/********************************************************************
	File Name:		LSS08.h
	Author:			Pham Quang Vinh

	Purpose:		Purge the database's BlockTable, LayerTable, 
					LinetypeTable, TextStyleTable.
*********************************************************************/

#ifndef LSS08_H_
#define LSS08_H_

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"
#include "LayerState.h"

/****************************
 * Purpose:	get all TableRecord's ObjectId to an AcDbObjectIdArray
 *				then return number of TableRecords.
 *
 * pTableClass:	Specify the kind of SymbolTable that we are going to
 *						work on.
 */
int getAllSymbolRecordsIds(AcRxClass* pTableClass, AcDbObjectIdArray& idaAll);

/****************************
 * Purpose:	Purge a SymbolTable.
 *
 * pTableClass:	Specify the kind of SymbolTable that we are going to
 *				purge.
 */
bool purgeSymbolTable(AcRxClass* pTableClass);

void LSS08()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 08 |--------------"));
	while (purgeSymbolTable(AcDbBlockTable::desc())
		|| purgeSymbolTable(AcDbLayerTable::desc())
		|| purgeSymbolTable(AcDbLinetypeTable::desc())
		|| purgeSymbolTable(AcDbTextStyleTable::desc()));
}


bool purgeSymbolTable(AcRxClass* pTableClass)
{
	CLogger::Print(_T("*Call: purgeSymbolTable()"));

	Acad::ErrorStatus es;
	AcDbObjectIdArray idaAll;
	getAllSymbolRecordsIds(pTableClass, idaAll);

	//------------
	// Filter SymbolTableRecord's ObjectIds that is useless.
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	es = pDb->purge(idaAll);

	if (Acad::eOk != es) {
		CLogger::Print(_T("*Exit: purgeSymbolTable() - Fail to get the ObjectIds that need to purge!"));
		return false;
	}

	//------------
	// If the kind of SymbolTable is AcDbLayerTable, Unlock all its records.
	CAllLayerState stateLayers;
	if (AcRx::kEqual == pTableClass->comparedTo(AcDbLayerTable::desc())) {
		stateLayers.memorize(pDb);
		stateLayers.unlock();
	}

	int nLength = idaAll.length();
	if (nLength <= 0) {
		CLogger::Print(_T("*Exit: purgeSymbolTable() - There is nothing to purge!"));
		return false;
	}

	//------------
	// Steps through the the SymbolTableRecords
	for (int nIndex = 0; nIndex < nLength; nIndex++) {
		AcDbSymbolTableRecord* pSymbolTableRecord = NULL;
		if (Acad::eOk != acdbOpenObject(pSymbolTableRecord, idaAll[nIndex], AcDb::kForWrite)) {
			CLogger::Print(_T("Warn: Fail to get SymbolTableRecord! > Ignore"));
			continue;
		}

		// Get SymbolTableRecord's name (just for informing)
		const ACHAR* szRecordName;
		if (Acad::eOk != pSymbolTableRecord->getName(szRecordName)) {
			CLogger::Print(_T("Warn: Fail to get the Record's name! > Ignore"));
			continue;
		}

		// Erase the SymbolTableRecord.
		if (Acad::eOk != pSymbolTableRecord->erase(Adesk::kTrue)) {
			CLogger::Print(_T("Warn: Fail to erase SymbolTableRecord named '%s'! > Ignore"), szRecordName);
			pSymbolTableRecord->close();
			continue;
		}

		CLogger::Print(_T("Inform: SymbolTableRecord '%s' has been erased."), szRecordName);
		pSymbolTableRecord->close();
	}

	//------------
	// If the kind of SymbolTable is AcDbLayerTable, restore all its records states.
	if (AcRx::kEqual == pTableClass->comparedTo(AcDbLayerTable::desc())) {
		stateLayers.remember();
	}

	CLogger::Print(_T("*Exit: purgeSymbolTable()"));
	return true;
}


int getAllSymbolRecordsIds(AcRxClass* pTableClass, AcDbObjectIdArray & idaAll)
{
	CLogger::Print(_T("*Call: getAllSymbolRecordsIds()"));
	Acad::ErrorStatus es;
	idaAll.setLogicalLength(0);

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbSymbolTable* pSymbolTable = NULL;

	if (AcRx::kEqual == pTableClass->comparedTo(AcDbBlockTable::desc())) {
		CLogger::Print(_T("> This is BlockTable!"));
		es = pDb->getBlockTable(pSymbolTable, AcDb::kForRead);
	}
	else if (AcRx::kEqual == pTableClass->comparedTo(AcDbLayerTable::desc())) {
		CLogger::Print(_T("> This is LayerTable!"));
		es = pDb->getLayerTable(pSymbolTable, AcDb::kForRead);
	}
	else if (AcRx::kEqual == pTableClass->comparedTo(AcDbLinetypeTable::desc())) {
		CLogger::Print(_T("> This is LinetypeTable!"));
		es = pDb->getLinetypeTable(pSymbolTable, AcDb::kForRead);
	}
	else if (AcRx::kEqual == pTableClass->comparedTo(AcDbTextStyleTable::desc())) {
		CLogger::Print(_T("> This is TextStyleTable!"));
		es = pDb->getTextStyleTable(pSymbolTable, AcDb::kForRead);
	}
	else {
		CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() - This kind of SymbolTable is not supported!"));
		return -1;
	}

	if (Acad::eOk != es) {
		CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() -  Fail to get SymbolTable!"));
		return -1;
	}
	
	//------------
	// Get the SymbolTable's iterator.
	AcDbSymbolTableIterator* pSymbolTableIter = NULL;
	es = pSymbolTable->newIterator(pSymbolTableIter);
	pSymbolTable->close();
	if (Acad::eOk != es) {
		CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() - Fail to get the SymbolTable's iterator!"));
		return -1;
	}

	//------------
	// Steps through the SymbolTable's records. 
	// Then get the SymbolTableRecord's ObjectID.
	for (; !pSymbolTableIter->done(); pSymbolTableIter->step()) {
		AcDbObjectId idObj = AcDbObjectId::kNull;
		if (Acad::eOk == pSymbolTableIter->getRecordId(idObj))
			idaAll.append(idObj);
	}

	delete pSymbolTableIter;
	CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() - Count: %02d"), idaAll.length());
	return idaAll.length();
}

#endif
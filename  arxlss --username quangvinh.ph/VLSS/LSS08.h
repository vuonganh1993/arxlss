#ifndef __LSS08_H
#define __LSS08_H
#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"
#include "LayerState.h"

int getAllSymbolRecordsIds(AcRxClass* pTableClass, AcDbObjectIdArray & idaAll);
bool purgeSymbolTable(AcRxClass* pTableClass);

void LSS08()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 08 |--------------"));
	while (purgeSymbolTable(AcDbBlockTable::desc()));
	purgeSymbolTable(AcDbLayerTable::desc());
	purgeSymbolTable(AcDbLinetypeTable::desc());
	purgeSymbolTable(AcDbTextStyleTable::desc());
}


bool purgeSymbolTable(AcRxClass* pTableClass)
{
	CLogger::Print(_T("*Call: purgeSymbolTable()"));

	Acad::ErrorStatus es;
	AcDbObjectIdArray idaAll;
	getAllSymbolRecordsIds(pTableClass, idaAll);

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	es = pDb->purge(idaAll);

	if (Acad::eOk != es) {
		CLogger::Print(_T("*Exit: purgeSymbolTable() : Khong lay duoc danh sach cac SymbolRecord de purge!"));
		return false;
	}

	CAllLayerState stateLayers;
	if (AcRx::kEqual == pTableClass->comparedTo(AcDbLayerTable::desc())) {
		CLogger::Print(_T("Inform: Unlock tat ca cac layer records!"));
		stateLayers.memorize(pDb);
		stateLayers.unlock();
	}

	int nLength = idaAll.length();
	if (nLength <= 0) {
		CLogger::Print(_T("*Exit: purgeSymbolTable() - Khong tim thay record nao de xoa!"));
		return false;
	}

	CLogger::Print(_T("Inform: Duyet qua het tat ca cac SymbolRecords : %d"), nLength);
	for (int nIndex = 0; nIndex < nLength; nIndex++) {
		AcDbSymbolTableRecord* pSymbolTableRecord = NULL;
		if (Acad::eOk != acdbOpenObject(pSymbolTableRecord, idaAll[nIndex], AcDb::kForWrite)) {
			CLogger::Print(_T("Warn: Khong the open duoc SymbolTableRecord! > Ignore"));
			continue;
		}

		const ACHAR*  szRecordName;
		if (Acad::eOk != pSymbolTableRecord->getName(szRecordName)) {
			CLogger::Print(_T("Warn: Khong the lay duoc ten cua Record! > Ignore"));
			continue;
		}

		if (Acad::eOk != pSymbolTableRecord->erase(Adesk::kTrue)) {
			CLogger::Print(_T("Warn: Khong the xoa duoc SymbolTableRecord ten la: %s! > Ignore"), szRecordName);
			pSymbolTableRecord->close();
			continue;
		}

		CLogger::Print(_T("Inform: Da xoa SymbolTableRecord ten la: %s"), szRecordName);
		pSymbolTableRecord->close();
	}

	if (AcRx::kEqual == pTableClass->comparedTo(AcDbLayerTable::desc())) {
		CLogger::Print(_T("Inform: Phuc hoi trang thai cua cac layer records!"));
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
		CLogger::Print(_T("> Day la BlockTable!"));
		es = pDb->getBlockTable(pSymbolTable, AcDb::kForRead);
	}
	else if (AcRx::kEqual == pTableClass->comparedTo(AcDbLayerTable::desc())) {
		CLogger::Print(_T("> Day la LayerTable!"));
		es = pDb->getLayerTable(pSymbolTable, AcDb::kForRead);
	}
	else if (AcRx::kEqual == pTableClass->comparedTo(AcDbLinetypeTable::desc())) {
		CLogger::Print(_T("> Day la LinetypeTable!"));
		es = pDb->getLinetypeTable(pSymbolTable, AcDb::kForRead);
	}
	else if (AcRx::kEqual == pTableClass->comparedTo(AcDbTextStyleTable::desc())) {
		CLogger::Print(_T("> Day la TextStyleTable!"));
		es = pDb->getTextStyleTable(pSymbolTable, AcDb::kForRead);
	}
	else {
		CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() - Kieu Table nay chua duoc ho tro!"));
		return -1;
	}

	if (Acad::eOk != es) {
		CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() -  Khong the lay duoc SymbolTable!"));
		return -1;
	}
	
	CLogger::Print(_T("Inform: Lay Iterator cua SymbolTable!"));
	AcDbSymbolTableIterator* pSymbolTableIter = NULL;
	es = pSymbolTable->newIterator(pSymbolTableIter);
	pSymbolTable->close();
	if (Acad::eOk != es) {
		CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() - khong the lay duoc iterator cua SymbolTable!"));
		return -1;
	}

	CLogger::Print(_T("Inform: Duyet qua tat ca cac SymbolTableRecords!"));
	for (; !pSymbolTableIter->done(); pSymbolTableIter->step()) {
		AcDbObjectId idObj = AcDbObjectId::kNull;
		if (Acad::eOk == pSymbolTableIter->getRecordId(idObj))
			idaAll.append(idObj);
	}

	delete pSymbolTableIter;
	CLogger::Print(_T("*Exit: getAllSymbolRecordsIds() - Tim duoc: %02d"), idaAll.length());
	return idaAll.length();
}

#endif
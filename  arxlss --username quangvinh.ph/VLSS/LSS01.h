/********************************************************************
	File Name:		LSS01.h
	Author:			Pham Quang Vinh
	Purpose:		Create DWG file and read DWG file.
						Practice Errors Handling.
*********************************************************************/

#ifndef LSS01_H
#define LSS01_H

#include "StdAfx.h"
#include "Resource.h"
#include "Logger.h"
#include "Funcs.h"

bool createDwg()
{
	CLogger::Print(_T("*Call: createDwg()"));
	Acad::ErrorStatus es, esTmp;

	//------------
	// Get the BlockTable from a new database object
	AcDbDatabase *pDb = new AcDbDatabase();
	AcDbBlockTable *pBlockTable = NULL;
	if (Acad::eOk != (es = pDb->getSymbolTable(pBlockTable, AcDb::kForRead))) {
		delete pDb;
		CLogger::Print(_T("*Exit: createDwg() - Fail to get the BlockTable. Error: %s"), acadErrorStatusText(es));
		return false;
	}
	
	//------------
	// Get the Model Space
	AcDbBlockTableRecord *pBlockTableRecord = NULL;
	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	if (Acad::eOk != (esTmp = pBlockTable->close()))
		CLogger::Print(_T("Warn: Fail to close the BlockTable object! Error: %s"), acadErrorStatusText(esTmp));
	if (Acad::eOk != es) {
		delete pDb;
		CLogger::Print(_T("*Exit: createDwg() - Fail to get the Model Space from BlockTable! Error: "), acadErrorStatusText(es));
		return false;
	}

	//------------
	// Create new entities then append it into the Model Space
	AcDbCircle	*pCir1 = new AcDbCircle(AcGePoint3d(1,1,1), AcGeVector3d(0,0,1), 1.0);
	es = pBlockTableRecord->appendAcDbEntity(pCir1);
	if (Acad::eOk != es) {
		delete pCir1;
		if (Acad::eOk != (esTmp = pBlockTableRecord->close()))
			CLogger::Print(_T("Warn: Fail to close ModelSpace! Error: %s"), acadErrorStatusText(esTmp));
		delete pDb;
		return false;
	}
	if (Acad::eOk != (esTmp = pCir1->close()))
		CLogger::Print(_T("Warn: Fail to close circle1 object! Error: %s"), acadErrorStatusText(esTmp));

	AcDbCircle *pCir2 = new AcDbCircle(AcGePoint3d(4,4,4), AcGeVector3d(0,0,1), 2.0);
	es = pBlockTableRecord->appendAcDbEntity(pCir2);
	if (Acad::eOk != es) {
		delete pCir2;
		if (Acad::eOk != (esTmp = pBlockTableRecord->close()))
			CLogger::Print(_T("Warn: Fail to close ModelSpace! Error: %s"), acadErrorStatusText(esTmp));

		delete pDb;
		return false;
	}

	if (Acad::eOk != (esTmp = pCir2->close()))
		CLogger::Print(_T("Warn: Fail to close circle2 object! Error: %s"), acadErrorStatusText(esTmp));

	if (Acad::eOk != (esTmp = pBlockTableRecord->close()))
		CLogger::Print(_T("Warn: Fail to close ModelSpace! Error: %s"), acadErrorStatusText(esTmp));

	//------------
	// Save database to file.
	es = pDb->saveAs(_T("c:\\test1.dwg"));
	if (Acad::eOk != es) {
		delete pDb;
		CLogger::Print(_T("*Exit: createDwg() - Cannot save Database object!"));
		return false;
	}

	delete pDb;
	CLogger::Print(_T("*Exit: createDwg()"));
	return true;
}

bool readDwg()
{
	CLogger::Print(_T("*Call: readDwg()"));
	Acad::ErrorStatus es, esTmp;

	//------------
	// Create new database object then read the DWG file into itself.
	AcDbDatabase *pDb = new AcDbDatabase(Adesk::kFalse);
	if(Acad::eOk != (es = pDb->readDwgFile(_T("c:\\test1.dwg")))) {
		delete pDb;
		CLogger::Print(_T("*Exit: readDwg() - Fail to read DWG file into database object. Error: %d"), acadErrorStatusText(es));
		return false;
	}

	//------------
	// Get the Block Table
	AcDbBlockTable *pBlkTbl = NULL;
	if (Acad::eOk != (es = pDb->getSymbolTable(pBlkTbl, AcDb::kForRead))) {
		delete pDb;
		CLogger::Print(_T("*Exit: readDwg() - Fail to get Block Table record! Error: %d"), acadErrorStatusText(es));
		return false;
	}

	//------------
	// Get the Model Space
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	es = pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	if (Acad::eOk != (esTmp = pBlkTbl->close()))
		CLogger::Print(_T("Warn: Fail to close the BlockTable object. Error: %d"), acadErrorStatusText(esTmp));
	if (Acad::eOk != es) {
		delete pDb;
		CLogger::Print(_T("*Exit: readDwg() - Fail to get the Model Space from BlockTable object. Error: %d"), acadErrorStatusText(es));
		return false;
	}

	//------------
	// Get the iterator of Model Space
	AcDbBlockTableRecordIterator *pBlkTblRcdItr = NULL;
	es = pBlkTblRcd->newIterator(pBlkTblRcdItr);
	if (Acad::eOk != (esTmp = pBlkTblRcd->close()))
		CLogger::Print(_T("Warn: Fail to close the Model Space. Error: %d"), acadErrorStatusText(esTmp));
	if (Acad::eOk != es) {
		delete pDb;
		CLogger::Print(_T("*Exit: readDwg() - Fail to get the Model Space Iterator! Error: %d"), acadErrorStatusText(es));
		return false;
	}

	//------------
	// Browse all entities within Model Space.
	AcDbEntity *pEnt = NULL;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if (Acad::eOk != (es = pBlkTblRcdItr->getEntity(pEnt, AcDb::kForRead))) {
			CLogger::Print(_T("Warn: Cannot get the entity object from the iterator! Error: %d > Ignore"), acadErrorStatusText(es));
			continue;
		}

		acutPrintf(ACRX_T("classname: %s\n"), pEnt->isA()->name());
		CLogger::Print(L"classname: %s\n", pEnt->isA()->name());

		if (Acad::eOk != (esTmp = pEnt->close()))
			CLogger::Print(_T("Warn: Fail to close the entity. Error: %d"), acadErrorStatusText(esTmp));
	}

	if (Acad::eOk != (esTmp = pBlkTblRcd->close()))
		CLogger::Print(_T("Warn: Fail to close the block table record object. Error: %d"), acadErrorStatusText(esTmp));

	delete pBlkTblRcdItr;
	delete pDb;
	CLogger::Print(_T("*Exit: readDwg()"));

	return true;
}


void LSS01(void)
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 01 |--------------"));

	if (createDwg())
		readDwg();
}

#endif
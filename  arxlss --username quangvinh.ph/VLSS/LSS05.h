/********************************************************************
	File Name:		LSS05.h
	Author:			Pham Quang Vinh
	
	Purpose:		Process a DWG file and don't take care of the way file is opened.
*********************************************************************/

#ifndef LSS05_H_
#define LSS05_H_

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

#include "DENKICMG.h"	// CMG::IsCacheRunning()
#include "DENKIPJT.h"	// DenkiDwgProject class, DenkiOpenProjectAcDbDatabase()
#include "DENKIPRJ.h"	// DenkiIsOpenProject(), DenkiSaveProjectDwgFile()
#include "axlock.h"			// AcAxDocLock class
#include "LayerState.h"

bool DoJob(LPCASTR pszFileName);

// List all managed documents.
void LSS05()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 05 |--------------"));

	/*
	AcApDocumentIterator* pDocIter = acDocManager->newAcApDocumentIterator();
	if (!pDocIter) return;

	for (; !pDocIter->done(); pDocIter->step()) {
		LPCASTR pszFileName = pDocIter->document()->fileName();
		acutPrintf(ACRX_T("\n%s"), pszFileName);
	}

	delete pDocIter;
	*/

	LPCTSTR pszDWGFile = _T("C:\\Documents and Settings\\All Users\\Documents\\Alfatech\\ACAD-DENKI\\Documents\\Drawings\\sample\\C1.dwg");
	DoJob(pszDWGFile);
}

/****************************
 *	Process a DWG file and don't take care of the way file is opened.
 */
bool DoJob(LPCASTR pszFileName)
{
	CLogger::Print(_T("*Call: DoJob()"));

	//------------
	// Get current working database, just to store.
	// We are going to restore it before exit the function.
	AcDbDatabase* pdbWorking = acdbHostApplicationServices()->workingDatabase();
	
	Acad::ErrorStatus esResult;
	AcApDocument* pDoc = NULL;
	AcDbDatabase* pDb = NULL;
	bool bNeedFreeDwg = false;
	bool bNeedDelDwg = false;

	//------------
	// Look up the document from managed documents.
	pDoc = findDocument(pszFileName);
	if (pDoc) {
		CLogger::Print(_T("Inform: have found out the required database from managed documents!"));
		pDb = pDoc->database();
	}

	if (!pDb) {
		//------------
		// Look up from opening DENKI project
		if (DenkiIsOpenProject()) {
			// Cache is not running.
			if (!DCMG::IsCacheRunning()) {
				pDb = DenkiGetProjectAcDbDatabase(pszFileName);
			}

			// Cache is running now! Browse all files of project for searching!
			if (!pDb && DCMG::IsCacheRunning()) {
				// Search file name from project.
				DenkiDwgProject* pProj = DenkiDwgProject::getCurrent();
				DenkiDwgProjectFile &fileDwg = pProj->findDwg(pszFileName);

				// Get database as the searching result, if fail to do it, try to open file from project.
				if (!fileDwg.getDatabase(pDb)) {
					pDb = DenkiOpenProjectAcDbDatabase(pszFileName);
					if (pDb) {
						CLogger::Print(_T("Inform: file is opened into DENKI project! Via this way we have to Free the obtained database."));
						bNeedFreeDwg = true;
					} 
				}

				if (pDb) {
					CLogger::Print(_T("Inform: File has been opened from DENKI project."));
				}
			}
		}

		//------------
		// Open DWG file into empty database manually.
		if (!pDb) {
			pDb = new AcDbDatabase(false, true);
			esResult = pDb->readDwgFile(pszFileName, _SH_DENYNO); // Permit read-write accessing by other session.
			if (Acad::eOk == esResult) {
				CLogger::Print(_T("Inform: DWG file has been read into an empty database. Need to free the obtained database."));
				bNeedDelDwg = true;
			} else {
				CLogger::Print(_T("Warn: Failse to read DWG file into an empty database."));
				delete pDb;
				pDb = NULL;
			}
		}

		if (!pDb) {
			CLogger::Print(_T("*Exit: DoJob() - cannot open DWG file: %s"), pszFileName);
			acutPrintf(ACRX_T("Cannot open file <%s>"), pszFileName);
			acdbHostApplicationServices()->setWorkingDatabase(pdbWorking);
			return false;
		}
	}

	//------------
	// PREPARE FOR MODIFYING DWG FILE

	// Lock the document.
	AcAxDocLock* pLock = NULL;
	if (pDoc)
		pLock = new AcAxDocLock(pDb, AcAxDocLock::kCurDocSwitch);
	else
		pDb->setRetainOriginalThumbnailBitmap(true);

	// Unlock all layer records (if required).
	CAllLayerState state;
	state.memorize(pDb);
	state.unlock();

	// <Executes modifying DWG file.>
	DWORD dwResult = 0; // MainJob(pDb);

	//------------
	// SAVE FILE AND RELEASE ALL COMPONENTS

	// Restore all layer record's state.
	state.remember();

	// Save to file.
	if (!pDoc) {
		bool bSave = false;
		if (bNeedDelDwg) {
			// DWG file has been opened into an empty database. Save file!
			esResult = pDb->saveAs(pszFileName);
			if (Acad::eOk == esResult)
				bSave = true;
		} else {
			// DWG file has been opened or got from an DENKI project. Save file!
			bSave = DenkiSaveProjectDwgFile(pDb) ? true : false;
		}

		ACHAR szPage[_MAX_FNAME] = ACRX_T("\0");
		_tsplitpath(pszFileName, NULL, NULL, szPage, NULL);
		if (!bSave) {
			acutPrintf(ACRX_T("\nFail to save file '%s'"), szPage);
			CLogger::Print(_T("Warn: Fail to save file!"));
		} else {
			acutPrintf(ACRX_T("\n[%s] save ok."), szPage);
			CLogger::Print(_T("Inform: DWG file has been saved!"));
		}
	}

	// Unlock database.
	if (pLock) {
		delete pLock;
	}

	// Free memory.
	if (bNeedDelDwg) {
		delete pDb;
	}
	if (bNeedFreeDwg) {
		DenkiCloseProjectAcDbDatabase(pDb);
	}

	// Restore the previous working database!
	acdbHostApplicationServices()->setWorkingDatabase(pdbWorking);
	CLogger::Print(_T("*Exit: DoJob()"));
	return true;
}
#endif
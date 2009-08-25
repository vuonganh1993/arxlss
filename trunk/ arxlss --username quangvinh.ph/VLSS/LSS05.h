#ifndef __LSS05_H
#define __LSS05_H

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

#include "DENKIPJM.h"
#include "DENKISYT.h"
#include "DENKICMG.h"
#include "DENKIPJT.h"
#include "DENKIPRJ.h"
#include "DENKIZWT.h"
#include "axlock.h"
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

// Process a DWG file and don't take care of the way that file is opened.
bool DoJob(LPCASTR pszFileName)
{
	CLogger::Print(_T("*Call: DoJob()"));
	AcDbDatabase* pdbWorking = acdbHostApplicationServices()->workingDatabase();
	
	Acad::ErrorStatus esResult;
	AcApDocument* pDoc = NULL;
	AcDbDatabase* pDb = NULL;
	bool bNeedFreeDwg = false;
	bool bNeedDelDwg = false;

	CLogger::Print(_T("Inform: Look up the document from managed documents."));
	pDoc = findDocument(pszFileName);

	if (pDoc) {
		CLogger::Print(_T("Inform: have found out the required database from managed document!"));
		pDb = pDoc->database();
	} else {
		CLogger::Print(_T("Warn: have not found out the required database from managed documents!"));
	}

	if (!pDb) {
		if (DenkiIsOpenProject()) {
			CLogger::Print(_T("Inform: Look up from opening DENKI project!"));

			if (!DCMG::IsCacheRunning()) {
				CLogger::Print(_T("Inform: Cache is not running."));
				pDb = DenkiGetProjectAcDbDatabase(pszFileName);
			}

			if (!pDb && DCMG::IsCacheRunning()) {

				CLogger::Print(_T("Inform: Cache is running now! Browse all files of project for seaching!"));
				DenkiDwgProject* pProj = DenkiDwgProject::getCurrent();
				DenkiDwgProjectFile &fileDwg = pProj->findDwg(pszFileName);

				if (!fileDwg.getDatabase(pDb)) {
					CLogger::Print(_T("Warn: Fail to get database object from project!"));
					CLogger::Print(_T("Inform: try to open DWG file into project!"));
					pDb = DenkiOpenProjectAcDbDatabase(pszFileName);

					if (pDb) {
						CLogger::Print(_T("Inform: file is opened into DENKI project! Via this way we have to Free the obtained database."));
						bNeedFreeDwg = true;
					} else {
						CLogger::Print(_T("Warn: Fail to open file into project!"));
					}
				} else {
					CLogger::Print(_T("Inform: File has been got from Denki Project!"));
				}
			}
		}

		if (!pDb) {
			CLogger::Print(_T("Inform: Open DWG file into empty database."));
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

	CLogger::Print(_T("Inform: Lock document for modifying!"));
	AcAxDocLock* pLock = NULL;
	if (pDoc)
		pLock = new AcAxDocLock(pDb, AcAxDocLock::kCurDocSwitch);
	else
		pDb->setRetainOriginalThumbnailBitmap(true);

	CLogger::Print(_T("Inform: Unlock all layer records!"));
	CAllLayerState state;
	state.memorize(pDb);
	state.unlock();

	CLogger::Print(_T(".......... Execute modifying DWG file!"));
	DWORD dwResult = 0; // MainJob(pDb);

	CLogger::Print(_T("Inform: restore all layer records."));
	state.remember();

	if (!pDoc) {
		CLogger::Print(_T("Inform: Save file....."));
		bool bSave = false;

		if (bNeedDelDwg) {
			CLogger::Print(_T("Inform: DWG file has been opened into an empty database. Save file!"));
			esResult = pDb->saveAs(pszFileName);
			if (Acad::eOk == esResult)
				bSave = true;
		} else {
			CLogger::Print(_T("Inform: DWG file has been opened or got from an DENKI project. Save file!"));
			bSave = DenkiSaveProjectDwgFile(pDb) ? true : false;
		}

		ACHAR szPage[256] = ACRX_T("\0");
		_tsplitpath(pszFileName, NULL, NULL, szPage, NULL);
		if (!bSave) {
			acutPrintf(ACRX_T("\n[%s] save error"), szPage);
			CLogger::Print(_T("Warn: Fail to save file!"));
		} else {
			acutPrintf(ACRX_T("\n[%s] save ok."), szPage);
			CLogger::Print(_T("Inform: DWG file has been saved!"));
		}
	}

	if (pLock) {
		CLogger::Print(_T("Inform: Unlock database."));
		delete pLock;
	}

	if (bNeedDelDwg) {
		CLogger::Print(_T("Inform: DWG file has been opened into an empty database. Delete the database object."));
		delete pDb;
	}

	if (bNeedFreeDwg) {
		CLogger::Print(_T("Inform: DWG file hase been got by an DENKI project! Close the database object."));
		DenkiCloseProjectAcDbDatabase(pDb);
	}

	// restore the previous working database!
	acdbHostApplicationServices()->setWorkingDatabase(pdbWorking);
	CLogger::Print(_T("*Exit: DoJob()"));
	return true;
}
#endif
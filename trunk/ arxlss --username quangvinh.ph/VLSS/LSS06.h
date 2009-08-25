/********************************************************************
	File Name:		LSS06.h
	Author:			Pham Quang Vinh
	Purpose:		Browse all DENKI project's files, then search the file has got BAN_NO
						NAME, INST_NO that match with the required values.
*********************************************************************/

#ifndef LSS06_H
#define LSS06_H

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

#include "DENKIUIM.h"	// DenkiDuplicationCheckLevel class, DenkiGetDuplicationCheckLevel()
#include "DENKISYT.h"	// DenkiSymbolSnapshot class
#include "DENKIPJT.h"	// DenkiDwgProject class, DenkiDwgProjectFile class
#include "DENKIPRJ.h"	// DenkiIsOpenProject()

void LSS06()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 06 |--------------"));

	//------------
	// Get the current DENKI project pointer, and number of files of project.
	if (!DenkiIsOpenProject()) {
		acutPrintf(ACRX_T("Denki project has not been opened yet!"));
		return;
	}

	ACHAR szMsg[256] = ACRX_T("\0");
	DenkiDwgProject* pCurProj = DenkiDwgProject::getCurrent();
	int nFileCount = pCurProj->count();
	if (nFileCount < 1) {
		CLogger::Print(_T("*Exit: There is no file within current project!"));
		return;
	}

	//------------
	// Get the 'Duplication Check Level' option of DENKI project.
	DenkiDuplicationCheckLevel CheckLvl = DenkiGetDuplicationCheckLevel(_T("NAME"));
	bool bChkInstNo = (CheckLvl == dclAsInstNumber) ? true : false;
	bool bChkBanNo = (CheckLvl == dclAsBanNumber) ? true : false;

	if (bChkInstNo) {
		bChkBanNo = true;
	}

	CLogger::Print(_T("> INS_NO: %s - BAN_NO: %s")
		, (bChkInstNo ? _T("Checked") : _T("Unchecked"))
		, (bChkBanNo ? _T("Checked") : _T("Unchecked")));

	//------------
	// Require to input BAN_NO, INST_NO, NAME keywords to search.
	ACHAR szNeedBanNo[256] = ACRX_T("\0");
	ACHAR szNeedInstNo[256] = ACRX_T("\0");
	ACHAR szNeedName[256] = ACRX_T("\0");

	if (bChkBanNo) {
		::LoadString(_hdllInstance, IDS_ASK_BANNO, szMsg, sizeof(szMsg)/sizeof(ACHAR));
		if (RTNORM != acedGetString(1, szMsg, szNeedBanNo)) {
			CLogger::Print(_T("*Exit: Fail to get the BAN_NO keyword."));
			return;
		}
	}

	if (bChkInstNo) {
		::LoadString(_hdllInstance, IDS_ASK_INSTNO, szMsg, sizeof(szMsg)/sizeof(ACHAR));
		if (RTNORM != acedGetString(1, szMsg, szNeedInstNo)) {
			CLogger::Print(_T("*Exit: Fail to get the INST_NO keyword."));
			return;
		}
	}

	::LoadString(_hdllInstance, IDS_ASK_NAME, szMsg, sizeof(szMsg)/sizeof(ACHAR));
	if (RTNORM != acedGetString(1, szMsg, szNeedName)) {
		CLogger::Print(_T("*Exit: Fail to get the NAME keyword."));
		return;
	}

	//------------
	// Steps through DENKI project's files.
	for (int nIndex = 0; nIndex < nFileCount; nIndex++) {
		CLogger::Print(_T("---- Index = %d ----"), nIndex);
		DenkiDwgProjectFile& file = pCurProj->getDwgAt(nIndex);
		if (!file.hasZuwaku()) {
			CLogger::Print(_T("Warn: File has not ZUWAKU. > Ignore"));
			continue;
		}

		// Get file path, and file name
		LPCASTR pszFileName = NULL;
		file.getFilePath(pszFileName);
		CLogger::Print(_T("> %s"), pszFileName);

		// Check whether or not file has got DENKI_ZUWAKU.
		DWORD dwFlags = 0;
		if (!file.getZuwakuFlags(dwFlags)) {
			CLogger::Print(_T("Warn: Fail to get the ZUWAKU flags > Ignore"));
			continue;
		}
		if (!(dwFlags & DENKI_ZUWAKU)) {
			CLogger::Print(_T("Warn: File has not DENKI_ZUWAKU. > Ignore"));
			continue;
		}

		// Get the current working database (Just to store).
		AcDbDatabase* pCurDb = acdbHostApplicationServices()->workingDatabase();

		Acad::ErrorStatus esResult;
		AcApDocument* pDoc = NULL;
		AcDbDatabase* pDb = NULL;
		bool bFromProject = false;
		bool bReadMyseft = false;

		// Look up the required database from managed documents.
		if (pDoc = findDocument(pszFileName)) {
			CLogger::Print(_T("Inform: found out the database from managed documents."));
			pDb = pDoc->database();
		}

		if (!pDb) {
			// Look up the required database from current opening DENKI project!
			if (!DCMG::IsCacheRunning()) {
				CLogger::Print(_T("Inform: cache is not running > try to get database object."));
				if (pDb = DenkiGetProjectAcDbDatabase(pszFileName)) {
					CLogger::Print(_T("Inform: Database is got from DENKI project."));
				}
			}

			if (!pDb && DCMG::IsCacheRunning()) {
				CLogger::Print(_T("Inform: cache is running > try to open DWG file into DENKI project."));
				if (pDb = DenkiOpenProjectAcDbDatabase(pszFileName)) {
					CLogger::Print(_T("Inform: DWG file is opened into DENKI project. (Need to close it)"));
					bFromProject = true;
				}
			}

			// Open DWG file into an empty database object!
			if (!pDb) {
				pDb = new AcDbDatabase(false, true);
				esResult = pDb->readDwgFile(pszFileName, _SH_DENYNO);
				if (Acad::eOk == esResult) {
					CLogger::Print(_T("Inform: DWG file has been opened into an empty database object! (Need to delete it)"));
					bReadMyseft = true;
				} else {
					delete pDb;
					pDb = NULL;
				}
			}

			if (!pDb) {
				CLogger::Print(_T("Warn: Cannot open dwg file name: '%s' > Ignore"), pszFileName);
				acutPrintf(ACRX_T("Cannot open dwg file name: %s"), pszFileName);
				acdbHostApplicationServices()->setWorkingDatabase(pCurDb);
				continue;
			}
		}

		//------------
		// We have got the database pointer now!
		// Get all of its BlockReferences. Then steps through them.
		AcDbObjectIdArray idaAll;
		int nBlkRefCount = getBlockRefAll(pDb, idaAll);

		CLogger::Print(_T("Inform: Browse all of file's block reference! - Number: %d"), nBlkRefCount);
		for (int nIdx = 0; nIdx < nBlkRefCount; nIdx++) {

			// Get the BlockReference's information!
			DenkiSymbolSnapshot dss;
			if (!dss.open(idaAll[nIdx], ACRX_T("NAME, NAME#*, BAN_NO, INST_NO"))) {
				CLogger::Print(_T("Warn: Fail to open DenkSymbolSnapshot object. > Ignore"));
				continue;
			}

			LPASTR pszBanNo = NULL, pszInstNo = NULL, pszName = NULL;

			// Compare BlockReference's information with the required Keywords.
			switch (1)
			{
			case 1:
				if (bChkBanNo) {
					if (!dss.getBanNo(pszBanNo)) {
						CLogger::Print(_T("Warn: %d. Fail to get BAN_NO value!"), nIdx);
						break;
					}
					if (!acutWcMatchEx(pszBanNo, szNeedBanNo, true)) {
						break;
					}
				}

				if (bChkInstNo) {
					if (!dss.getInstNo(pszInstNo)) {
						CLogger::Print(_T("Warn: %d. Fail to get INST_NO value!"), nIdx);
						break;
					}
					if (!acutWcMatchEx(pszInstNo, szNeedInstNo, true)) {
						break;
					}
				}

				if (!dss.getExpandedValue(ACRX_T("NAME"), pszName)) {
					CLogger::Print(_T("Warn: %d. Fail to get NAME value!"), nIdx);
					break;
				}
				if (!acutWcMatchEx(pszName, szNeedName, true)) {
					break;
				}

			default: // Found out the match BlockReference.
				acutPrintf(ACRX_T("Found out %d. NAME = '%s', BAN_NO = '%s', INST_NO = '%s'")
					, nIdx, szNeedName, (bChkBanNo ? szNeedBanNo : L""), (bChkInstNo ? szNeedInstNo : L""));
				CLogger::Print(_T("Inform: Found out %d. NAME = '%s', BAN_NO = '%s', INST_NO = '%s'")
					, nIdx, szNeedName, (bChkBanNo ? szNeedBanNo : L""), (bChkInstNo ? szNeedInstNo : L""));
			}

			// Release BlockReference's information.
			if (pszBanNo)
				dss.freeString(pszBanNo);
			if (pszInstNo)
				dss.freeString(pszInstNo);
			if (pszName)
				dss.freeString(pszName);
			dss.close();
		}

		// Free memory to exit.
		if (bReadMyseft) {
			CLogger::Print(_T("Inform: DWG file has been opened into an empty database object!"));
			delete pDb;
		}
		if (bFromProject) {
			CLogger::Print(_T("Inform: DWG file has been opened into an DENKI project!"));
			DenkiCloseProjectAcDbDatabase(pDb);
		}

		acdbHostApplicationServices()->setWorkingDatabase(pCurDb);
		CLogger::Print(_T("*EXit"));
	}
}

#endif
#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"

#include "Logger.h"

#include "DENKIPJM.h"	// DENKI_ZUWAKU, OTHER_ZUWAKU
#include "DENKIPJT.h"	// DenkiDwgProject class, DenkiDwgProjectFile class
#include "DENKIPRJ.h"	// DenkiIsOpenProject()
#include "DENKIZWT.h"	// DenkiGetDwgBanNumber()

void LSS04()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 04 |--------------"));

	if (!DenkiIsOpenProject()) {
		CLogger::Print(_T("*Exit: Denki Project is not being opened now!"));
		return;
	}

	//------------
	// Get current opening DENKI project object.
	DenkiDwgProject* pDProject = DenkiDwgProject::getCurrent();
	int nCount = pDProject->count();
	if (nCount < 1) {
		CLogger::Print(_T("Inform: no files within project!"));
		return;
	}

	//------------
	// Steps through Project's files.
	for (int idx = 0; idx < nCount; idx++) {
		DenkiDwgProjectFile& fileDwg = pDProject->getDwgAt(idx);
		if (!fileDwg.hasZuwaku()) {
			CLogger::Print(_T("Warn: File has not zuwaku! > Ignore."));
			continue;
		}

		// Get file path.
		LPCTSTR pszFileName = NULL;
		fileDwg.getFilePath(pszFileName);

		acutPrintf(ACRX_T("\n[%02d] : [%s] "), idx, pszFileName);
		CLogger::Print(_T("%02d. %s"), idx, pszFileName);

		// Get ZUWAKU flags (to identify the kind of file)
		DWORD dwFlags = 0;
		if (!fileDwg.getZuwakuFlags(dwFlags)) {
			CLogger::Print(_T("Warn: Fail to get the Zuwaku flags. > Ignore"));
			continue;
		}

		if (dwFlags & DENKI_ZUWAKU) {
			acutPrintf(ACRX_T(" - Type = 0 (DENKI_ZUWAKU)"));
			CLogger::Print(_T("Inform: This a DENKI_ZUWAKU"));
		} else if (dwFlags & OTHER_ZUWAKU) {
			acutPrintf(ACRX_T(" - Type = 1 (OTHER_ZUWAKU)"));
			CLogger::Print(_T("Inform: This OTHER_ZUWAKU"));
		} else {
			CLogger::Print(_T("Warn: Invalid Zuwaku! > Ignore"));
			continue;
		}

		// Inform: get current working database (Just to store).
		// we going to restore this database as soon as step out this code block.
		AcDbDatabase* pdbCurr = acdbHostApplicationServices()->workingDatabase(); 
		{
			// Create new empty AcDbDatabase object. Then read the DWG file into itself.
			AcDbDatabase* pDb = new AcDbDatabase(false, true);
			if (Acad::eOk == pDb->readDwgFile(pszFileName)) {
				ACHAR szBanNo[256] = ACRX_T("\0");

				// Get DWG file's BAN_NO, then print it out.
				if (DenkiGetDwgBanNumber(pDb, szBanNo, ARRAY_SIZE(szBanNo))) {
					acutPrintf(ACRX_T(", BAN_NO=[%s]"), szBanNo);
					CLogger::Print(_T("Inform: BAN_NO = '%s'"), szBanNo);
				} else {
					CLogger::Print(_T("Warn: Fail to get the DWG file's BAN_NO number!"));
				}

			} else {
				CLogger::Print(_T("Warn: Fail to read the DWG file!"));
			}
			delete pDb;
		}
		acdbHostApplicationServices()->setWorkingDatabase(pdbCurr);
	}
}
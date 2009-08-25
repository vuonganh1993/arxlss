#ifndef __LSS06_H
#define __LSS06_H

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

#include "DENKIUIM.h"
#include "DENKISYT.h"
#include "DENKIPJT.h"
#include "DENKIPRJ.h"

void LSS06()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 06 |--------------"));

	CLogger::Print(_T("Inform: Get the Denki current project pointer, and number of files of project."));
	if (!DenkiIsOpenProject()) {
		CLogger::Print(_T("Denki project has not been opened yet!"));
		acutPrintf(ACRX_T("Denki project has not been opened yet!"));
		return;
	}

	ACHAR szMsg[256] = ACRX_T("\0");
	DenkiDwgProject* pCurProj = DenkiDwgProject::getCurrent();
	int nFileCount = pCurProj->count();
	if (nFileCount < 1) {
		CLogger::Print(_T("*Exit: no file within current project!"));
		return;
	}

	CLogger::Print(_T("Inform: Get the 'Duplication Check Level' option."));
	DenkiDuplicationCheckLevel CheckLvl = DenkiGetDuplicationCheckLevel(_T("NAME"));
	bool bChkInstNo = (CheckLvl == dclAsInstNumber) ? true : false;
	bool bChkBanNo = (CheckLvl == dclAsBanNumber) ? true : false;

	if (bChkInstNo) {
		bChkBanNo = true;
	}

	CLogger::Print(_T("> INS_NO: %s - BAN_NO: %s")
		, (bChkInstNo ? _T("Checked") : _T("Unchecked"))
		, (bChkBanNo ? _T("Checked") : _T("Unchecked")));

	
	CLogger::Print(_T("Inform: Require to input keywords to seach"));
	ACHAR szNeedBanNo[256] = ACRX_T("\0");
	ACHAR szNeedInstNo[256] = ACRX_T("\0");
	ACHAR szNeedName[256] = ACRX_T("\0");

	if (bChkBanNo) {
		CLogger::Print(_T("Inform: require the BAN_NO keyword."));
		::LoadString(_hdllInstance, IDS_ASK_BANNO, szMsg, sizeof(szMsg)/sizeof(ACHAR));
		if (RTNORM != acedGetString(1, szMsg, szNeedBanNo)) {
			CLogger::Print(_T("*Exit: Fail to get the BAN_NO keyword."));
			return;
		}
	}

	if (bChkInstNo) {
		CLogger::Print(_T("Inform: require the INST_NO keyword."));
		::LoadString(_hdllInstance, IDS_ASK_INSTNO, szMsg, sizeof(szMsg)/sizeof(ACHAR));
		if (RTNORM != acedGetString(1, szMsg, szNeedInstNo)) {
			CLogger::Print(_T("*Exit: Fail to get the INST_NO keyword."));
			return;
		}
	}

	CLogger::Print(_T("Inform: require the NAME keyword."));
	::LoadString(_hdllInstance, IDS_ASK_NAME, szMsg, sizeof(szMsg)/sizeof(ACHAR));
	if (RTNORM != acedGetString(1, szMsg, szNeedName)) {
		CLogger::Print(_T("*Exit: Fail to get the NAME keyword."));
		return;
	}

	CLogger::Print(_T("Inform: Browse all of project's files to search."));
	for (int nIndex = 0; nIndex < nFileCount; nIndex++) {
		CLogger::Print(_T("---- Index = %d ----"), nIndex);

		DenkiDwgProjectFile& file = pCurProj->getDwgAt(nIndex);
		if (!file.hasZuwaku()) {
			CLogger::Print(_T("Warn: File has not ZUWAKU. > Ignore"));
			continue;
		}

		//CLogger::Print(_T("Inform: get file's path, and file name"));
		LPCASTR pszFileName = NULL;
		file.getFilePath(pszFileName);
		acutPrintf(ACRX_T("\n%02d:[%s]"), nIndex, pszFileName);
		CLogger::Print(_T("> %s"), pszFileName);

		//CLogger::Print(_T("Inform: check whether or not file has got DENKI_ZUWAKU."));
		DWORD dwFlags = 0;
		if (!file.getZuwakuFlags(dwFlags)) {
			CLogger::Print(_T("Warn: Fail to get the ZUWAKU flags > Ignore"));
			continue;
		}
		if (!(dwFlags & DENKI_ZUWAKU)) {
			CLogger::Print(_T("Warn: File has not DENKI_ZUWAKU. > Ignore"));
			continue;
		}

		CLogger::Print(_T("Inform: get the current working database (Just to store)"));
		AcDbDatabase* pCurDb = acdbHostApplicationServices()->workingDatabase();

		Acad::ErrorStatus esResult;
		AcApDocument* pDoc = NULL;
		AcDbDatabase* pDb = NULL;
		bool bFromProject = false;
		bool bReadMyseft = false;

		CLogger::Print(_T("Inform: Look up the required database from managed documents."));
		if (pDoc = findDocument(pszFileName)) {
			CLogger::Print(_T("Inform: found out the database from managed documents."));
			pDb = pDoc->database();
		}

		if (!pDb) {
			CLogger::Print(_T("Inform: Look up the required database from current DENKI project!"));
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

			if (!pDb) {
				CLogger::Print(_T("Inform: open DWG file into an empty database object!"));
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

		
		AcDbObjectIdArray idaAll;
		int nBlkRefCount = getBlockRefAll(pDb, idaAll);
		CLogger::Print(_T("Inform: Browse all of file's block reference! - %d"), nBlkRefCount);
		for (int nIdx = 0; nIdx < nBlkRefCount; nIdx++) {
			DenkiSymbolSnapshot dss;
			if (!dss.open(idaAll[nIdx], ACRX_T("NAME, NAME#*, BAN_NO, INST_NO"))) {
				CLogger::Print(_T("Warn: Fail to open DenkSymbolSnapshot object. > Ignore"));
				continue;
			}

			LPASTR pszBanNo = NULL, pszInstNo = NULL, pszName = NULL;

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

			default:
				acutPrintf(ACRX_T("Found out %d. NAME = '%s', BAN_NO = '%s', INST_NO = '%s'")
					, nIdx, szNeedName, (bChkBanNo ? szNeedBanNo : L""), (bChkInstNo ? szNeedInstNo : L""));
				CLogger::Print(_T("Inform: Found out %d. NAME = '%s', BAN_NO = '%s', INST_NO = '%s'")
					, nIdx, szNeedName, (bChkBanNo ? szNeedBanNo : L""), (bChkInstNo ? szNeedInstNo : L""));
			}

			dss.close();
			if (pszBanNo)
				dss.freeString(pszBanNo);
			if (pszInstNo)
				dss.freeString(pszInstNo);
			if (pszName)
				dss.freeString(pszName);
		}

		CLogger::Print(_T("Inform: Free memory to exit."));
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
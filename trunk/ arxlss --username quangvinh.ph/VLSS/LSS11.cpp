#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"
#include "Logger.h"

#ifdef DENKI_APPLICATION
#include "DENKICMG.h"	// CMG::IsCacheRunning()
#include "DENKIPJT.h"	// DenkiDwgProject class, DenkiOpenProjectAcDbDatabase()
#include "DENKIPRJ.h"	// DenkiIsOpenProject(), DenkiSaveProjectDwgFile()
#include "axlock.h"			// AcAxDocLock class

// http://trac.local/gendoc/wiki/VietLearning200904test1
//
// katayama(2009.05.07)

void LSS11(void)
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 06 |--------------"));
	DoFolder(_T("c:\\sample\\"));
}

// 指定フォルダ(※最後の￥あり)の *.dwg を、すべて処理する
int		DoFolder(LPCTSTR pszPath)
{
	CLogger::Print(L"*Call: DoFolder()");
	int		nModifiedDwg = 0;

	CLogger::Print(L"Inform: Get the list of files contained in the folder!");
	// 指定フォルダに存在する *.dwg を、すべて得る
	CStringArray	csaFiles;
	{
		WIN32_FIND_DATA fd;
		HANDLE	hHandle = NULL;
		CString	csSeek;
		csSeek.Format(_T("%s*.dwg"), pszPath);
		hHandle = ::FindFirstFile(csSeek, &fd);
		while(INVALID_HANDLE_VALUE != hHandle) {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				CString	csFile;
				csFile.Format(_T("%s%s"), pszPath, fd.cFileName);
				csaFiles.Add(csFile);
			}
			if (!::FindNextFile(hHandle, &fd))	break;
		}
		if (hHandle != INVALID_HANDLE_VALUE)
			::FindClose(hHandle);
	}

	if (csaFiles.GetSize() <= 0) {
		CLogger::Print(L"*Exit: DoFolder() - There is not any DWG files");
		return	0;
	}

	//------------
	// Get current working database, just to store.
	// We are going to restore it before exit the function.
	AcDbDatabase* pdbWorking = acdbHostApplicationServices()->workingDatabase();

	// 各図面を処理する
	CLogger::Print(L"Inform: Process all DWG files.");
	for (int nIdx = 0; nIdx < csaFiles.GetSize(); nIdx++) {
		CLogger::Print(L"--------- Index = %d --------", nIdx);

		CString	csDwg = csaFiles[nIdx];
		acutPrintf(_T("\n\n%02d:[%s]"), nIdx, csDwg);
		
		CLogger::Print(L"%s", csDwg);
		Acad::ErrorStatus esResult;
		AcApDocument* pDoc = NULL;
		AcDbDatabase* pDb = NULL;
		bool bNeedFreeDwg = false;
		bool bNeedDelDwg = false;

		//------------
		// Look up the document from managed documents.
		pDoc = findDocument(csDwg);
		if (pDoc) {
			CLogger::Print(_T("Inform: have found out the required database from managed documents!"));
			pDb = pDoc->database();
		}

		//------------
		// Look up the document from DENKI project
		if (!pDb && DenkiIsOpenProject()) {
			if (!DCMG::IsCacheRunning()) {
				pDb = DenkiGetProjectAcDbDatabase(csDwg);
			}

			if (!pDb && DCMG::IsCacheRunning()) {
				// Search file name from DENKI project
				DenkiDwgProject* pPrj = DenkiDwgProject::getCurrent();
				DenkiDwgProjectFile& fileDwg = pPrj->findDwg(csDwg);

				// Get database, if fail to do it, try to open file from project.
				if (!fileDwg.getDatabase(pDb)) {
					pDb = DenkiOpenProjectAcDbDatabase(csDwg);
					if (pDb) {
						CLogger::Print(_T("Inform: file is opened into DENKI project! Via this way we have to Free the obtained database."));
						bNeedFreeDwg = true;
					} 
				}
			}
		}

		//------------
		// Open DWG file manually
		if (!pDb) {
			pDb = new AcDbDatabase(false, true);
			esResult = pDb->readDwgFile(csDwg, _SH_DENYNO); // Permit read-write accessing by other session.
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
			CLogger::Print(_T("Warn: Cannot open DWG file: %s"), csDwg);
			acutPrintf(ACRX_T("\nCannot open file <%s>"), csDwg);
			continue;
		}

		//------------
		// If file has been opened by document, lock the document
		AcAxDocLock* pLock = NULL;
		if (pDoc)
			pLock = new AcAxDocLock(pDb, AcAxDocLock::kCurDocSwitch);
		else
			pDb->setRetainOriginalThumbnailBitmap(true);

		int nRecords = 0;
		if (0 < (nRecords = PurgeJob(pDb))) {
			nModifiedDwg++;
		}

		//------------
		// Save to file
		if (!pDoc && nRecords > 0) {
			bool bSaved = false;
			if (bNeedDelDwg) {
				// DWG file has been opened into an empty database. Save file!
				esResult = pDb->saveAs(csDwg);
				if (Acad::eOk == esResult)
					bSaved = true;
			} else {
				// DWG file has been opened or got from an DENKI project. Save file!
				bSaved = DenkiSaveProjectDwgFile(pDb) ? true : false;
			}

			ACHAR szFileName[_MAX_FNAME] = ACRX_T("\0");
			_tsplitpath(csDwg, NULL, NULL, szFileName, NULL);
			if (!bSaved) {
				acutPrintf(ACRX_T("\nFail to save file '%s'"), szFileName);
				CLogger::Print(_T("Warn: Fail to save file!"));
			} else {
				acutPrintf(ACRX_T("\n[%s] file has been saved."), szFileName);
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
	}

	// Restore the previous working database!
	acdbHostApplicationServices()->setWorkingDatabase(pdbWorking);

	return	nModifiedDwg;
}


// 課題1 の正解例
int PurgeJob(AcDbDatabase *pDb)
{
	Acad::ErrorStatus es;
	int		nTotal = 0;

	for (int nObjType=0; nObjType<3; nObjType++) {
		while(1) {
			int		nDelLot = 0;
			ACHAR	szFmt[256];
			AcDbSymbolTable*	pTbl = NULL;
			switch(nObjType) {
			case 0:
				//	LoadString(_hdllInstance, IDS_MESDELBLKDEF, szFmt, sizeof(szFmt) / sizeof(ACHAR));
				lstrcpy(szFmt, _T("\n利用されていないブロック定義[%s]を削除しました。"));
				es = pDb->getBlockTable(pTbl, AcDb::kForRead);
				break;
			case 1:
				//	LoadString(_hdllInstance, IDS_MESDELLAYER, szFmt, sizeof(szFmt) / sizeof(ACHAR));
				lstrcpy(szFmt, _T("\n利用されていない画層[%s]を削除しました。"));
				es = pDb->getLayerTable(pTbl, AcDb::kForRead);
				break;
			case 2:
				//	LoadString(_hdllInstance, IDS_MESDELTXTSTYLE, szFmt, sizeof(szFmt) / sizeof(ACHAR));
				lstrcpy(szFmt, _T("\n利用されていない文字スタイル[%s]を削除しました。"));
				es = pDb->getTextStyleTable(pTbl, AcDb::kForRead);
				break;
			}
			if (Acad::eOk != es)
				break;

			// purge 対象を列挙する
			AcDbSymbolTableIterator* pIter = NULL;
			es = pTbl->newIterator(pIter);
			pTbl->close();
			if (Acad::eOk != es)
				break;

			AcDbObjectIdArray ida;
			for(; !pIter->done(); pIter->step()) {
				AcDbObjectId id;
				es = pIter->getRecordId(id);
				ida.append(id);
			}
			delete pIter;
			if (ida.length() == 0)
				break;

			// 削除できない要素を除外
			es = pDb->purge(ida);
			if (es != Acad::eOk || ida.length() == 0) 
				break;

			for (int i = 0; i < ida.length(); i++) {
				AcDbSymbolTableRecord*	pRec = NULL;
				es = acdbOpenObject(pRec, ida[i], AcDb::kForWrite);
				if (es != Acad::eOk)
					continue;
				// 名前を得る
				ACHAR	szName[256] = ACRX_T("\0");
				LPCASTR	pszName = NULL;
				if (Acad::eOk == pRec->getName(pszName))
					lstrcpy(szName, pszName);
				if (!lstrlen(szName) && nObjType==2) {
					// 文字スタイルだけは取得できない場合があるので、独自に対処する
					AcDbTextStyleTableRecord*	pStyleRec = AcDbTextStyleTableRecord::cast(pRec);
					if (Acad::eOk == pStyleRec->fileName(pszName) && pszName && lstrlen(pszName))
						lstrcpy(szName, pszName);
					if (Acad::eOk == pStyleRec->bigFontFileName(pszName) && pszName && lstrlen(pszName))
						lstrcpy(szName, pszName);
				}
				if (Acad::eOk == pRec->erase()) {
					acutPrintf(szFmt, szName);
					nDelLot++;
				}
				pRec->close();
			}
			nTotal += nDelLot;

			// ブロック定義はネスティングするので、再度　purge 処理を実行できる場合がある
			if (nDelLot==0 || nObjType!=0)
				break;
		}
	}

	return	nTotal;
}
#endif
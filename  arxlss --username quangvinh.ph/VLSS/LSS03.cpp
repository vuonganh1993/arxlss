#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"

#include "Logger.h"

#include "DENKIPJM.h"
#include "DENKISYT.h"
#include "DENKICMG.h"
#include "DENKIPJT.h"
#include "DENKIPRJ.h"
#include "DENKIZWT.h"

void LSS03()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 03 |--------------"));
	listFilesEx();
}

//------------
// Get project's files names by using mask option.
bool listFiles()
{
	CLogger::Print(_T("*Call: listFiles()"));

	// Check whether or not a DENKI project is opening?
	if (!DenkiIsOpenProject()) {
		CLogger::Print(_T("*Exit: listFiles() - Denki project is not being opened!"));
		return false;
	}

	DenkiDwgProject* pProject = DenkiDwgProject::getCurrent();

	// Create a DenkiGetProjectFileMask object (mask option).
	DenkiGetProjectFileMask mask = (DenkiGetProjectFileMask)(MASK_DENKIZUMEN | MASK_SONOTAZUMEN);
	int nCount = 0;

	// Get project's files into an array.
	const LPCTSTR* pAryDwg = DenkiGetProjectFiles(mask, &nCount);
	if (!pAryDwg) {
		CLogger::Print(_T("*Exit: listFiles() - Fail to get the project's files names!"));
		return false;
	}

	// Steps through the array's items to print out its value.
	for (int nIdx=0; nIdx<nCount; nIdx++) {
		acutPrintf(ACRX_T("\n%02d:%s"), nIdx, pAryDwg[nIdx]);
		CLogger::Print(_T("Inform: %02d : %s"), nIdx, pAryDwg[nIdx]);
	}

	DenkiFreeCharPtrArray(pAryDwg); // Remember to free returned memory after using DenkiGetProjectFiles function
	CLogger::Print(_T("*Exit: listFiles()"));
	return true;
}

bool listFilesEx(void)
{
	CLogger::Print(_T("*Call: listFilesEx()"));

	if (!DenkiIsOpenProject()) {
		CLogger::Print(_T("Denki project is not being opened!"));
		return false;
	}

	// Get the current opening DENKI project, and the number of files.
	DenkiDwgProject* pCurProj = DenkiDwgProject::getCurrent();
	int nCount = pCurProj->count();
	if (nCount < 1) {
		CLogger::Print(_T("*Exit: listFilesEx no file within project!"));
		return false;
	}

	// Steps through project's files then get its name.
	for (int nIdx=0; nIdx<nCount; nIdx++) {
		DenkiDwgProjectFile& file = pCurProj->getDwgAt(nIdx);

		LPCASTR pszFileName = NULL;
		file.getFilePath(pszFileName);
		acutPrintf(_ACRX_T("\n%02d:%s"), nIdx, pszFileName);
		CLogger::Print(_T("Inform: %d. %s"), nIdx, pszFileName);

		if (!file.hasZuwaku()) {
			CLogger::Print(_T("Inform: File has not Zuwaku!"), nIdx);
		}
	}

	return true;
}
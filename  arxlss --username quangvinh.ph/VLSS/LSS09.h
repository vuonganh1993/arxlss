#ifndef __LSS09_H
#define __LSS09_H

#include "StdAfx.h"
#include "Resource.h"
#include "Funcs.h"
#include "Logger.h"

void LSS09()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 09 |--------------"));

	Acad::ErrorStatus es;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbDatabaseSummaryInfo* pSummaryInfo = NULL;
	es = acdbGetSummaryInfo(pDb, pSummaryInfo);
	
	if (Acad::eOk == es) {
		ACHAR* szAuthor = _T("\0");
		ACHAR* szTitle = _T("\0");
		ACHAR* szComments = _T("\0");
		ACHAR* szHyperlinkBase = _T("\0");
		ACHAR* szKeywords = _T("\0");
		ACHAR* szLastSavedBy = _T("\0");
		ACHAR* szRevision = _T("\0");
		ACHAR* szSubject = _T("\0");

		pSummaryInfo->getAuthor(szAuthor);
		pSummaryInfo->getTitle(szTitle);
		pSummaryInfo->getComments(szComments);
		pSummaryInfo->getHyperlinkBase(szHyperlinkBase);
		pSummaryInfo->getKeywords(szKeywords);
		pSummaryInfo->getLastSavedBy(szLastSavedBy);
		pSummaryInfo->getRevisionNumber(szRevision);
		pSummaryInfo->getSubject(szSubject);

		CLogger::Print(_T("Database Summary Information"));
		CLogger::Print(_T("Author: %s"), szAuthor);
		CLogger::Print(_T("Title: %s"), szTitle);
		CLogger::Print(_T("Comments: "), szComments);
		CLogger::Print(_T("Hyperlink Base: %s"), szHyperlinkBase);
		CLogger::Print(_T("Key words: %s"), szKeywords);
		CLogger::Print(_T("Last saved by: %s"), szLastSavedBy);
		CLogger::Print(_T("Revision: %s"), szRevision);
		CLogger::Print(_T("Subject: %s"), szSubject);

		acutDelString(szAuthor);
		acutDelString(szTitle);
		acutDelString(szComments);
		acutDelString(szHyperlinkBase);
		acutDelString(szKeywords);
		acutDelString(szLastSavedBy);
		acutDelString(szRevision);
		acutDelString(szSubject);
	} else {
		CLogger::Print(_T("*Exit: Khong the lay duoc DatabaseSummaryInfomation"));
	}
}

#endif
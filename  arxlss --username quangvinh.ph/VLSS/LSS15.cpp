#include "StdAfx.h"
#include "Resource.h"
#include "Logger.h"
#include "Lessons.h"

void LSS15(void)
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 15 |--------------"));
	readProfile();
}

void readProfile()
{
	TCHAR buff[1000];
	memset(buff, 0, 1000);
	GetPrivateProfileSectionNames(buff, 1000, L"C:\\boot.ini");
	CStringArray staSections;

	for (int nIdx = 0; nIdx < 1000; nIdx++) {
		// Tim diem co gia tri khac 0 va gia tri truoc no bang 0
		if ((nIdx == 0 && buff[nIdx] != 0) || (nIdx > 0 && buff[nIdx] != 0 && buff[nIdx - 1] == 0)) {
				TCHAR* pszTmp = &buff[nIdx];
				staSections.Add(pszTmp);
				CLogger::Print(L"%s", pszTmp);
		}
	}

	delete [] buff;
}

void writePrifile()
{
	
}
#include "StdAfx.h"
#include "Resource.h"
#include "Lessons.h"

#include "Logger.h"

bool printXData()
{
	CLogger::Print(_T("*Call: printxData()"));
	AcDbObject* pObj;
	
	//------------
	// Require to select an entity
	if (!(pObj = selectObject(AcDb::kForRead))) {
		CLogger::Print(_T("*Exit: printxData() - Object have not selected."));
		return false;
	}

	//------------
	// Require to enter xData application name
	ACHAR appname[133];
	if (RTNORM != acedGetString(NULL, ACRX_T("\nEnter the desired Xdata application name: "), appname)) 
	{
		CLogger::Print(_T("*Exit: printxData() - Fail to enter the application name!"));
		return false;
	}

	//------------
	// Read the xData that contained in object.
	// If application name is existing then print out its values.
	struct resbuf* pRb;
	pRb = pObj->xData(appname);
	pObj->close();
	if (pRb) {
		acutPrintf(ACRX_T("Inform: Application name '%s' is existing - The values are: "), appname);
		printList(pRb);
		acutRelRb(pRb); // release xData after using!
	} else {
		acutPrintf(ACRX_T("\n*Exit: printxData() - Application name '%s' is not existing."), appname);
		pObj->close();
		return false;
	}

	pObj->close();
	CLogger::Print(_T("*Exit: printxData()"));
	return true;
}

bool addXData()
{
	Acad::ErrorStatus es, esTmp;
	CLogger::Print(_T("*Call: addXData()"));

	//------------
	// Require to select an entity.
	AcDbObject* pObj = selectObject(AcDb::kForRead);
	if (!pObj) {
		CLogger::Print(_T("*Exit: addXData() - Khong chon duoc object!"));
		return false;
	}

	//------------
	// Require to enter the application name, then require to enter its string value.
	ACHAR appName[133], resString[133];
	appName[0] = resString[0] = L'\0';
	acedGetString(NULL, ACRX_T("\nEnter application name: "), appName);
	acedGetString(NULL, ACRX_T("\nEnter string to be added: "), resString);

	CLogger::Print(_T("Inform: Found out application name '%s'!"), appName);
	struct resbuf * pRb;
	struct resbuf * pTemp;
	pRb = pObj->xData(appName);

	if (pRb) {
		// If the appname is existing! Seek to the tail of resbuf link list.
		CLogger::Print(_T("Inform: Found out application name '%s' - Seek to the end of resbuf link list."), appName);
		for (pTemp = pRb; pTemp->rbnext; pTemp = pTemp->rbnext) {}
	} else {
		// If the appname is not existing! Register a new application name.
		CLogger::Print(_T("Inform: The application name %s is not existing, register this application name!"), appName);
		acdbRegApp(appName);

		// Then create new resbuf to copy the application name into it.
		pRb = acutNewRb(AcDb::kDxfRegAppName);
		pTemp = pRb;
		pTemp->resval.rstring = (ACHAR*) malloc((lstrlen(appName) + 1) * sizeof(ACHAR));
		lstrcpy(pTemp->resval.rstring, appName);
	}

	//------------
	// Create new resbuf at the tail of the link list, then copy the value string into it.
	pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp = pTemp->rbnext;
	pTemp->resval.rstring = (ACHAR*) malloc((lstrlen(resString) + 1) * sizeof(ACHAR));
	lstrcpy(pTemp->resval.rstring, resString);

	CLogger::Print(_T("Inform: Upgrade object opening!"));
	if (Acad::eOk != (es = pObj->upgradeOpen())) {
		CLogger::Print(_T("*Exit: addXData() - Fail to upgrade opening object!"));
		pObj->close();
		acutRelRb(pRb);
		return false;
	}

	//------------
	// Set object's xData.
	pObj->setXData(pRb);
	pObj->close();
	acutRelRb(pRb); // remember to release resbuf after using
	CLogger::Print(_T("*Exit: addXData()!"));
	return true;
}

void LSS02()
{
	CLogger::Print(_T("-------------| START LOGGING LESSONS 02 |--------------"));
	if (addXData()) 
		printXData();
	else
		CLogger::Print(_T("Warn: Fail to call addXData()"));
}
// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("SGN")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CAPPLApp : public AcRxArxApp {

public:
	CAPPLApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
protected:
	static void LoadAppFromFolder(LPTSTR szSearch);
	static void UnloadAppFromFolder(LPTSTR szSearch);
		
public:
	// - SGNAPPL.APL command (do not rename)
	static void SGNAPPLAPL(void)
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFullPath[_MAX_PATH];

		int nLen = GetModuleFileName(_hdllInstance, szFullPath, _MAX_PATH);
		if (nLen <= 0) {
			return;
		}

		szFullPath[nLen] = L'\0';
		_tsplitpath(szFullPath, szDrive, szDir, NULL, NULL);
		
		TCHAR szTargetDir[_MAX_PATH] = L"\0";
		lstrcat(szTargetDir, szDrive);
		lstrcat(szTargetDir, szDir);

		LoadAppFromFolder(szTargetDir);
	}

	// - SGNAPPL.AUL command (do not rename)
	static void SGNAPPLAUL(void)
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];

		TCHAR szFullPath[_MAX_PATH];
		int nLen = GetModuleFileName(_hdllInstance, szFullPath, _MAX_PATH);
		if (nLen <= 0) {
			return;
		}

		szFullPath[nLen] = L'\0';
		_tsplitpath(szFullPath, szDrive, szDir, NULL, NULL);

		TCHAR szTargetDir[_MAX_PATH] = L"\0";
		lstrcat(szTargetDir, szDrive);
		lstrcat(szTargetDir, szDir);

		UnloadAppFromFolder(szTargetDir);
	}
} ;

void CAPPLApp::LoadAppFromFolder(LPTSTR szFolder)
{
	int nLength = lstrlen(szFolder) + 6;
	LPTSTR szSearch = (LPTSTR)::LocalAlloc(LMEM_FIXED, nLength * sizeof(TCHAR));
	lstrcpy(szSearch, szFolder);
	szSearch[lstrlen(szFolder)] = L'\0';

	if (!lstrcat(szSearch, _T("*.arx")))
	{
		::LocalFree(szSearch);
		return;
	}

	CFileFind finder;
	BOOL bWorking = finder.FindFile(szSearch);

	if (!bWorking)
	{
		acutPrintf(ACRX_T("\nCannot find out the folder name: %s"), szFolder);
		::LocalFree(szSearch);
		return;
	}

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		LPCTSTR szFileName = finder.GetFileName();
		if (0 == lstrcmp(szFileName, _T("SGNAPPL.arx")) ||
			finder.IsDirectory())
			continue;

		CString strFilePath = finder.GetFilePath();
		strFilePath.Replace(L'\\', L'/');

		acutPrintf(ACRX_T("\nLoad '%s' - "), strFilePath);
		if (RTNORM == acedArxLoad(strFilePath))
			acutPrintf(ACRX_T("Done!"));
		else
			acutPrintf(ACRX_T("Fail!"));
	}

	::LocalFree(szSearch);
}

void CAPPLApp::UnloadAppFromFolder(LPTSTR szFolder)
{
	int nLength = lstrlen(szFolder) + 6;
	LPTSTR szSearch = (LPTSTR)::LocalAlloc(LMEM_FIXED, nLength * sizeof(TCHAR));
	lstrcpy(szSearch, szFolder);
	szSearch[lstrlen(szFolder)] = L'\0';

	if (!lstrcat(szSearch, _T("*.arx")))
	{
		::LocalFree(szSearch);
		return;
	}

	CFileFind finder;
	BOOL bWorking = finder.FindFile(szSearch);

	if (!bWorking)
	{
		acutPrintf(ACRX_T("\nCannot find out the folder name: %s"), szFolder);
		::LocalFree(szSearch);
		return;
	}

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		LPCTSTR szFileName = finder.GetFileName();
		if (0 == lstrcmp(szFileName, _T("SGNAPPL.arx")) ||
			finder.IsDirectory())
			continue;

		CString strFilePath = finder.GetFilePath();
		strFilePath.Replace(L'\\', L'/');

		acutPrintf(ACRX_T("\nUnload '%s' - "), strFilePath);
		if (RTNORM == acedArxUnload(strFilePath))
			acutPrintf(ACRX_T("Done!"));
		else
			acutPrintf(ACRX_T("Fail!"));
	}

	::LocalFree(szSearch);
}

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CAPPLApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CAPPLApp, SGNAPPL, APL, APL, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAPPLApp, SGNAPPL, AUL, AUL, ACRX_CMD_TRANSPARENT, NULL)


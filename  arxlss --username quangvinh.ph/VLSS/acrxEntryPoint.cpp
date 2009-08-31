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
#include "Lessons.h"
#include "Ex01Dlg.h"
#include "Ex02Dlg.h"
#include "Ex03Dlg.h"
#include "Ex04Dlg.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("SGN")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CVLSSApp : public AcRxArxApp {

public:
	CVLSSApp () : AcRxArxApp () {}

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

	// - SGNVLSS.LSS01 command (do not rename)
	static void SGNVLSSLSS01(void)
	{
		LSS01();
	}

	// - SGNVLSS.LSS02 command (do not rename)
	static void SGNVLSSLSS02(void)
	{
		LSS02();
	}

	// - SGNVLSS.LSS03 command (do not rename)
	static void SGNVLSSLSS03(void)
	{
		#ifdef DENKI_APPLICATION
			LSS03();
		#else
			acutPrintf(ACRX_T("\nDon't support DENKI library!"));
		#endif
	}

	// - SGNVLSS.LSS04 command (do not rename)
	static void SGNVLSSLSS04(void)
	{
		#ifdef DENKI_APPLICATION
			LSS04();
		#else
			acutPrintf(ACRX_T("\nDon't support DENKI library!"));
		#endif
	}

	// - SGNVLSS.LSS05 command (do not rename)
	static void SGNVLSSLSS05(void)
	{
		#ifdef DENKI_APPLICATION
			LSS05();
		#else
			acutPrintf(ACRX_T("\nDon't support DENKI library!"));
		#endif
	}

	// - SGNVLSS.LSS06 command (do not rename)
	static void SGNVLSSLSS06(void)
	{
		#ifdef DENKI_APPLICATION
			LSS06();
		#else
			acutPrintf(ACRX_T("\nDon't support DENKI library!"));
		#endif
	}

	// - SGNVLSS.LSS07 command (do not rename)
	static void SGNVLSSLSS07(void)
	{
		LSS07();
	}

	// - SGNVLSS.LSS08 command (do not rename)
	static void SGNVLSSLSS08(void)
	{
		LSS08();
	}

	// - SGNVLSS.LSS09 command (do not rename)
	static void SGNVLSSLSS09(void)
	{
		LSS09();
	}

	// - SGNVLSS.LSS10 command (do not rename)
	static void SGNVLSSLSS10(void)
	{
		LSS10();
	}

	// - SGNVLSS.LSS11 command (do not rename)
	static void SGNVLSSLSS11(void)
	{
		LSS11();
	}

	// - SGNVLSS.LSS12 command (do not rename)
	static void SGNVLSSLSS12(void)
	{
		#ifdef DENKI_APPLICATION
			LSS12();
		#else
			acutPrintf(ACRX_T("\nDon't support DENKI library!"));
		#endif
	}

	// - SGNVLSS.LSS13 command (do not rename)
	static void SGNVLSSLSS13(void)
	{
		LSS13();
	}

	// ----- SGNVLSS.LSS14 command
	static void SGNVLSSLSS14(void)
	{
		LSS14();
	}

	static void SGNVLSSDLG01(void)
	{
		CEx01Dlg::RunMe();
	}

	// - SGNVLSS.DLG02 command (do not rename)
	static void SGNVLSSDLG02(void)
	{
		CEx02Dlg::RunMe();
	}

	// - SGNVLSS.DLG03 command (do not rename)
	static void SGNVLSSDLG03(void)
	{
		CEx03Dlg::RunMe();
	}

	// - SGNVLSS.DLG04 command (do not rename)
	static void SGNVLSSDLG04(void)
	{
		CEx04Dlg::RunMe();
	}

	// - SGNVLSS.LSS15 command (do not rename)
	static void SGNVLSSLSS15(void)
	{
		// Add your code for command SGNVLSS.LSS15 here
		LSS15();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CVLSSApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS01, LSS01, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS02, LSS02, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS03, LSS03, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS04, LSS04, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS05, LSS05, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS06, LSS06, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS07, LSS07, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS08, LSS08, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS09, LSS09, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS10, LSS10, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS11, LSS11, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS12, LSS12, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS13, LSS13, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS14, LSS14, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, DLG01, DLG01, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, DLG02, DLG02, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, DLG03, DLG03, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, DLG04, DLG04, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CVLSSApp, SGNVLSS, LSS15, LSS15, ACRX_CMD_TRANSPARENT, NULL)

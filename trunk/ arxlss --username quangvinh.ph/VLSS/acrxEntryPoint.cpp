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
#include "Funcs.h"
#include "LSS01.h"
#include "LSS02.h"
#include "LSS03.h"
#include "LSS04.h"
#include "LSS05.h"
#include "LSS06.h"
#include "LSS07.h"
#include "LSS08.h"
#include "LSS09.h"
#include "LSS10.h"
#include "LSS11.h"
#include "LSS12.h"
#include "LSS13.h"
#include "LSS14.h"
#include "Ex01Dlg.h"

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
		LSS03();
	}

	// - SGNVLSS.LSS04 command (do not rename)
	static void SGNVLSSLSS04(void)
	{
		LSS04();
	}

	// - SGNVLSS.LSS05 command (do not rename)
	static void SGNVLSSLSS05(void)
	{
		LSS05();
	}

	// - SGNVLSS.LSS06 command (do not rename)
	static void SGNVLSSLSS06(void)
	{
		LSS06();
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
		LSS12();
	}

	// - SGNVLSS.LSS13 command (do not rename)
	static void SGNVLSSLSS13(void)
	{
		LSS13();
	}

	// ----- SGNVLSS.LSS15 command
	static void SGNVLSSLSS14(void)
	{
		LSS14();
	}

	static void SGNVLSSDLG01(void)
	{
		CEx01Dlg::RunMe();
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

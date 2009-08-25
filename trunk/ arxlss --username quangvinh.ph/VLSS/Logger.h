/********************************************************************
	File Name:	Logger.h
	Author:		Pham Quang Vinh
	Purpose:	This file declares the CLogger class that will enable we to log our application into
					an special file.
*********************************************************************/
#pragma once

class CLogger
{
private:
	static bool m_bFileExist;
	static TCHAR m_szLogFile[2048];
	static bool initFileName();

public:
	CLogger(void);
	virtual ~CLogger(void);

	//------------
	// Print the format string into the log file.
	static void Print(LPCTSTR pszFormat, ...);
	static bool m_bEnableLogging;	
};

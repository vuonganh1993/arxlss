/********************************************************************
	File Name:		Logger.cpp
	Author:			Pham Quang Vinh
	
	Purpose:		This file implements the CLogger class.
*********************************************************************/

#include "StdAfx.h"
#include "Resource.h"
#include "Logger.h"

bool CLogger::m_bEnableLogging = APPLICATION_ENABLE_LOGGING;
bool CLogger::m_bFileExist = FALSE;
TCHAR CLogger::m_szLogFile[2048] = APPLICATION_LOG_FILE;

CLogger::CLogger(void)
{
}

CLogger::~CLogger(void)
{
}

bool CLogger::initFileName()
{
	if (m_bFileExist) {
		return true;
	}

	int nLen = GetModuleFileName(_hdllInstance, m_szLogFile, 2048);
	if (nLen <= 1) {
		lstrcpy(m_szLogFile, APPLICATION_LOG_FILE);
		nLen = lstrlen(m_szLogFile);
		if (nLen <= 1) {
			m_bFileExist = m_bEnableLogging = false;
			return false;
		}
	}

	m_szLogFile[nLen] = L'\0';
	lstrcat(m_szLogFile, L".log");

	CFile fileLog;
	bool bIsNewFile = false;
	if (!fileLog.Open(m_szLogFile, CFile::modeRead)) {
		if (!fileLog.Open(m_szLogFile, CFile::modeCreate)) {
			m_bFileExist = m_bEnableLogging = false;
			return false;
		} else {
			bIsNewFile = true;
		}
	}

	fileLog.Close();

	if (bIsNewFile) {
		if (fileLog.Open(m_szLogFile, CFile::modeWrite | CFile::typeBinary)) {
			BYTE byteHeader[2] = {0xFF, 0xFE};
			fileLog.Write(byteHeader, 2);
			fileLog.Close();
		}
	}
	m_bFileExist = true;
	return true;
}

void CLogger::Print(LPCTSTR pszFormat, ...)
{
	if (!CLogger::m_bEnableLogging)
		return;

	if (!initFileName())
		return;

	TCHAR szLog[256];

	va_list argList;
	va_start(argList, pszFormat);
	_vstprintf(szLog, pszFormat, argList);
	va_end(argList);

	//Get current time
	SYSTEMTIME	time;
	::GetLocalTime(&time);
	TCHAR	szLine[256];

	_stprintf(szLine, _T("%02d:%02d:%02d:%03d: \t%s\n"), 
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
		szLog);

	CFile logFile;
	if (logFile.Open(m_szLogFile, CFile::modeReadWrite | CFile::typeBinary)) {
		logFile.SeekToEnd();
		logFile.Write(szLine, lstrlen(szLine)*sizeof(TCHAR));
		logFile.Close();
	}
}
// Ex04Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Resource.h"
#include "Logger.h"
#include "Ex04Dlg.h"


// CEx04Dlg ダイアログ

IMPLEMENT_DYNAMIC(CEx04Dlg, CAcUiDialog)

CEx04Dlg::CEx04Dlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CEx04Dlg::IDD, pParent)
	, m_strCopy(_T(""))
	, m_strPaste(_T(""))
{

}

CEx04Dlg::~CEx04Dlg()
{
}

void CEx04Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITCOPY, m_strCopy);
	DDX_Text(pDX, IDC_EDITPASTE, m_strPaste);
}


BEGIN_MESSAGE_MAP(CEx04Dlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BTNCOPY, &CEx04Dlg::on_btnCopy_clicked)
	ON_BN_CLICKED(IDC_BTNPASTE, &CEx04Dlg::on_btnPaste_clicked)
END_MESSAGE_MAP()


// CEx04Dlg メッセージ ハンドラ

void CEx04Dlg::on_btnCopy_clicked()
{
	UpdateData();
	if (OpenClipboard()) {
		HGLOBAL clipBuff;	
		TCHAR* buff;
		EmptyClipboard();

		clipBuff = GlobalAlloc(GMEM_DDESHARE, m_strCopy.GetLength() + 1);
		buff = (TCHAR*)GlobalLock(clipBuff);
		lstrcpy(buff, LPCTSTR(m_strCopy));

		GlobalUnlock(clipBuff);
		SetClipboardData(CF_TEXT, clipBuff);
		CloseClipboard();
	}
}

void CEx04Dlg::on_btnPaste_clicked()
{
	TCHAR* buffer = NULL;
	CString strClipboard;

	if (OpenClipboard()) {
		HANDLE hData = GetClipboardData(CF_TEXT);
		TCHAR* buffer = (TCHAR*)GlobalLock(hData);
		strClipboard = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
	}

	m_strPaste = strClipboard;
	UpdateData(FALSE);
}

void CEx04Dlg::RunMe()
{
	CLogger::Print(L"--------------------| START LOGGING DIALOG 04 |--------------------");
	CEx04Dlg dlg;
	dlg.DoModal();
}
/********************************************************************
	Created:		26:8:2009   16:22
	File Name:		Ex01Dlg.cpp
	Author:			Pham Quang Vinh
	
	Purpose:		This file implements CEx01Dlg's functions.
*********************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "Ex01Dlg.h"
#include "Logger.h"

IMPLEMENT_DYNAMIC(CEx01Dlg, CAcUiDialog)

CEx01Dlg::CEx01Dlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CEx01Dlg::IDD, pParent)
	, m_strUser(_T(""))
	, m_strPass(_T(""))
{
	CLogger::Print(L"*Construct: class CEx01Dlg");
}

CEx01Dlg::~CEx01Dlg()
{
	CLogger::Print(L"*Destruct: class CEx01Dlg");
}

void CEx01Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
	DDV_MaxChars(pDX, m_strPass, 12);
}


BEGIN_MESSAGE_MAP(CEx01Dlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CEx01Dlg::on_btnOk_clicked)
	ON_BN_CLICKED(IDCANCEL, &CEx01Dlg::on_btnCancel_clicked)
	ON_BN_CLICKED(IDC_BTN_TEST, &CEx01Dlg::on_btnTest_clicked)
END_MESSAGE_MAP()


// CEx01Dlg メッセージ ハンドラ

void CEx01Dlg::on_btnOk_clicked()
{
	CLogger::Print(_T("*Event: on_btnOk_clicked()"));

	UpdateData(TRUE);
	if (m_strUser != L"vinh" || m_strPass != L"cancel") {
		MessageBox(L"Invalid username or password, please try again!"
			, L"Failed!", MB_OK | MB_ICONEXCLAMATION);

		m_strPass = L"";
		UpdateData(FALSE);

		CWnd* pStaticUser = GetDlgItem(IDC_EDIT_USER);
		pStaticUser->SetFocus();

		CLogger::Print(_T("*Exit: on_btnOk_clicked() - Invalid username or password!"));
		return;
	}

	MessageBox(L"Welcome to ObjectARX examples.\nAuthor: Pham Quang Vinh."
		, L"Welcome!", MB_ICONINFORMATION | MB_OK);

	OnOK();
	CLogger::Print(_T("*Exit: on_btnOk_clicked()"));
}

void CEx01Dlg::on_btnCancel_clicked()
{
	CLogger::Print(_T("*Event: on_btnCancel_clicked()"));

	int nResult = MessageBox(L"Do you make sure to exit!"
		, L"Confirm", MB_ICONQUESTION | MB_YESNO);
	if (nResult == IDYES) {
		OnCancel();
		CLogger::Print(_T("*Exit: on_btnCancel_clicked()"));
	} else {
		CLogger::Print(_T("*Exit: on_btnCancel_clicked() - Do not exit!"));
	}
}

void CEx01Dlg::on_btnTest_clicked()
{
	CLogger::Print(L"*Event: on_btnTest_clicked()");
	CWnd* pWnd = GetDlgItem(IDCANCEL);
	pWnd->SetFocus();
	CLogger::Print(L"*Exit: on_btnTest_clicked()");
}

void CEx01Dlg::RunMe()	
{
	CLogger::Print(L"--------------------| START LOGGING DIALOG 01 |--------------------");
	CEx01Dlg dlg;
	dlg.DoModal();
}
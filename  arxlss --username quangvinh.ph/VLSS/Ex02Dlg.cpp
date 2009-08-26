// Ex02Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Ex02Dlg.h"


// CEx02Dlg ダイアログ

IMPLEMENT_DYNAMIC(CEx02Dlg, CAcUiDialog)

CEx02Dlg::CEx02Dlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CEx02Dlg::IDD, pParent)
	, m_strFileName(_T(""))
{
	CLogger::Print(L"*Construct: CEx02Dlg");
}

CEx02Dlg::~CEx02Dlg()
{
	CLogger::Print(L"*Destruct: CEx02Dlg");
}

void CEx02Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_BITMAP, m_stcBitmap);
}


BEGIN_MESSAGE_MAP(CEx02Dlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CEx02Dlg::on_btnBrowse_clicked)
	ON_BN_CLICKED(IDCANCEL, &CEx02Dlg::on_btnCancle_clicked)
END_MESSAGE_MAP()


// CEx02Dlg メッセージ ハンドラ

void CEx02Dlg::on_btnBrowse_clicked()
{
	CLogger::Print(L"*Call: on_btnBrowse_clicked()");

	//------------
	// Open a dialog to select bitmap file.
	CFileDialog dlgOpenFile(TRUE,	NULL, NULL, OFN_FILEMUSTEXIST
		, L"Bitmap file (*.bmp)|*.bmp||", this);

	if (IDOK == dlgOpenFile.DoModal()) {
		//------------
		// After selected file, update the file path string into text box.
		// Then load bitmap into picture box.
		m_strFileName = dlgOpenFile.GetPathName();
		UpdateData(FALSE);

		// Load the bitmap from file.
		HBITMAP hBitmap = (HBITMAP) LoadImage(NULL, m_strFileName
			, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hBitmap != NULL) {
			m_stcBitmap.SetBitmap(hBitmap);
		} else {
			CLogger::Print(L"Warn: Fail to load image!");
		}
	} else {
		CLogger::Print(L"Warn: Have not choice any bimap file!");
	}

	CLogger::Print(L"*Exit: on_btnBrowse_clicked()");
}

void CEx02Dlg::RunMe(void)
{
	CLogger::Print(L"--------------------| START LOGGING DIALOG 02 |--------------------");
	CEx02Dlg dlg;
	dlg.DoModal();
}

void CEx02Dlg::on_btnCancle_clicked()
{
	OnCancel();
}

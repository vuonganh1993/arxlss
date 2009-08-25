#include "stdafx.h"
#include "Resource.h"
#include "Ex01Dlg.h"
#include "Logger.h"

IMPLEMENT_DYNAMIC(CEx01Dlg, CAcUiDialog)

CEx01Dlg::CEx01Dlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CEx01Dlg::IDD, pParent)
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
}


BEGIN_MESSAGE_MAP(CEx01Dlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CEx01Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEx01Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTNTEST, &CEx01Dlg::OnBnClickedBtntest)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CEx01Dlg::OnAcadKeepFocus)
END_MESSAGE_MAP()


// CEx01Dlg ���b�Z�[�W �n���h��

void CEx01Dlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	OnOK();
}

void CEx01Dlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
}

void CEx01Dlg::OnBnClickedBtntest()
{
	CLogger::Print(L"*Event: OnBnClickedBtntest()");
	CLogger::Print(L"*Exit: OnBnClickedBtntest()");
}

void CEx01Dlg::RunMe()	
{
	CLogger::Print(L"--------------------| DIALOG 01 |--------------------");
	CEx01Dlg dlg;
	dlg.DoModal();
}
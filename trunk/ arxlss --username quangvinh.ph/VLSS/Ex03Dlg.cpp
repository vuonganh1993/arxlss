// Ex03Dlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Ex03Dlg.h"


// CEx03Dlg �_�C�A���O

IMPLEMENT_DYNAMIC(CEx03Dlg, CAcUiDialog)

CEx03Dlg::CEx03Dlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CEx03Dlg::IDD, pParent)
{

}

CEx03Dlg::~CEx03Dlg()
{
}

void CEx03Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCONTROL, m_ctrList);
}


BEGIN_MESSAGE_MAP(CEx03Dlg, CAcUiDialog)
END_MESSAGE_MAP()


// CEx03Dlg ���b�Z�[�W �n���h��

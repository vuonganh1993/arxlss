// Ex03Dlg.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "Resource.h"
#include "Ex03Dlg.h"


IMPLEMENT_DYNAMIC(CEx03Dlg, CAcUiDialog)

CEx03Dlg::CEx03Dlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CEx03Dlg::IDD, pParent)
{
	CLogger::Print(L"*Construct: CEx03Dlg");
	m_lstImage.Create(32, 32, ILC_COLOR32, 7, 20);

	CBitmap bmp;
	for (int i = 0; i < 7; i++) {
		bmp.LoadBitmap(IDB_BITMAP1 + i);
		m_lstImage.Add(&bmp, RGB(0, 0, 0));
		bmp.Detach();
	}
}

CEx03Dlg::~CEx03Dlg()
{
	CLogger::Print(L"*Destruct: CEx03Dlg.");
}

void CEx03Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCONTROL, m_ctrList);
}


BEGIN_MESSAGE_MAP(CEx03Dlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CEx03Dlg::on_btnOk_clicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCONTROL, &CEx03Dlg::on_ctrList_doubleClick)
END_MESSAGE_MAP()


void CEx03Dlg::on_btnOk_clicked()
{
	CLogger::Print(L"*Call: on_btnOk_clicked()");
	m_ctrList.SetImageList(&m_lstImage, LVSIL_NORMAL);

	CString strTitle;
	for (int i = 0; i < 7; i++) {
		strTitle.Format(L"Icon %d", i+1);
		m_ctrList.InsertItem(i, strTitle, i);
	}
	CLogger::Print(L"*Exit: on_btnOk_clicked()");
}

void CEx03Dlg::on_ctrList_doubleClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_ctrList.GetFirstSelectedItemPosition();
	int nSelected = m_ctrList.GetNextSelectedItem(pos);
	m_ctrList.DeleteItem(nSelected);
	m_ctrList.InsertItem(nSelected, L"Hello", 6);
	*pResult = 0;
}

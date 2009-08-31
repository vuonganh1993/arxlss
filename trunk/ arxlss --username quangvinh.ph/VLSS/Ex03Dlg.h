#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "Logger.h"
#include "afxcmn.h"

// CEx03Dlg ダイアログ

class CEx03Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CEx03Dlg)

public:
	CEx03Dlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CEx03Dlg();

// ダイアログ データ
	enum { IDD = IDD_EX03DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	static void RunMe(void)
	{
		CLogger::Print(L"--------------------| START LOGGING DIALOG 03 |--------------------");
		CEx03Dlg dlg;
		dlg.DoModal();
	}
private:
	CListCtrl m_ctrList;
	CImageList m_lstImage;
public:
	afx_msg void on_btnOk_clicked();
	afx_msg void on_ctrList_doubleClick(NMHDR *pNMHDR, LRESULT *pResult);
};

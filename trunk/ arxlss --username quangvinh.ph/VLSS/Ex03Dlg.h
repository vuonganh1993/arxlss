#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "afxcmn.h"

// CEx03Dlg �_�C�A���O

class CEx03Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CEx03Dlg)

public:
	CEx03Dlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CEx03Dlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_EX03DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
};

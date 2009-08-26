#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "Logger.h"

class CEx02Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CEx02Dlg)

public:
	CEx02Dlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CEx02Dlg();

// ダイアログ データ
	enum { IDD = IDD_EX02DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
private:
	CString m_strFileName;
public:
	CStatic m_stcBitmap;
	afx_msg void on_btnBrowse_clicked();

public:
	static void RunMe(void);
	afx_msg void on_btnCancle_clicked();
};

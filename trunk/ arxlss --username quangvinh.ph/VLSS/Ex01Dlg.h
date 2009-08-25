#pragma once


// CEx01Dlg ダイアログ

class CEx01Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CEx01Dlg)

public:
	CEx01Dlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CEx01Dlg();

// ダイアログ データ
	enum { IDD = IDD_EX01DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtntest();
	afx_msg LONG OnAcadKeepFocus(UINT, LONG)
	{
		return FALSE;
	}
};

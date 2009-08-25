/********************************************************************
	File Name:		Ex01Dlg.h
	Author:			Pham Quang Vinh
	Purpose:		This file declare CEx01Dlg class that is drived from CAcUiDialog class.
						Practice for MFC support.
*********************************************************************/

#ifndef Ex01Dlg_h__
#define Ex01Dlg_h__

class CEx01Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CEx01Dlg)

public:
	CEx01Dlg(CWnd* pParent = NULL); 
	virtual ~CEx01Dlg();

	enum { IDD = IDD_EX01DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void on_btnOk_clicked();
	afx_msg void on_btnCancel_clicked();
	afx_msg void on_btnTest_clicked();

	static void RunMe();
public:
	CString m_strUser;
public:
	CString m_strPass;
};

#endif // Ex01Dlg_h__

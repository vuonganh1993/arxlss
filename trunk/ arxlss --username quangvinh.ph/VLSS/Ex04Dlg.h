#pragma once


// CEx04Dlg �_�C�A���O

class CEx04Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CEx04Dlg)

public:
	CEx04Dlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CEx04Dlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_EX04DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void on_btnCopy_clicked();
	afx_msg void on_btnPaste_clicked();

public:
	static void RunMe();
	CString m_strCopy;
	CString m_strPaste;
};

#pragma once


// CEx01Dlg �_�C�A���O

class CEx01Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CEx01Dlg)

public:
	CEx01Dlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CEx01Dlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_EX01DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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

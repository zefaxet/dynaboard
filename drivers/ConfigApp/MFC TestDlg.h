
// MFC TestDlg.h : header file
//

#pragma once


// CMFCTestDlg dialog
class CMFCTestDlg : public CDialogEx
{
// Construction
public:
	CMFCTestDlg(CWnd* pParent = nullptr);	// standard constructor
	void OnBnClickedButton1();
	void OnBnClickedButton2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbutton();
	afx_msg void OnBnClickedRecv();
	afx_msg void OnEnChangeEdit1();
};

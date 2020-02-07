
// MFC TestDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC Test.h"
#include "MFC TestDlg.h"
#include "afxdialogex.h"

#include "winioctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TODO shared header?
#define DEVICE_SEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_WRITE_DATA)
#define DEVICE_RECV CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_DATA)

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCTestDlg dialog



CMFCTestDlg::CMFCTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(open_button, &OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &OnBnClickedButton2)
	ON_BN_CLICKED(send_button, &CMFCTestDlg::OnBnClickedbutton)
	ON_BN_CLICKED(recv_button, &CMFCTestDlg::OnBnClickedRecv)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCTestDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CMFCTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMFCTestDlg message handlers

BOOL CMFCTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HANDLE deviceHandle;

void CMFCTestDlg::OnBnClickedButton1()
{
	deviceHandle = CreateFile(L"\\\\.\\mydevicelink123", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (deviceHandle == INVALID_HANDLE_VALUE) MessageBox(L"Invalid Device Handle", 0, 0);
	else MessageBox(L"Valid Device Handle\r\nOpen", 0, 0);

}

void CMFCTestDlg::OnBnClickedButton2()
{

	if (deviceHandle != INVALID_HANDLE_VALUE && deviceHandle != NULL)
	{
		if (CloseHandle(deviceHandle))
		{
			MessageBox(L"Valid Device Handle Closed");
			deviceHandle = NULL;
		}
		else MessageBox(L"Valid Device Handle Failed to Close");
	}
	else MessageBox(L"Nothing to Close: Invalid Device Handle");

}

void CMFCTestDlg::OnBnClickedbutton()
{

	PWCHAR message = L"send sample from mfc";
	ULONG returnLength = 0;
	wchar_t wr[4] = { 0 };
	if (deviceHandle != INVALID_HANDLE_VALUE && deviceHandle != NULL)
	{
		if (!DeviceIoControl(deviceHandle, DEVICE_SEND, message, (wcslen(message) + 1) * 2, NULL, 0, &returnLength, 0))
		{
			MessageBox(L"Failed to send message to driver.");
		}
		else
		{
			_ultow_s(returnLength, wr, 10);
			MessageBox(wr, 0, 0);
		}
	}
	
}

void CMFCTestDlg::OnBnClickedRecv()
{

	WCHAR message[1024] = { 0 };
	ULONG returnLength = 0;

	if (deviceHandle != INVALID_HANDLE_VALUE && deviceHandle != NULL)
	{
		if (!DeviceIoControl(deviceHandle, DEVICE_RECV, NULL, 0, message, 1024, &returnLength, 0)) 
			MessageBox(L"Failed to receive message from driver.");
		else MessageBox(message, 0, 0);
	}

}


void CMFCTestDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CMFCTestDlg::OnBnClickedOk()
{
	HANDLE hComm = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
	CString string;
	GetDlgItemText(IDC_EDIT1, string);

	const CString command = L"./setchar.sh ";
	CString execute = command + string + L"\n";
	int t = execute.GetLength();
	DWORD written;
	if (WriteFile(hComm, execute, execute.GetLength() * 2 - 1, &written, NULL))
	{
		MessageBoxW(L"Successfully communicated configuration to Pi");
	}
	else
	{
		MessageBoxW(L"Failed to write to serial port given for the Pi.");
	}
	CloseHandle(hComm);
}

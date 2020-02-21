
// MFC TestDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC Test.h"
#include "MFC TestDlg.h"
#include "afxdialogex.h"

#include "winioctl.h"

#include <sstream>
#include <vector>

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
	, selected_character(_T(""))
	, character_set(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, CHARACTER_LIST, characters);
	DDX_LBString(pDX, CHARACTER_LIST, selected_character);
	DDX_Control(pDX, CHARACTER_SET_LIST, character_sets);
	DDX_CBString(pDX, CHARACTER_SET_LIST, character_set);
}

BEGIN_MESSAGE_MAP(CMFCTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCTestDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(CHARACTER_SET_LIST, &CMFCTestDlg::OnLbnSelchangeSetList)
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

	CString errormsg;
	HKEY unicode_key;
	LSTATUS status = RegOpenKeyEx(HKEY_CURRENT_USER, L"Control Panel\\Input Method", 0, KEY_QUERY_VALUE, &unicode_key);
	if (status != ERROR_SUCCESS)
	{
		errormsg = "Error occured when opening Input Method registry key.";
		goto error;
	}

	BYTE data[4];
	DWORD type, ndata;
	status = RegQueryValueEx(unicode_key, L"EnableHexNumpad", NULL, &type, data, &ndata);
	if (status == ERROR_FILE_NOT_FOUND || status == ERROR_SUCCESS_REBOOT_REQUIRED || data[0] != '1')
	{
		if (MessageBox(
			L"Before running the configuration application for the first time, some system changes need to be made and a reboot will be required. Would you like to do this now?",
			L"First Time Setup",
			MB_YESNO) == IDNO) exit(1);
		status = RegOpenKeyEx(unicode_key, L"", 0, KEY_SET_VALUE, &unicode_key);
		if (status != ERROR_SUCCESS)
		{
			errormsg = "Error in opening registry key.";
			goto error;
		}
		const BYTE data[2] = { '1', 0 };
		status = RegSetValueEx(unicode_key, L"EnableHexNumpad", 0, REG_SZ, data, 2);
		if (status != ERROR_SUCCESS)
		{
			errormsg = "Error in setting registry key value.";
			goto error;
		}
		if (!ExitWindows(0x2, SHTDN_REASON_MINOR_RECONFIG))
		{
			errormsg = "Failed to initiate Windows shutdown.";
			status = GetLastError();
			goto error;
		}
		goto exit;
	}
	else if (status == ERROR_ACCESS_DENIED)
	{
		MessageBox(L"Please run the application as an administrator.", L"Error");
		goto exit;
	}
	else if (status != ERROR_SUCCESS)
	{
		errormsg = "An unknown error has occurred.";
		goto error;
	}
	RegCloseKey(unicode_key);

	character_sets.AddString(L"English Alphabet");
	//character_sets.AddString(L"Russian Alphabet");
	//character_sets.AddString(L"Arabic Abjad");
	character_sets.SetCurSel(0);
	character_set = L"English Alphabet";
	OnLbnSelchangeSetList();

	return TRUE;  // return TRUE  unless you set the focus to a control

exit:
	if (unicode_key != 0)
		RegCloseKey(unicode_key);
	exit(status);

error:
	std::wstringstream stream;
	stream << errormsg << std::endl << "Status Code: " << status;
	MessageBox(stream.str().c_str(), L"Error");
	goto exit;
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


void CMFCTestDlg::OnBnClickedOk()
{
	std::wstringstream comfile;
	comfile << "COM";
	int status = false;
	int com_port = GetDlgItemInt(COM_FIELD, &status, 0);
	if (!status)
	{
	invalid_com:
		MessageBox(L"Please enter a valid COM port number");
		return;
	}
	comfile << com_port;

	characters.GetLBText(characters.GetCurSel(), selected_character);

	HANDLE hComm = CreateFile(comfile.str().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
	if (GetLastError() == ERROR_FILE_NOT_FOUND)
		goto invalid_com;

	unsigned char Header[2] = { 0xFE, 0xFE };
	DWORD written;
	const wchar_t t = L'ж';
	//WriteFile(hComm, Header, 2, &written, NULL);
	const CString command = L"./setchar.sh ";
	CString execute = command + selected_character + L"\n";
	if (WriteFile(hComm, (LPCTSTR)execute, execute.GetLength() * 2 + 2, &written, NULL))
	{
		MessageBoxW(L"Successfully communicated configuration to Pi");
	}
	else
	{
		MessageBoxW(L"Failed to write to serial port given for the Pi.");
	}
	CloseHandle(hComm);
}

const std::vector<wchar_t *> English = {
	L"A", L"B", L"C", L"D",
	L"E", L"F", L"G", L"H",
	L"I", L"J", L"K", L"L",
	L"M", L"N", L"O", L"P",
	L"Q", L"R", L"S", L"T",
	L"U", L"V", L"W", L"X",
	L"Y", L"Z"
};

const std::vector<wchar_t *> Russian = {
	L"А", L"Б", L"В", L"Г",
	L"Д", L"Е", L"Ё", L"Ж",
	L"З", L"И", L"Й", L"К",
	L"Л", L"М", L"Н", L"О",
	L"П", L"Р", L"С", L"Т",
	L"У", L"Ф", L"Х", L"Ц",
	L"Ч", L"Ш", L"Щ", L"Ъ",
	L"Ы", L"Ь", L"Э", L"Ю",
	L"Я"
};

const std::vector<CString> Arabic = {
	L"ي", L"و", L"ه", L"ن",
	L"م", L"ل", L"ك", L"ق",
	L"ف", L"غ", L"ع", L"ظ",
	L"ط", L"ض", L"ص", L"ش",
	L"س", L"ز", L"ر", L"ذ",
	L"د", L"خ", L"ح", L"ج",
	L"ث", L"ت", L"ب", L"ا"
};

// Character set list
void CMFCTestDlg::OnLbnSelchangeSetList()
{
	character_sets.GetLBText(character_sets.GetCurSel(), character_set);
	characters.ResetContent();
	std::vector<wchar_t *> alphabet;

	if (character_set == "English Alphabet")
	{
		alphabet = English;
	}
	else if (character_set == "Russian Alphabet")
	{
		alphabet = Russian;
	}
	else if (character_set == "Arabic Abjad")
	{
		//alphabet = Arabic;
	}
	else
	{
		MessageBox(L"An error has occurred.", L"Error");
		character_set = "English Alphabet";
		OnLbnSelchangeSetList();
		return;
	}

	for each (wchar_t * letter in alphabet)
	{
		characters.AddString(letter);
	}
	characters.SetCurSel(0);
}
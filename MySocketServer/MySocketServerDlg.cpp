
// MySocketServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MySocketServer.h"
#include "MySocketServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMySocketServerDlg 对话框



CMySocketServerDlg::CMySocketServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYSOCKETSERVER_DIALOG, pParent)
	, m_send_text(_T(""))
	, m_recv_text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMySocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_send_text);
	DDX_Text(pDX, IDC_EDIT2, m_recv_text);
}

BEGIN_MESSAGE_MAP(CMySocketServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CMySocketServerDlg::OnBnClickedSend)
	ON_MESSAGE(SOCKET_EVENT, &CMySocketServerDlg::OnSocket)
	ON_BN_CLICKED(IDC_SEND2, &CMySocketServerDlg::OnBnClickedSend2)
END_MESSAGE_MAP()


BOOL CMySocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitSock();
	return TRUE;  
}

void CMySocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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



void CMySocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CMySocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMySocketServerDlg::OnBnClickedSend()
{
	m_server_sock.OnSend(0);
}


void CMySocketServerDlg::InitSock()
{
	m_server_sock.AttachCWnd(this);
	m_accept_sock.AttachCWnd(this);

	if (!m_server_sock.Create(8888,SOCK_STREAM,INADDR_ANY))
	{
		MessageBox(L"FAILED TO CREATE SERVER SOCKET");
	}
	if(!m_server_sock.Listen())
	{
		MessageBox(L"FAILED TO LISTEN");
    }
	//m_server_sock.OnAccept(0);
}




LRESULT CMySocketServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	SOCKADDR_IN sockAddr;
	int            nSize = sizeof(sockAddr);
	BOOL        res;

	switch (lParam)
	{
	case ACCEPT:
		res = m_server_sock.Accept(m_accept_sock, (SOCKADDR *)&sockAddr, &nSize);
		if (res == FALSE)
			MessageBox(_T("Accept Error!"));
		break;

	case SEND:                       
		SendText(m_accept_sock);
		break;

	case RECV:                       
		RecvText(m_accept_sock);
		break;

	case CLOSE: 
		m_server_sock.Close();
		break;

	}
	return TRUE;
}

char* CMySocketServerDlg::UsrCStringToChar(CString str)
{
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *cs_text = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, cs_text, len, NULL, NULL);
	return cs_text;
}

void CMySocketServerDlg::SendText(CMySocket& sock)
{
	UpdateData(TRUE);
	char *sz_buff = NULL;
	sz_buff = UsrCStringToChar(m_send_text);
	sock.Send(sz_buff,sizeof(sz_buff),0);
}

CString CMySocketServerDlg::AnsiToUnicode(char * szAnsi)
{
	CString str;
	int wcsLen = 0;
	wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	str = wszString;
	delete wszString;
	return str;
}


void CMySocketServerDlg::RecvText(CMySocket& sock)
{
	char sz_buff[1024];
	sock.Receive(sz_buff,1024,0);
	m_recv_text = AnsiToUnicode(sz_buff);
	UpdateData(FALSE);
}

void CMySocketServerDlg::OnBnClickedSend2()
{
	// TODO: 在此添加控件通知处理程序代码
}

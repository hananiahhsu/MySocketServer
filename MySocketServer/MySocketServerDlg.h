#pragma once
#include "MySocket.h"

class CMySocketServerDlg : public CDialogEx
{
public:
	CMySocketServerDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSOCKETSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSend();
	LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
	CString m_send_text;
	CString m_recv_text;

	CMySocket m_server_sock;
	CMySocket m_accept_sock;

public:
	void InitSock();
	void SendText(CMySocket& sock);
	void RecvText(CMySocket& sock);
	char* UsrCStringToChar(CString str);
	CString AnsiToUnicode(char * szAnsi);
	afx_msg void OnBnClickedSend2();
};

// TradeMarkFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "TradeMarkFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTradeMarkFrame dialog


CTradeMarkFrame::CTradeMarkFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeMarkFrame::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradeMarkFrame)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTradeMarkFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeMarkFrame)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTradeMarkFrame, CDialog)
	//{{AFX_MSG_MAP(CTradeMarkFrame)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeMarkFrame message handlers

void CTradeMarkFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(0);
//	CDialog::OnTimer(nIDEvent);
}

BOOL CTradeMarkFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CTradeMarkFrame::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos(NULL,0,0,539,369, SWP_NOMOVE | SWP_NOZORDER ); //设置窗口与位图一样大
	// TODO: Add extra initialization here
	SetTimer(1,3000,0); // 位图保留3秒
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

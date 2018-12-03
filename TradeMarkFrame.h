#if !defined(AFX_TRADEMARKFRAME_H__EBF32557_3830_40F9_B20E_BC248C5A30B5__INCLUDED_)
#define AFX_TRADEMARKFRAME_H__EBF32557_3830_40F9_B20E_BC248C5A30B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TradeMarkFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTradeMarkFrame dialog

class CTradeMarkFrame : public CDialog
{
// Construction
public:
	CTradeMarkFrame(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTradeMarkFrame)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeMarkFrame)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTradeMarkFrame)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADEMARKFRAME_H__EBF32557_3830_40F9_B20E_BC248C5A30B5__INCLUDED_)

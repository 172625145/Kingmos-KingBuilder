#if !defined(AFX_OUTPUTVIEW_H__FA565192_048D_4A96_943C_C605511774AF__INCLUDED_)
#define AFX_OUTPUTVIEW_H__FA565192_048D_4A96_943C_C605511774AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputView.h : header file
//
#include "MessageView.h"

/////////////////////////////////////////////////////////////////////////////
// COutputView view

class COutputView : public CMessageView
{
protected:
	COutputView();           // protected constructor used by dynamic creation
	~COutputView();
	DECLARE_DYNCREATE(COutputView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnChange();
	afx_msg void OnEditPaste();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTVIEW_H__FA565192_048D_4A96_943C_C605511774AF__INCLUDED_)

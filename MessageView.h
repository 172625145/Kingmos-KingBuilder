#if !defined(AFX_MESSAGEVIEW_H__44B700F2_83FB_4F81_87EE_62255118100E__INCLUDED_)
#define AFX_MESSAGEVIEW_H__44B700F2_83FB_4F81_87EE_62255118100E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageView.h : header file
//
#include "OutPutMessageEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CMessageView view

class CMessageView : public CView
{
protected:
	CMessageView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMessageView)

	COutputMessageEdit* m_hMessageEdit;

// Attributes
public:

// Operations
public:
	void SetWindowsFileInfo( BOOL bWindowsFileInfo = FALSE);
	BOOL AddString(LPCTSTR lpMessage);

	void OnEditFind();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMessageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMessageView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(int nChar, int nRepCnt, int nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEVIEW_H__44B700F2_83FB_4F81_87EE_62255118100E__INCLUDED_)

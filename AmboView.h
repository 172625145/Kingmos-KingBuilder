// AmboView.h : interface of the CAmboView class
//
/////////////////////////////////////////////////////////////////////////////
#include "Specialview.h"

#if !defined(AFX_AMBOVIEW_H__2AE321DB_FD61_4437_BD39_3CC76001C88C__INCLUDED_)
#define AFX_AMBOVIEW_H__2AE321DB_FD61_4437_BD39_3CC76001C88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//class CAmboView : public CSpecialView
class CAmboView : public CSpecialView
{
protected: // create from serialization only
	CAmboView();
	DECLARE_DYNCREATE(CAmboView)

// Attributes
public:
	CAmboDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmboView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAmboView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAmboView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AmboView.cpp
inline CAmboDoc* CAmboView::GetDocument()
   { return (CAmboDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBOVIEW_H__2AE321DB_FD61_4437_BD39_3CC76001C88C__INCLUDED_)

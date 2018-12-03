#if !defined(AFX_AMBOCONFIGVIEW_H__DFA7A9D5_C03D_472A_A649_54A40ED6F795__INCLUDED_)
#define AFX_AMBOCONFIGVIEW_H__DFA7A9D5_C03D_472A_A649_54A40ED6F795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AmboConfigView.h : header file
//
#include "WorkSpaceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CAmboConfigView view

class CAmboConfigView : public CTreeView
{
protected:
	CAmboConfigView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAmboConfigView)


//	CString m_strWorkSpaceName;
//	CString m_strWorkSpaceRoot;
//	CString m_strKingmosRoot;
	CWorkSpaceInfo *m_pWorkSpaceInfo;
	CImageList *m_imgList;

	BOOL OpenCaretFile();

// Attributes
public:
	CProjectDoc *GetDocument ();

	BOOL ConfigWorkSpace(CWorkSpaceInfo *pWorkSpaceInfo);

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmboConfigView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAmboConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAmboConfigView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in mlTreeView.cpp
inline CProjectDoc* CAmboConfigView::GetDocument()
   { return (CProjectDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBOCONFIGVIEW_H__DFA7A9D5_C03D_472A_A649_54A40ED6F795__INCLUDED_)

#if !defined(AFX_MLVIEWTAB_H__C53055DF_2E65_4A1F_BDBA_F333E8D209D6__INCLUDED_)
#define AFX_MLVIEWTAB_H__C53055DF_2E65_4A1F_BDBA_F333E8D209D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mlViewTab.h : header file
//
#include <afxtempl.h>
#include "resource.h"       // main symbols

typedef struct
{
	CWnd *pWnd;
	TCHAR szLabel[32];
}VTC_ITEM;

/////////////////////////////////////////////////////////////////////////////
// CmlViewTab window

class CmlViewTab : public CTabCtrl
{
// Construction
public:
	CmlViewTab();

// Attributes
public:

// Operations
public:
	void SetActiveView(int nNewTab);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmlViewTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	CDocument * GetDocument ();
	CDocument* SetDocument (CRuntimeClass *pDocClass);

	BOOL AddView (LPCTSTR lpszLabel, CRuntimeClass *pViewClass, int iIndex = -1, int iImage = -1);

	CView* GetActiveView();
	CView* GetView(int nView);
	void RemoveAllView();
	void RemoveViewAt(int nView);

	virtual ~CmlViewTab();

	// Generated message map functions
protected:
	BOOL RelayoutViews ();
	CRect m_rectView;
	VTC_ITEM *GetItemData (int nItem);
	CImageList	m_ImageList;

	CDocument*	m_pDocument;

public:
	//{{AFX_MSG(CmlViewTab)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#undef AFX_DATA
#define AFX_DATA
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLVIEWTAB_H__C53055DF_2E65_4A1F_BDBA_F333E8D209D6__INCLUDED_)

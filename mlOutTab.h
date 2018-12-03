#if !defined(AFX_MLOUTTAB_H__413D6421_DD6A_45ED_8738_F76826313ACC__INCLUDED_)
#define AFX_MLOUTTAB_H__413D6421_DD6A_45ED_8738_F76826313ACC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mlOutTab.h : header file
#include "OutputMessageEdit.h"
//
typedef struct
{
	CWnd	*pWnd;
	TCHAR	lpszLabel[32];
}OTC_ITEM;

/////////////////////////////////////////////////////////////////////////////
// CmlOutTab window

class CmlOutTab : public CTabCtrl
{
// Construction
public:
	CmlOutTab();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmlOutTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	CView * GetActiveView();
	void SetWindowsFileInfo( UINT iViewIndex,BOOL bWindowsFileInfo = FALSE);
	int AddView (LPCTSTR lpstrLabel, CRuntimeClass *pViewClass,int iIndex);
	virtual ~CmlOutTab();
//	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
//	int HitTest(TCHITTESTINFO* pHitTestInfo) const;
	BOOL AddString (UINT iViewIndex,LPCTSTR lpstrContent);
	BOOL Empty (UINT iViewIndex);
	void RemoveViewAt (int nView);
	void RemoveAllView();
	void SetActiveView(int nNewTab);
	COutputMessageEdit &GetTabEdit(int iItem);
	// Generated message map functions
protected:
	void GetViewRect (CRect &rect);
	OTC_ITEM * GetItemData (int nItem);
	//{{AFX_MSG(CmlOutTab)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(int nChar, int nRepCnt, int nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLOUTTAB_H__413D6421_DD6A_45ED_8738_F76826313ACC__INCLUDED_)

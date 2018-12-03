#if !defined(AFX_SPECIALVIEW_H__392CBB1C_7D16_4D3C_82ED_9A76A83A39CB__INCLUDED_)
#define AFX_SPECIALVIEW_H__392CBB1C_7D16_4D3C_82ED_9A76A83A39CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecialView.h : header file
//
#include "SpecialEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CSpecialView view

class CSpecialView : public CView
{
protected:
	CSpecialView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSpecialView)

	CSpecialEdit *h_Edit;
// Attributes
public:
	LPCTSTR LockBuffer() const;
	void UnlockBuffer() const;
	UINT GetBufferLength() const;

// Operations
public:
	void SetModify(BOOL bModified);
	BOOL GetModify();
	void SaveOldFile();
	int m_nPrintPages;
	int *m_pnPages;
	CFont *m_pPrintFont;
	int m_nPrintLineHeight;
	BOOL m_bPrintHeader, m_bPrintFooter;
	CRect m_ptPageArea, m_rcPrintArea;

	void SerializeRaw(CArchive& ar);
	void WriteToArchive(CArchive& ar);
	void ReadFromArchive(CArchive& ar, UINT nLen);
	void OnEditFind();

	UINT nMaxSize;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecialView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSpecialView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void PrintFooter(CDC *pdc, int nPageNum);
	void PrintHeader(CDC *pdc, int nPageNum);
	void GetPrintFooterText(int nPageNum, CString &text);
	void GetPrintHeaderText(int nPageNum, CString &text);
	void RecalcPageLayouts(CDC *pdc, CPrintInfo *pInfo);
	LPTSTR m_pShadowBuffer;     // special shadow buffer only used in Win32s
	UINT m_nShadowSize;
	//{{AFX_MSG(CSpecialView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditToggleBookmark();
	afx_msg void OnEditGotoNextBookmark();
	afx_msg void OnUpdateEditGotoNextBookmark(CCmdUI* pCmdUI);
	afx_msg void OnEditGotoPrevBookmark();
	afx_msg void OnUpdateEditGotoPrevBookmark(CCmdUI* pCmdUI);
	afx_msg void OnEditClearAllBookmarks();
	afx_msg void OnUpdateEditClearAllBookmarks(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnSetsyscolor();
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECIALVIEW_H__392CBB1C_7D16_4D3C_82ED_9A76A83A39CB__INCLUDED_)

#if !defined(AFX_AMBOMATTERVIEW_H__EC3EBB75_ADAC_4808_AAEA_11292E5B0377__INCLUDED_)
#define AFX_AMBOMATTERVIEW_H__EC3EBB75_ADAC_4808_AAEA_11292E5B0377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AmboMatterView.h : header file
//
#include "WorkSpaceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CAmboMatterView view

class CAmboMatterView : public CTreeView
{
protected:
	CAmboMatterView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAmboMatterView)

	CWorkSpaceInfo *m_pWorkSpaceInfo;

// Attributes
public:
//	CProjectDoc *GetDocument ();

// Operations
public:
	BOOL IsFullPath(CString strFolder);
	void RefreshModules(LPCTSTR lpModulesName);
	void GetCurFilePath(HTREEITEM hTree,CString& strFilePath);
	int LoadFolder(HTREEITEM hTreeItem,CString strFolder);
	BOOL OpenWorkSpace(CWorkSpaceInfo *pWorkSpaceInfo);
	BOOL OpenCaretFile();
	BOOL HaveSelFeature();
	int GetSelFeature(CString &strPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmboMatterView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAmboMatterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CImageList *imgList;

	// Generated message map functions
protected:
	//{{AFX_MSG(CAmboMatterView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void TransCygwinFolderToMSFolder(CString &strCygWinFolder,CString &strFolder);
	void MargePath(CString strParentPath,CString strChildPath,CString &strFullPath);
	void GetFullPath(CString strCurPath , CString strFolderName, CString &strFullPath);
	void LoadKingmos(HTREEITEM hRoot);
};
/*
#ifndef _DEBUG  // debug version in mlTreeView.cpp
inline CProjectDoc* CAmboMatterView::GetDocument()
   { return (CProjectDoc*)m_pDocument; }
#endif
*/
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBOMATTERVIEW_H__EC3EBB75_ADAC_4808_AAEA_11292E5B0377__INCLUDED_)

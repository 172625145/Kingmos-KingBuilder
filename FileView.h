#if !defined(AFX_FILEVIEW_H__61125C96_0EAA_435E_B74C_291C3A532CE2__INCLUDED_)
#define AFX_FILEVIEW_H__61125C96_0EAA_435E_B74C_291C3A532CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileView.h : header file
//
#include "WorkSpaceInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CFileView view

class CFileView : public CTreeView
{
protected:
	CFileView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFileView)

// Attributes
public:

// Operations
public:
	BOOL LoadFolder(HTREEITEM hTreeItem,CString strPath);
	BOOL OpenWorkSpace(CWorkSpaceInfo *pWorkSpaceInfo);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CImageList *m_pImgList;
	// Generated message map functions
protected:
	//{{AFX_MSG(CFileView)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEVIEW_H__61125C96_0EAA_435E_B74C_291C3A532CE2__INCLUDED_)

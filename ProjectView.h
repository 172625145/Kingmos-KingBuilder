#if !defined(AFX_PROJECTVIEW_H__24381FB6_5561_4B5F_A3A4_B34BE33CF5A6__INCLUDED_)
#define AFX_PROJECTVIEW_H__24381FB6_5561_4B5F_A3A4_B34BE33CF5A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectView.h : header file
//
#include "ProjectInfo.h"

#define IMAGE_MDL_ROOT					0
#define IMAGE_MDL_GROUP                 1
#define IMAGE_MDL_LIST	                2
#define IMAGE_MDL_MODULE                3
#define IMAGE_MDL_DIRECTORY             4
#define IMAGE_MDL_SOURCE	            5
/////////////////////////////////////////////////////////////////////////////
// CProjectView view

class CProjectView : public CTreeView
{
protected:
	CProjectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CProjectView)

// Attributes
public:

// Operations
public:
	BOOL OpenProject(LPCTSTR lpProject);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectView)
	public:
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InsertSpecialFile(CString strFilename);
	HTREEITEM GetSingleStageChild(CString strContent);
	int GetFileType(CString strFileName);
	LPPROJECTITEM GetProjectItem(HTREEITEM hItem);
	void UpdateProject(CProjectInfo *pProjectInfo);
	void UpdateView();
	void RefreshModules(LPCTSTR lpModulesName);
//	CDocument * GetDocument ();
//	CDocument* SetDocument (CRuntimeClass *pDocClass);
	void OnInsertfile();


protected:
	virtual ~CProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
//	BOOL OrganizeProject();
//	BOOL ReadProject(LPCTSTR lpstrDoc);
	void InsertProjectItem(HTREEITEM hTree,LPPROJECTITEM lpProjectItem);

	BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog);
//	CDocument*	m_pDocument;
	CImageList *m_pImgList;
//	CsrcData m_srcData;
//	CPtrList m_SourceLib;
//	CPtrList m_TargetLib;

	CProjectInfo *m_pProjectInfo;
	//{{AFX_MSG(CProjectView)
	afx_msg void OnDestroy();
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDeleteitem();
	afx_msg void OnUpdateDeleteitem(CCmdUI* pCmdUI);
	afx_msg void OnInsertfolder();
	afx_msg void OnUpdateInsertfolder(CCmdUI* pCmdUI);
	afx_msg void OnCompile();
	afx_msg void OnUpdateCompile(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL OpenCaretFile();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTVIEW_H__24381FB6_5561_4B5F_A3A4_B34BE33CF5A6__INCLUDED_)

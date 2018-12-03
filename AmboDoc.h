// AmboDoc.h : interface of the CAmboDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMBODOC_H__3B363214_587D_44CE_AB42_32BDA10EFF68__INCLUDED_)
#define AFX_AMBODOC_H__3B363214_587D_44CE_AB42_32BDA10EFF68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAmboDoc : public CDocument
{
protected: // create from serialization only
	CAmboDoc();
	DECLARE_DYNCREATE(CAmboDoc)

// Attributes
public:
	void UpdateFileAttrib( void );
	void CheckFileChanged(void);
	BOOL IsFileChanged(void);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmboDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL IsModified();
	virtual void SetModifiedFlag(BOOL bModified);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual POSITION GetFirstViewPosition() const;
	virtual CView* GetNextView(POSITION& rPosition) const;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAmboDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	FILETIME m_ftLastWriteTime;
    ULONG    m_fileSizeLow;
	ULONG    m_fileSizeHigh;

// Generated message map functions
protected:
	//{{AFX_MSG(CAmboDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBODOC_H__3B363214_587D_44CE_AB42_32BDA10EFF68__INCLUDED_)

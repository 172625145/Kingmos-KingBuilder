#if !defined(AFX_PROJECTDOC_H__83E82950_C926_480A_94D2_FACBB2B85386__INCLUDED_)
#define AFX_PROJECTDOC_H__83E82950_C926_480A_94D2_FACBB2B85386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc document

class CProjectDoc : public CDocument
{
protected:
	CProjectDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CProjectDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CProjectDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTDOC_H__83E82950_C926_480A_94D2_FACBB2B85386__INCLUDED_)

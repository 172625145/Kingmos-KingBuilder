// ProjectDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "ProjectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc

IMPLEMENT_DYNCREATE(CProjectDoc, CDocument)

CProjectDoc::CProjectDoc()
{
}

BOOL CProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CProjectDoc::~CProjectDoc()
{
}


BEGIN_MESSAGE_MAP(CProjectDoc, CDocument)
	//{{AFX_MSG_MAP(CProjectDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc diagnostics

#ifdef _DEBUG
void CProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc serialization

void CProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc commands

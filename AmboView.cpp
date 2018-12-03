// AmboView.cpp : implementation of the CAmboView class
//

#include "stdafx.h"
#include "Ambo.h"
#include "Specialview.h"

#include "AmboDoc.h"
#include "AmboView.h"
#include "MainFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmboView

IMPLEMENT_DYNCREATE(CAmboView, CSpecialView)

BEGIN_MESSAGE_MAP(CAmboView, CSpecialView)
	//{{AFX_MSG_MAP(CAmboView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CSpecialView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CSpecialView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CSpecialView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmboView construction/destruction

CAmboView::CAmboView()
{
	// TODO: add construction code here

}

CAmboView::~CAmboView()
{
}

BOOL CAmboView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CSpecialView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAmboView drawing

void CAmboView::OnDraw(CDC* pDC)
{
//	CAmboDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CAmboView printing

BOOL CAmboView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAmboView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAmboView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAmboView diagnostics

#ifdef _DEBUG
void CAmboView::AssertValid() const
{
	CSpecialView::AssertValid();
}

void CAmboView::Dump(CDumpContext& dc) const
{
	CSpecialView::Dump(dc);
}

CAmboDoc* CAmboView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAmboDoc)));
	return (CAmboDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAmboView message handlers

void CAmboView::OnInitialUpdate() 
{
	CSpecialView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
//	GetTreeCtrl().InsertItem (TEXT("Ambo"));
//	GetTreeCtrl().InsertItem (TEXT("Ambo1"));
}



//BOOL CAmboView::DestroyWindow() 
//{
//	// TODO: Add your specialized code here and/or call the base class
//	
//	return CSpecialView::DestroyWindow();
//}

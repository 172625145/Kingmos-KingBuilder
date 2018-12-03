// CreateSelectPage.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "CreateSelectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateSelectPage property page

IMPLEMENT_DYNCREATE(CCreateSelectPage, CPropertyPage)

CCreateSelectPage::CCreateSelectPage() : CPropertyPage(CCreateSelectPage::IDD)
{
	//{{AFX_DATA_INIT(CCreateSelectPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCreateSelectPage::~CCreateSelectPage()
{
}

void CCreateSelectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateSelectPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateSelectPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCreateSelectPage)
	ON_BN_CLICKED(IDC_WORKSPACE, OnWorkspace)
	ON_BN_CLICKED(IDC_PROJECT, OnProject)
	ON_BN_CLICKED(IDC_TEXTFILE, OnTextfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateSelectPage message handlers

BOOL CCreateSelectPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class

   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
   psheet->SetWizardButtons(PSWIZB_NEXT);

   CButton *pCreateWorkSpace = (CButton *)GetDlgItem(IDC_WORKSPACE);
   pCreateWorkSpace->SetCheck(1);

   iSelectRoute = ROUTE_WORKSPACE;
	
	return CPropertyPage::OnSetActive();
}

void CCreateSelectPage::OnWorkspace() 
{
	// TODO: Add your control notification handler code here
   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
   psheet->SetWizardButtons(PSWIZB_NEXT);
   iSelectRoute = ROUTE_WORKSPACE;
	
}

void CCreateSelectPage::OnProject() 
{
	// TODO: Add your control notification handler code here
   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
   psheet->SetWizardButtons(PSWIZB_NEXT);
   iSelectRoute = ROUTE_PROJECT;	
}

void CCreateSelectPage::OnTextfile() 
{
	// TODO: Add your control notification handler code here
   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
   psheet->SetWizardButtons(PSWIZB_FINISH);	
   iSelectRoute = ROUTE_TEXTFILE;
}

LRESULT CCreateSelectPage::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
    CPropertySheet* psheet = (CPropertySheet*) GetParent();   
	
	if (iSelectRoute == ROUTE_WORKSPACE)
	{
		psheet->SetActivePage(2);
		return 2;
	}
	if (iSelectRoute == ROUTE_PROJECT)
	{
		psheet->SetActivePage(1);
		return 1;
	}
	return -1;
//	return CPropertyPage::OnWizardNext();
}

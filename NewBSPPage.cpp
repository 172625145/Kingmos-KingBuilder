// NewBSPPage.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "NewBSPPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewBSPPage property page

IMPLEMENT_DYNCREATE(CNewBSPPage, CPropertyPage)

CNewBSPPage::CNewBSPPage() : CPropertyPage(CNewBSPPage::IDD)
{
	//{{AFX_DATA_INIT(CNewBSPPage)
	m_strDescription = _T("");
	m_strNewBSPName = _T("");
	m_strVendor = _T("");
	m_strVersion0 = _T("1");
	m_strVersion1 = _T("0");
	m_strVersion2 = _T("0");
	m_strVersion3 = _T("0");
	//}}AFX_DATA_INIT
}

CNewBSPPage::~CNewBSPPage()
{
}

void CNewBSPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewBSPPage)
	DDX_Text(pDX, IDC_DESCRIPTION, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, 256);
	DDX_Text(pDX, IDC_NEWBSPNAME, m_strNewBSPName);
	DDV_MaxChars(pDX, m_strNewBSPName, 64);
	DDX_Text(pDX, IDC_Vendor, m_strVendor);
	DDV_MaxChars(pDX, m_strVendor, 32);
	DDX_Text(pDX, IDC_VERSION0, m_strVersion0);
	DDV_MaxChars(pDX, m_strVersion0, 2);
	DDX_Text(pDX, IDC_VERSION1, m_strVersion1);
	DDV_MaxChars(pDX, m_strVersion1, 2);
	DDX_Text(pDX, IDC_VERSION2, m_strVersion2);
	DDV_MaxChars(pDX, m_strVersion2, 2);
	DDX_Text(pDX, IDC_VERSION3, m_strVersion3);
	DDV_MaxChars(pDX, m_strVersion3, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewBSPPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNewBSPPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewBSPPage message handlers

BOOL CNewBSPPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CNewBSPPage::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	UpdateData();
	return CPropertyPage::OnWizardFinish();
}

BOOL CNewBSPPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
   psheet->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}

LRESULT CNewBSPPage::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	UpdateData();
	if (m_strNewBSPName.IsEmpty())
	{
		MessageBox("Please Input BSP name!!!");
		CWnd *pWnd = GetDlgItem(IDC_NEWBSPNAME);
		pWnd->SetFocus();
		return -1;
	}
	return CPropertyPage::OnWizardNext();
}

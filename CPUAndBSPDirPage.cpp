// CPUAndBSPDirPage.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "CPUAndBSPDirPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPUAndBSPDirPage property page

IMPLEMENT_DYNCREATE(CCPUAndBSPDirPage, CPropertyPage)

CCPUAndBSPDirPage::CCPUAndBSPDirPage() : CPropertyPage(CCPUAndBSPDirPage::IDD)
{
	//{{AFX_DATA_INIT(CCPUAndBSPDirPage)
	m_strBSPPlatformName = _T("");
	m_strCPUType = _T("");
	//}}AFX_DATA_INIT
}

CCPUAndBSPDirPage::~CCPUAndBSPDirPage()
{
}

void CCPUAndBSPDirPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPUAndBSPDirPage)
	DDX_Control(pDX, IDC_CPUTYPE, m_ListCPUType);
	DDX_CBString(pDX, IDC_BSPPLATFORMNAME, m_strBSPPlatformName);
	DDV_MaxChars(pDX, m_strBSPPlatformName, 64);
	DDX_LBString(pDX, IDC_CPUTYPE, m_strCPUType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPUAndBSPDirPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCPUAndBSPDirPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPUAndBSPDirPage message handlers

BOOL CCPUAndBSPDirPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
    psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	
	return CPropertyPage::OnSetActive();
}

BOOL CCPUAndBSPDirPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strBuilderRoot;
	CString strCPUTypeFile;
	CString strCPUDatal;

	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strBuilderRoot);
	strCPUTypeFile =strBuilderRoot + "\\script\\cputype.";
	CFile cpuTypefile(strCPUTypeFile, CFile::modeRead);
	DWORD dwFileLen = cpuTypefile.GetLength();
	cpuTypefile.Read(strCPUDatal.GetBuffer(dwFileLen+1),dwFileLen);
	strCPUDatal.ReleaseBuffer();
	cpuTypefile.Close();
//	m_ListCPUType.AddString("ARM7");
//	m_ListCPUType.AddString("ARMV4I");
	int iStart,ilen,iNext;
	CString strItemName;

	iStart = 0;
	while(iStart != -1)
	{
		iNext = strCPUDatal.Find("\r\n",iStart);
		if (iNext == -1)
		{
			ilen = (strCPUDatal.GetLength() -1 ) - iStart;  // (strCPUDatal.GetLength() -1 )  The last Char position is (length -1)
			if (ilen == 0)
				break;
		}
		else
		{
			ilen = iNext - iStart;
		}
		if (ilen == 0)
		{
			iStart += strlen("\r\n");
			continue;
		}
		strItemName = strCPUDatal.Mid(iStart,ilen);
		if (strItemName.IsEmpty() == FALSE)
			m_ListCPUType.AddString(strItemName);
		iStart = iNext ;
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPUAndBSPDirPage::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData();

	if (m_strBSPPlatformName.IsEmpty())
	{
		MessageBox("Please Input BSP Platfrom Name!!!");
		CWnd *pWnd = GetDlgItem(IDC_BSPPLATFORMNAME);
		pWnd->SetFocus();
		return FALSE;
	}
	if (m_strCPUType.IsEmpty())
	{
		MessageBox("Please Select CPU Type!!!");
		m_ListCPUType.SetFocus();
		return FALSE;
	}
	return CPropertyPage::OnWizardFinish();
}

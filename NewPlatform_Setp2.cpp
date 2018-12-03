// NewPlatform_Setp2.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "NewPlatform_Setp2.h"

#include "BSPConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp2 property page

IMPLEMENT_DYNCREATE(CNewPlatform_Setp2, CPropertyPage)

CNewPlatform_Setp2::CNewPlatform_Setp2() : CPropertyPage(CNewPlatform_Setp2::IDD)
{
	//{{AFX_DATA_INIT(CNewPlatform_Setp2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CNewPlatform_Setp2::~CNewPlatform_Setp2()
{
}

void CNewPlatform_Setp2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPlatform_Setp2)
	DDX_Control(pDX, IDC_BSPLIST, m_listBSPs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPlatform_Setp2, CPropertyPage)
	//{{AFX_MSG_MAP(CNewPlatform_Setp2)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_BSPLIST, OnSelchangeBsplist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp2 message handlers

BOOL CNewPlatform_Setp2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	WIN32_FIND_DATA  FindFileData;
	HANDLE hFindFile;
	CString strSearch;
	CString strBspFile;
	CString strKingbuilderRoot;
	
	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKingbuilderRoot);

	strSearch = strKingbuilderRoot + "\\bspconfig\\*.bcf";
	hFindFile = FindFirstFile(strSearch.LockBuffer(),&FindFileData);
	if( hFindFile != INVALID_HANDLE_VALUE) 
	{
		while(1)
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//m_listBSPs.AddString(FindFileData.cFileName);
				strBspFile = strKingbuilderRoot + "\\bspconfig\\";
				strBspFile +=FindFileData.cFileName;

				CBSPConfig BSpConfig;
				BSpConfig.LoadBSP(strBspFile);
				CString strBSPName,strCPUType,strItemName;
				BSpConfig.GetBSPName(strBSPName);
				BSpConfig.GetCPUType(strCPUType);
				strItemName = strBSPName;
				strItemName += ":";
				strItemName +=  strCPUType;
				DWORD dwIndex = m_listBSPs.AddString(strItemName);
				TCHAR *pFileName = new TCHAR[strBspFile.GetLength() + 1];
				strcpy(pFileName,strBspFile);
				m_listBSPs.SetItemData(dwIndex,(DWORD)pFileName);
			}
			if( FindNextFile(hFindFile,&FindFileData)==0 )  //查找下一个文件
			{
				break;
			}
		}
		FindClose(hFindFile);
	}
	strSearch.UnlockBuffer();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewPlatform_Setp2::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	int nCount,nIndex;
	TCHAR *pFileName ;

		nCount = m_listBSPs.GetCount();

		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			pFileName = (TCHAR *)m_listBSPs.GetItemData(nIndex);
			if (pFileName)
				delete pFileName;
		}
	
}

void CNewPlatform_Setp2::OnSelchangeBsplist() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_listBSPs.GetCurSel();
	LPTSTR lpFileName;
	lpFileName = (LPTSTR)m_listBSPs.GetItemData(nIndex);
	m_strBSPFileName = lpFileName;
	
}

BOOL CNewPlatform_Setp2::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
    CPropertySheet* psheet = (CPropertySheet*) GetParent();   

//    psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);

	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	
	return CPropertyPage::OnSetActive();
}

LRESULT CNewPlatform_Setp2::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_strBSPFileName.IsEmpty())
	{
		MessageBox("Please Select a BSP!!!");
		CWnd *pWnd = GetDlgItem(IDC_BSPLIST);
		pWnd->SetFocus();
		return -1;
	}
	return CPropertyPage::OnWizardNext();
}

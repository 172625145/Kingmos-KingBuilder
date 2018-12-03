// NewPlatform_Setp1.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "NewPlatform_Setp1.h"
#include "shlobj.h"
#include "Winbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


AFX_STATIC_DATA const TCHAR LocateWorkSpaceRootSection[] = _T("WorkSpace Locate Root");
AFX_STATIC_DATA const TCHAR LocateWorkSpaceRootEntry[] = _T("WorkSpace Locate Root");

AFX_STATIC_DATA const TCHAR LocatePlatformRootSection[] = _T("Platform Locate Root");
AFX_STATIC_DATA const TCHAR LocatePlatformRootEntry[] = _T("Platform Locate Root");
/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp1 property page

IMPLEMENT_DYNCREATE(CNewPlatform_Setp1, CPropertyPage)

CNewPlatform_Setp1::CNewPlatform_Setp1() : CPropertyPage(CNewPlatform_Setp1::IDD)
{
	//{{AFX_DATA_INIT(CNewPlatform_Setp1)
	m_strPlatformName = _T("");
	m_strLocation = _T("");
	m_strPlatformRoot = _T("");
	//}}AFX_DATA_INIT
}

CNewPlatform_Setp1::~CNewPlatform_Setp1()
{
}

void CNewPlatform_Setp1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPlatform_Setp1)
	DDX_Text(pDX, IDC_PLATFORMNAME, m_strPlatformName);
	DDV_MaxChars(pDX, m_strPlatformName, 256);
	DDX_Text(pDX, IDC_LOCATION, m_strLocation);
	DDV_MaxChars(pDX, m_strLocation, 256);
	DDX_Text(pDX, IDC_PLATFORMROOT, m_strPlatformRoot);
	DDV_MaxChars(pDX, m_strPlatformRoot, 260);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPlatform_Setp1, CPropertyPage)
	//{{AFX_MSG_MAP(CNewPlatform_Setp1)
	ON_BN_CLICKED(IDC_BROWRES, OnBrowres)
	ON_EN_CHANGE(IDC_PLATFORMNAME, OnChangePlatformname)
	ON_EN_CHANGE(IDC_LOCATION, OnChangeLocation)
	ON_EN_CHANGE(IDC_PLATFORMROOT, OnChangePlatformroot)
	ON_BN_CLICKED(IDC_BROWRES2, OnBrowres2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPlatform_Setp1 message handlers

LRESULT CNewPlatform_Setp1::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	if (m_strPlatformName.IsEmpty())
	{
		MessageBox("Please Input Platform name!!!");
		CWnd *pWnd = GetDlgItem(IDC_PLATFORMNAME);
		pWnd->SetFocus();
		return -1;
	}
	if (m_strLocation.IsEmpty())
	{
		MessageBox("Please Input Location!!!");
		CWnd *pWnd = GetDlgItem(IDC_LOCATION);
		pWnd->SetFocus();
		return -1;
	}
	//m_strLocation	我们将要判断路径是否存在，如果不存在则创建它，如果不能创建则返回失败。
	if (CreateMultiDirectory(m_strLocation)== FALSE)
	{
		MessageBox("Invalidate Location!!!");
		CWnd *pWnd = GetDlgItem(IDC_LOCATION);
		pWnd->SetFocus();
		return -1;
	}
	((CAmboApp *)AfxGetApp())->WriteProfileString(LocateWorkSpaceRootSection,LocateWorkSpaceRootEntry,m_strLocationRoot);
	((CAmboApp *)AfxGetApp())->WriteProfileString(LocatePlatformRootSection,LocatePlatformRootEntry,m_strPlatformRoot);
	return CPropertyPage::OnWizardNext();
}

BOOL CNewPlatform_Setp1::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
    CPropertySheet* psheet = (CPropertySheet*) GetParent();   
    psheet->SetWizardButtons(PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}

void CNewPlatform_Setp1::OnBrowres() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	Browse(m_strLocation);
	CString strNewRoot;
	if (Browse(strNewRoot) == TRUE)
	{
		m_strLocationRoot = strNewRoot;
		m_strLocation = m_strLocationRoot + "\\";
		m_strLocation += m_strPlatformName;
	}
	UpdateData(FALSE);
}

BOOL CNewPlatform_Setp1::Browse(CString &csPath)
{
	BROWSEINFO bi;
	bi.hwndOwner = this->m_hWnd; 
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("Browse ...");
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_EDITBOX ;             
	bi.lpfn = NULL;
	bi.lParam = NULL;

    LPITEMIDLIST iil = SHBrowseForFolder(&bi);

	if(iil)
	{
		TCHAR	*pBuf = new TCHAR[MAX_PATH];

		SHGetPathFromIDList (iil, pBuf);
		csPath = pBuf;
		delete[] pBuf;

		return TRUE;
	}
	csPath.Empty ();
	return FALSE;
}

BOOL CNewPlatform_Setp1::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CPropertyPage::OnWizardFinish();
}

BOOL CNewPlatform_Setp1::CreateMultiDirectory(
  LPCTSTR lpPathName                         // pointer to directory path string
  )
{
	CString lpDirectory;
	int bRoot = FALSE;

		lpDirectory = "";
		while(*lpPathName)
		{
			if (*lpPathName==':')
			{// The Disk Volume mark 
				bRoot = TRUE;
			}
			if (*lpPathName=='\\')
			{// a directory complete ,mult create this directory
				if (bRoot == FALSE)
					return FALSE; // No Disk Volume mark , Not Root path
				lpDirectory +=*lpPathName++;
				break;
			}
			lpDirectory +=*lpPathName++;
		}
		if (bRoot == FALSE)
			return FALSE; // No Disk Volume mark , Not Root path
		while(*lpPathName)
		{
			if (*lpPathName=='\\')
			{// a directory complete ,mult create this directory
				if (IsExistDirectory(lpDirectory) == FALSE)
				{
					if (CreateDirectory(lpDirectory,NULL) == 0)
						return FALSE;
				}
			}
			lpDirectory +=*lpPathName++;
		}
		if (IsExistDirectory(lpDirectory) == FALSE)
			return CreateDirectory(lpDirectory,NULL);
		return TRUE;
}

BOOL CNewPlatform_Setp1::IsExistDirectory(CString strDirectory)
{
	WIN32_FIND_DATA  FindFileData;
    HANDLE						hFind;

		hFind=FindFirstFile(strDirectory, &FindFileData  );  // 查找该目录是否存在
		if( hFind == INVALID_HANDLE_VALUE) 
		{// 文件不存在
			return FALSE;
		}
		while(TRUE)
		{  // 文件存在
			if( FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
			{
				// 查找到的是一个目录
				return TRUE;
			}
			if( FindNextFile(hFind,&FindFileData)==0 )  //查找下一个文件
			{ // 已经没有文件了
				break;
			}
		}
		FindClose(hFind);
		return FALSE;
}

BOOL CNewPlatform_Setp1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strDefaultRoot;
	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strDefaultRoot);
	strDefaultRoot+="\\WorkSpace";
	m_strLocationRoot = ((CAmboApp *)AfxGetApp())->GetProfileString(LocateWorkSpaceRootSection,LocateWorkSpaceRootEntry,strDefaultRoot);

	m_strLocation = m_strLocationRoot + "\\";
	m_strLocation += m_strPlatformName;

	CString strDefaultPlatformRoot;
	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strDefaultPlatformRoot);
	strDefaultPlatformRoot+="\\kingmos";
	m_strPlatformRoot = ((CAmboApp *)AfxGetApp())->GetProfileString(LocatePlatformRootSection,LocatePlatformRootEntry,strDefaultPlatformRoot);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewPlatform_Setp1::OnChangePlatformname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strLocation = m_strLocationRoot + "\\";
	m_strLocation += m_strPlatformName;
	UpdateData(FALSE);
	
}

void CNewPlatform_Setp1::OnChangeLocation() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strLocationRoot = m_strLocation.Left(m_strLocation.ReverseFind('\\'));
	UpdateData(FALSE);
	
}

void CNewPlatform_Setp1::OnChangePlatformroot() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CNewPlatform_Setp1::OnBrowres2() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	Browse(m_strLocation);
	CString strNewRoot;
	if (Browse(strNewRoot) == TRUE)
	{
		m_strPlatformRoot = strNewRoot;
	}
	UpdateData(FALSE);
}

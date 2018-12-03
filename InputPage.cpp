// InputPage.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "InputPage.h"
#include "shlobj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


AFX_STATIC_DATA const TCHAR LocateRootSection[] = _T("Project Locate Root");
AFX_STATIC_DATA const TCHAR LocateRootEntry[] = _T("Project Locate Root");
/////////////////////////////////////////////////////////////////////////////
// CInputPage property page

IMPLEMENT_DYNCREATE(CInputPage, CPropertyPage)

CInputPage::CInputPage() : CPropertyPage(CInputPage::IDD)
{
	//{{AFX_DATA_INIT(CInputPage)
	m_CPUType = 0;
	m_strCPUType = _T("");
	m_strLocation = _T("");
	m_strWorkSpaceName = _T("");
	m_bLoadFromSource = FALSE;
	m_strSourceLocation = _T("");
	//}}AFX_DATA_INIT
	m_strLocationRoot = _T("");
}

CInputPage::~CInputPage()
{
}

void CInputPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPage)
	DDX_Control(pDX, IDC_WORKSPACESNAME, m_ctrlWorkSpaceName);
	DDX_Control(pDX, IDC_SOURCELOCATION, m_crtlSourceLocation);
	DDX_Control(pDX, IDC_SOURCEBROWES, m_ctrlSourceBrowse);
	DDX_Control(pDX, IDC_LOCATION, m_ctrlLocation);
	DDX_Control(pDX, IDC_BROWES, m_ctrlBrowse);
	DDX_Control(pDX, IDC_PROJECTTYPE, m_ctrlProjectType);
	DDX_Control(pDX, IDC_CPUSELECT, m_ctrlCPUSelect);
	DDX_CBIndex(pDX, IDC_CPUSELECT, m_CPUType);
	DDX_Text(pDX, IDC_LOCATION, m_strLocation);
	DDX_Text(pDX, IDC_WORKSPACESNAME, m_strWorkSpaceName);
	DDX_Check(pDX, IDC_LOADFROMSOURCE, m_bLoadFromSource);
	DDX_Text(pDX, IDC_SOURCELOCATION, m_strSourceLocation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPage, CPropertyPage)
	//{{AFX_MSG_MAP(CInputPage)
	ON_BN_CLICKED(IDC_BROWES, OnBrowes)
	ON_CBN_SELCHANGE(IDC_CPUSELECT, OnSelchangeCpuselect)
	ON_BN_CLICKED(IDC_LOADFROMSOURCE, OnLoadfromsource)
	ON_BN_CLICKED(IDC_SOURCEBROWES, OnSourcebrowes)
	ON_EN_CHANGE(IDC_WORKSPACESNAME, OnChangeWorkspacesname)
	ON_LBN_SELCHANGE(IDC_PROJECTTYPE, OnSelchangeProjecttype)
	ON_EN_CHANGE(IDC_LOCATION, OnChangeLocation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPage message handlers

void CInputPage::OnBrowes() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	Browse(m_strLocation);
	CString strNewRoot;
	if (Browse(strNewRoot) == TRUE)
	{
		m_strLocationRoot = strNewRoot;
		m_strLocation = m_strLocationRoot + "\\";
		m_strLocation += m_strWorkSpaceName;
	}
	UpdateData(FALSE);
}

BOOL CInputPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
   CPropertySheet* psheet = (CPropertySheet*) GetParent();   
   psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	
	return CPropertyPage::OnSetActive();
}

BOOL CInputPage::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if (m_bLoadFromSource == FALSE)
	{
		if (m_strProjectType.IsEmpty() == TRUE)
		{
			MessageBox("Please Select Project Type");
			m_ctrlProjectType.SetFocus();
			return FALSE;
		}
		if (m_strLocation.IsEmpty() == TRUE)
		{
			m_ctrlLocation.SetFocus();
			return FALSE;
		}
		if (m_strWorkSpaceName.IsEmpty() == TRUE)
		{
			MessageBox("Invalidate Project Name!!!");
			m_ctrlWorkSpaceName.SetFocus();
			return FALSE;
		}
		//m_strLocation	我们将要判断路径是否存在，如果不存在则创建它，如果不能创建则返回失败。
		if (CreateMultiDirectory(m_strLocation)== FALSE)
		{
			MessageBox("Invalidate Location!!!");
			m_ctrlLocation.SetFocus();
			return -1;
		}
	}
	else
	{
		// Create Project From Modules
		
		if (m_strSourceLocation.IsEmpty() == TRUE)
		{
			MessageBox("Invalidate Mudule Name!!!");
			m_crtlSourceLocation.SetFocus();
			return FALSE;
		}
	}

//	((CAmboApp *)AfxGetApp())->CreateNewWorkSpace(m_strWorkSpaceName, m_strLocation, m_CPUType);
	((CAmboApp *)AfxGetApp())->WriteProfileString(LocateRootSection,LocateRootEntry,m_strLocationRoot);
	
	return CPropertyPage::OnWizardFinish();
}
BOOL CInputPage::Browse(CString &csPath)
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


BOOL CInputPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
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
		m_ctrlCPUSelect.AddString(strItemName);
		iStart = iNext ;
	}	
	m_ctrlCPUSelect.SetCurSel(0);
	m_ctrlCPUSelect.GetLBText(0, m_strCPUType );

	m_ctrlProjectType.AddString("应用程序(EXE)");
	m_ctrlProjectType.AddString("动态链接库(DLL)");
	m_ctrlProjectType.AddString("静态链接库(LIB)");

	m_ctrlWorkSpaceName.EnableWindow(TRUE);
	m_ctrlLocation.EnableWindow(TRUE);
	m_ctrlBrowse.EnableWindow(TRUE);
	m_ctrlProjectType.EnableWindow(TRUE);

	m_crtlSourceLocation.EnableWindow(FALSE);
	m_ctrlSourceBrowse.EnableWindow(FALSE);

	CString strDefaultRoot;
	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strDefaultRoot);
	strDefaultRoot+="\\Project";
	m_strLocationRoot = ((CAmboApp *)AfxGetApp())->GetProfileString(LocateRootSection,LocateRootEntry,strDefaultRoot);

	m_strLocation = m_strLocationRoot + "\\";
	m_strLocation += m_strWorkSpaceName;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputPage::OnSelchangeCpuselect() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctrlCPUSelect.GetCurSel();
	if (nIndex == CB_ERR)
		return ;
	m_ctrlCPUSelect.GetLBText(nIndex, m_strCPUType );
	
}

void CInputPage::OnLoadfromsource() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if (m_bLoadFromSource)
	{// 选择从一个SOURCE文件中导入一个工程
		m_ctrlWorkSpaceName.EnableWindow(FALSE);
		m_ctrlLocation.EnableWindow(FALSE);
		m_ctrlBrowse.EnableWindow(FALSE);
		m_ctrlProjectType.EnableWindow(FALSE);

		m_crtlSourceLocation.EnableWindow(TRUE);
		m_ctrlSourceBrowse.EnableWindow(TRUE);
	}
	else
	{// 选择创建一个新的一个工程
		m_ctrlWorkSpaceName.EnableWindow(TRUE);
		m_ctrlLocation.EnableWindow(TRUE);
		m_ctrlBrowse.EnableWindow(TRUE);
		m_ctrlProjectType.EnableWindow(TRUE);

		m_crtlSourceLocation.EnableWindow(FALSE);
		m_ctrlSourceBrowse.EnableWindow(FALSE);
	}
}

void CInputPage::OnSourcebrowes() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFile(TRUE);

	CString title;
	VERIFY(title.LoadString(IDS_OPENSOURCE));

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

	CString strFilter;

	UpdateData(TRUE);

	// append the Source files filter
	CString strTextFilter;
	VERIFY(strTextFilter.LoadString(IDS_SOURCEFILTER));
	strFilter += strTextFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("MODULES");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = m_strSourceLocation.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	m_strSourceLocation.ReleaseBuffer();

	UpdateData(FALSE);
	return ;
	
}

void CInputPage::OnChangeWorkspacesname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strLocation = m_strLocationRoot + "\\";
	m_strLocation += m_strWorkSpaceName;
	UpdateData(FALSE);
	
}

BOOL CInputPage::CreateMultiDirectory(
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

BOOL CInputPage::IsExistDirectory(CString strDirectory)
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

void CInputPage::OnSelchangeProjecttype() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctrlProjectType.GetCurSel();
	
	switch(nIndex)
	{
		case 0:
			m_strProjectType = "PROGRAM";
			break;
		case 1:
			m_strProjectType = "DYNAMIC";
			break;
		case 2:
			m_strProjectType = "LIBRARY";
			break;
	}
}

void CInputPage::OnChangeLocation() 
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

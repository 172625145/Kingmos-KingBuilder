// DirectoryList.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "DirectoryList.h"
#include "shlobj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDC_LISTBOX  300
#define ID_INSERT  301
#define ID_DELETE  302
#define ID_UPLINE  303
#define ID_DOWNLINE  304
/////////////////////////////////////////////////////////////////////////////
// CDirectoryList

CDirectoryList::CDirectoryList()
{
	m_pListBox = new CListBox;
	m_pInsert = new CBitmapButton;
	m_pDelete = new CBitmapButton;
	m_pUpLine = new CBitmapButton;
	m_pDownLine = new CBitmapButton;

	m_strTitle = _T("Directory:");
	m_iContent = DIRECTROY_LIST;
}

CDirectoryList::~CDirectoryList()
{
	if (m_pListBox)
		delete m_pListBox;
	if (m_pInsert)
		delete m_pInsert ;
	if (m_pDelete)
		delete m_pDelete ;
	if (m_pUpLine)
		delete m_pUpLine ;
	if (m_pDownLine)
		delete m_pDownLine ;
}


BEGIN_MESSAGE_MAP(CDirectoryList, CWnd)
	//{{AFX_MSG_MAP(CDirectoryList)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DELETEITEM()
	ON_COMMAND(ID_INSERT, OnInsert)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_UPLINE, OnUpLine)
	ON_COMMAND(ID_DOWNLINE, OnDownLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDirectoryList message handlers


void CDirectoryList::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	CBrush brush(RGB(0xff,0,0));
	RECT rect;
	GetClientRect(&rect);
	rect.bottom = rect.top + 20;
//	dc.FillRect(&rect,&brush);
	dc.DrawEdge(&rect,BDR_SUNKENOUTER ,BF_RECT);

	rect.top += 2;
	rect.left += 2;
	int nBkMode = dc.SetBkMode(TRANSPARENT);
	dc.DrawText((LPCTSTR)m_strTitle,&rect,DT_NOCLIP);
	dc.SetBkMode(nBkMode);
	
	// Do not call CWnd::OnPaint() for painting messages
}

int CDirectoryList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	RECT rect;
	GetClientRect(&rect);
	rect.top +=20;
	m_pListBox->Create(WS_CHILD|WS_VISIBLE,//|LBS_NOINTEGRALHEIGHT,
					rect,
					this,
					IDC_LISTBOX);
	m_pListBox->ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	rect.right -= 2;
	rect.left =rect.right - 16;
	rect.top =2;
	rect.bottom =18;
	m_pDownLine->Create("Insert",WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_OWNERDRAW  ,rect,this,ID_DOWNLINE);
	m_pDownLine->LoadBitmaps(IDB_DOWNLINE,IDB_DOWNLINE,IDB_DOWNLINE,IDB_DOWNLINE);
	rect.right =rect.left - 2;
	rect.left =rect.right - 16;
	m_pUpLine->Create("Insert",WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_OWNERDRAW  ,rect,this,ID_UPLINE);
	m_pUpLine->LoadBitmaps(IDB_UPLINE,IDB_UPLINE,IDB_UPLINE,IDB_UPLINE);
	rect.right =rect.left - 2;
	rect.left =rect.right - 16;
	m_pDelete->Create("Insert",WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_OWNERDRAW  ,rect,this,ID_DELETE);
	m_pDelete->LoadBitmaps(IDB_DELETE,IDB_DELETE,IDB_DELETE,IDB_DELETE);
	rect.right =rect.left - 2;
	rect.left =rect.right - 16;
	m_pInsert->Create("Insert",WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_OWNERDRAW ,rect,this,ID_INSERT);
	m_pInsert->LoadBitmaps(IDB_INSERT,IDB_INSERT,IDB_INSERT,IDB_INSERT);

	return 0;
}

void CDirectoryList::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}

void CDirectoryList::OnInsert() 
{
	//插入一个条目
	CString strDirectory;
	if (m_iContent == DIRECTROY_LIST)
	{
		if (Browse(strDirectory)== TRUE)
		{
			int iIndex = m_pListBox->AddString((LPCTSTR)strDirectory);
			m_pListBox->SetCurSel(iIndex);
		}
	}
	else
	{
		if (DoPromptFileName(strDirectory)== TRUE)
		{
			int iIndex = m_pListBox->AddString((LPCTSTR)strDirectory);
			m_pListBox->SetCurSel(iIndex);
		}
	}
}
void CDirectoryList::OnDelete() 
{
	//删除当前选择项
	int iIndex = m_pListBox->GetCurSel();// 得到当前选择项
	if (iIndex != LB_ERR)
	{
		m_pListBox->DeleteString(iIndex);
		if (iIndex >= m_pListBox->GetCount())  // 已经到了最上一行，不能移动
			iIndex = m_pListBox->GetCount() -1;
		m_pListBox->SetCurSel(iIndex);
	}
}
void CDirectoryList::OnUpLine() 
{
	// 将当前选择项上移一行
	int iIndex = m_pListBox->GetCurSel();// 得到当前选择项
	if (iIndex != LB_ERR)
	{
		if (iIndex == 0)  // 已经到了最上一行，不能移动
			return ;
		CString strSel;
		m_pListBox->GetText( iIndex, strSel ); // 得到选择项的内容
		m_pListBox->DeleteString(iIndex);
		iIndex--;
		m_pListBox->InsertString(iIndex,strSel);
		m_pListBox->SetCurSel(iIndex);
	}
}
void CDirectoryList::OnDownLine() 
{
	// 将当前选择项下移一行
	int iIndex = m_pListBox->GetCurSel();// 得到当前选择项
	if (iIndex != LB_ERR)
	{
		if (iIndex == (m_pListBox->GetCount() -1))  // 已经到了最上一行，不能移动
			return ;
		CString strSel;
		m_pListBox->GetText( iIndex, strSel ); // 得到选择项的内容
		m_pListBox->DeleteString(iIndex);
		iIndex++;
		m_pListBox->InsertString(iIndex,strSel);
		m_pListBox->SetCurSel(iIndex);
	}
}

BOOL CDirectoryList::Browse(CString &csPath)
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

void CDirectoryList::SetTitle(CString strTitle)
{
	m_strTitle = strTitle;
}

BOOL CDirectoryList::DoPromptFileName(CString& fileName)
{
	CFileDialog dlgFile(TRUE);

	CString title;
	VERIFY(title.LoadString(IDS_INSERTFILE));

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

	CString strFilter;

	if (m_strSpecial.IsEmpty() == FALSE && m_strSpecialFilter.IsEmpty() == FALSE)
	{
		// append the C,C++ files filter
		strFilter += m_strSpecial;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += m_strSpecialFilter;
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;
	}

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
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}

void CDirectoryList::SetContentType(int iType)
{
	if (iType == DIRECTROY_LIST || iType == FILE_LIST )
	{
		m_iContent = iType;
	}
}

void CDirectoryList::SetSpecialString(CString strSpecial)
{
	m_strSpecial = strSpecial;
}

void CDirectoryList::SetSpecialFilterString(CString strSpecialFilter)
{
	m_strSpecialFilter = strSpecialFilter;
}

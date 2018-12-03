// ProjectView.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "ProjectView.h"
#include "srcdata.h"
//#include "ProjectDoc.h"
#include "PlatformSet_Page1.h"
#include "PlatformSet_Page2.h"

#include "NewFolder.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
#define FILE_SOURCE		1 // C,CPP
#define FILE_HEADER		2 // H,HPP
#define	FILE_LIBRARY	3 // o,a,so
#define FILE_EXTERN		4 // other file ,will can't insert to the project, only use by the user

/////////////////////////////////////////////////////////////////////////////
// CProjectView

IMPLEMENT_DYNCREATE(CProjectView, CTreeView)

CProjectView::CProjectView()
{
	m_pImgList = NULL;
	m_pProjectInfo = NULL;
}

CProjectView::~CProjectView()
{
//	GetTreeCtrl().DeleteAllItems();
//	m_SourceLib.RemoveAll();
//	m_TargetLib.RemoveAll();
}


BEGIN_MESSAGE_MAP(CProjectView, CTreeView)
	//{{AFX_MSG_MAP(CProjectView)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_DELETEITEM, OnDeleteitem)
	ON_UPDATE_COMMAND_UI(IDM_DELETEITEM, OnUpdateDeleteitem)
	ON_COMMAND(IDM_INSERTFOLDER, OnInsertfolder)
	ON_UPDATE_COMMAND_UI(IDM_INSERTFOLDER, OnUpdateInsertfolder)
	ON_COMMAND(ID_COMPILE, OnCompile)
	ON_UPDATE_COMMAND_UI(ID_COMPILE, OnUpdateCompile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectView drawing
BOOL CProjectView::PreTranslateMessage(MSG *pMsg)
{	
	if (pMsg->message == WM_KEYDOWN && (UINT)pMsg->wParam == VK_RETURN)
	{
		OpenCaretFile();
		return 1;
	}
	return CTreeView::PreTranslateMessage(pMsg);	
}
void CProjectView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CProjectView diagnostics

#ifdef _DEBUG
void CProjectView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CProjectView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectView message handlers

void CProjectView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if (m_pImgList == NULL)
	{  // 创建一个ImageList
		m_pImgList = new CImageList;
		if (m_pImgList == NULL)
			return;
		//m_pImgList->Create(16,16,ILC_MASK,16,16);
		//添加两个ICON到ImageList
		//m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_ICONFOLDER));
		//m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_ICONFILE));
		m_pImgList->Create(16,16,ILC_COLOR8|ILC_MASK,16,16);
		//添加两个ICON到ImageList
		m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_ROOT));
		m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_GROUP));
		m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_LIST));
		m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_MODULE));
		m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_DIRECTORY));
		m_pImgList->Add(AfxGetApp()->LoadIcon(IDI_SOURCE));
		
//		if (! m_pImgList->Create (IDB_TABIMAGES, 16, 1, RGB(255,0,255)))
//		{
		//	AfxSetResourceHandle (hOldRes);
//			return;
//		}
		GetTreeCtrl().SetImageList(m_pImgList,TVSIL_NORMAL);
	
		//Af*xSetResourceHandle (hOldRes);
	}	
	//SetDocument (RUNTIME_CLASS(CProjectDoc));
	UpdateWindow();
}

void CProjectView::OnDestroy() 
{
//	CString * pstrLibName;
	if (m_pImgList != NULL)
	{
		m_pImgList->DeleteImageList();
		delete m_pImgList;
	}
/*	
	m_srcData.RemoveAll();

	while (!m_SourceLib.IsEmpty())
	{
		pstrLibName = (CString *)m_SourceLib.GetHead();
		if (pstrLibName)
			delete pstrLibName;
		m_SourceLib.RemoveHead();
	}

	while (!m_TargetLib.IsEmpty())
	{
		pstrLibName = (CString *)m_TargetLib.GetHead();
		if (pstrLibName)
			delete pstrLibName;
		m_TargetLib.RemoveHead();
	}
*/
	GetTreeCtrl().DeleteAllItems();

	CTreeView::OnDestroy();	
}

BOOL CProjectView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 设置新TreeView的Style.
	cs.style |= TVS_LINESATROOT | TVS_HASLINES  |TVS_HASBUTTONS;
//	cs.style |= TVS_LINESATROOT | TVS_HASLINES  |TVS_SINGLEEXPAND |TVS_HASBUTTONS ;
	
	return CTreeView::PreCreateWindow(cs);
}
/*
BOOL CProjectView::ReadProject(LPCTSTR lpstrDoc)
{
	m_srcData.Load(lpstrDoc);
	return FALSE;
}
BOOL CProjectView::OrganizeProject()
{
	CString csTitle("Project: ");
	CString csName;
	CString csType;
	CString csSources;
	UINT iRet;

	// 得到工程文件名
	if (m_srcData.Get (TEXT("MODULENAME"), csName)
		&&m_srcData.Get (TEXT("MODULETYPE"), csType))
	{
		int iImage;
		if (csType.CompareNoCase (TEXT("PROGRAM")) == 0)
			iImage = IMAGE_MDL_ROOT;
		else if (csType.CompareNoCase (TEXT("DYNLINK")) == 0)
			iImage = IMAGE_MDL_ROOT;
		else
			iImage = IMAGE_MDL_ROOT;

		csTitle += csName;
		HTREEITEM hRoot = GetTreeCtrl().InsertItem (csName, iImage, iImage);

		// 5j
		HTREEITEM hSourceFile = GetTreeCtrl().InsertItem (CString("Source Files"), IMAGE_MDL_MODULE, IMAGE_MDL_MODULE, hRoot);
		if ((hRoot != NULL)&&m_srcData.Get (TEXT("LOCAL_FILES"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					GetTreeCtrl().InsertItem (csFile, IMAGE_MDL_SOURCE, IMAGE_MDL_SOURCE, hSourceFile);
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetTreeCtrl().InsertItem (csFile, IMAGE_MDL_SOURCE, IMAGE_MDL_SOURCE, hSourceFile);
				}
			}while (1);
		}

		// 得到工程需要的源库
		if (m_srcData.Get (TEXT("STATICLIBS"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					m_SourceLib.AddTail(new CString(csFile));
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					m_SourceLib.AddTail(new CString(csFile));
				}
			}while (1);
		}

		// 插入目标库
		// 得到工程需要的目标库
		if (m_srcData.Get (TEXT("DYNAMICLIBS"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					m_TargetLib.AddTail(new CString(csFile));
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					m_TargetLib.AddTail(new CString(csFile));
				}
			}while (1);
		}
	}
	iRet = TRUE;
	return TRUE;
}

BOOL CProjectView::OpenProject(LPCTSTR lpProject)
{
	ReadProject(lpProject);
//	OrganizeProject();
	UpdateView();
	return TRUE;
}

*/
void CProjectView::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
//	GetTreeCtrl().	
	*pResult = 0;
}

void CProjectView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OpenCaretFile();
	*pResult = 0;
}

void CProjectView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RETURN)
	{
		OpenCaretFile();
		return;
	}
	if (nChar == VK_DELETE)
	{
		OnDeleteitem();
		return;
	}
	CTreeView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CProjectView::OpenCaretFile()
{
	HTREEITEM hTree;
	TVITEM tvItem;
	
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree)
	{
		tvItem.mask = TVIF_HANDLE | TVIF_IMAGE;
		tvItem.hItem = hTree;
		GetTreeCtrl().GetItem(&tvItem);
		if (tvItem.iImage == IMAGE_MDL_SOURCE)
		{
			// 我们将要去打开这个选定的文件
			CString strFileName;
//			CString strFileName = GetTreeCtrl().GetItemText( hTree );
			LPPROJECTITEM lpProjectItem = GetProjectItem(hTree);
			strFileName = lpProjectItem->strFileName;
			AfxGetApp()->OpenDocumentFile(strFileName);			
		}
	}
	return TRUE;
}


/*
CDocument* CProjectView::SetDocument (CRuntimeClass *pDocClass)
{
	ASSERT(pDocClass != NULL);
	ASSERT(pDocClass->IsDerivedFrom(RUNTIME_CLASS(CDocument)));
	ASSERT(AfxIsValidAddress(pDocClass, sizeof(CRuntimeClass), FALSE));
	
	CDocument *pOldDoc = m_pDocument;
	TRY
	{
		m_pDocument = (CDocument *)pDocClass->CreateObject ();
		if (m_pDocument == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		m_pDocument = pOldDoc;

		TRACE0("Out of memory creating a document.\n");

		return NULL;
	}
	END_CATCH_ALL

	return pOldDoc;
}

CDocument *CProjectView::GetDocument()
{
	return m_pDocument;
}

void CProjectView::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}
*/

void CProjectView::RefreshModules(LPCTSTR lpModulesName)
{
//	ReadProject(lpProject);
//	OrganizeProject();
}

void CProjectView::UpdateView()
{

}

void CProjectView::UpdateProject(CProjectInfo *pProjectInfo)
{
		int iImage;

//		GetTreeCtrl().DeleteAllItems();
//		m_SourceLib.RemoveAll();
//		m_TargetLib.RemoveAll();

		m_pProjectInfo = pProjectInfo;
/*
		if (pProjectInfo->m_iProgramType == PROGRAM_EXE)
			iImage = IMAGE_MDL_ROOT;
		else if (pProjectInfo->m_iProgramType == PROGRAM_DLL)
			iImage = IMAGE_MDL_GROUP;
		else
			iImage = IMAGE_MDL_LIST;
*/
		if (pProjectInfo->m_strProgramType.Compare("PROGRAM") == 0)
		{
			iImage = IMAGE_MDL_MODULE;
		}
		else if (pProjectInfo->m_strProgramType.Compare("DYNAMIC") == 0)
		{
			iImage = IMAGE_MDL_GROUP;
		}
		else if (pProjectInfo->m_strProgramType.Compare("LIBRARY") == 0)
		{
			iImage = IMAGE_MDL_LIST;
		}

		HTREEITEM hRoot = GetTreeCtrl().InsertItem (pProjectInfo->m_strProjectName, iImage, iImage);
		if (pProjectInfo->m_root)
		{
			int iContentNum = pProjectInfo->m_root->arrChild.GetSize();
			if (iContentNum)
			{
				LPPROJECTITEM lpChildItem;
				for (int iIndex = 0; iIndex < iContentNum; iIndex++)
				{
					lpChildItem = (LPPROJECTITEM)pProjectInfo->m_root->arrChild.GetAt(iIndex);
					InsertProjectItem(hRoot,lpChildItem);
				}
			}
		}

			
}
void CProjectView::InsertProjectItem(HTREEITEM hTree,LPPROJECTITEM lpProjectItem)
{
	int iContentNum;
	int iImage;

	if (lpProjectItem->iItemType == PROJECT_FOLDER)
	{
			iImage = IMAGE_MDL_DIRECTORY;
	}
	if (lpProjectItem->iItemType == PROJECT_FILE)
	{
			iImage = IMAGE_MDL_SOURCE;
	}
	HTREEITEM hChild = GetTreeCtrl().InsertItem (lpProjectItem->strShowString, iImage, iImage,hTree);
	iContentNum = lpProjectItem->arrChild.GetSize();
	if (iContentNum)
	{
		LPPROJECTITEM lpChildItem;
		for (int iIndex = 0; iIndex < iContentNum; iIndex++)
		{
			lpChildItem = (LPPROJECTITEM)lpProjectItem->arrChild.GetAt(iIndex);
			InsertProjectItem(hChild,lpChildItem);
		}
	}
}

void CProjectView::OnInsertfile() 
{
	if (m_pProjectInfo== NULL)
		return ;
//	if (!DoPromptFileName(strInsert, AFX_IDS_OPENFILE,
//	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, TRUE))
//		return; // open cancelled	
	CFileDialog dlgFile(TRUE);

	CString title;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

	CString strFilter;

	// append the C,C++ files filter
	CString strCFileFilter;
	VERIFY(strCFileFilter.LoadString(IDS_CFILEFILTER));
	strFilter += strCFileFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.c;*.cpp;*.h");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	// append the Text files filter
	CString strTextFilter;
	VERIFY(strTextFilter.LoadString(IDS_TEXTFILTER));
	strFilter += strTextFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.txt");
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
//	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
//	fileName.ReleaseBuffer();
//	return nResult == IDOK;
	if (nResult == IDOK)
	{
		CString strFileName;
		POSITION pos;
//		strFilename = GetPathName() ;
		pos = dlgFile.GetStartPosition();
		while(1)
		{
			if (pos == NULL)
				break;
			strFileName = dlgFile.GetNextPathName( pos );
			InsertSpecialFile(strFileName);
		}
	}
}

/*
BOOL CProjectView::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog)
{
	CFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	if (nIDSTitle == AFX_IDS_OPENFILE)
	{
		// append the C,C++ files filter
		CString strCFileFilter;
		VERIFY(strCFileFilter.LoadString(IDS_CFILEFILTER));
		strFilter += strCFileFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.c;*.cpp;*.h");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		// append the Text files filter
		CString strTextFilter;
		VERIFY(strTextFilter.LoadString(IDS_TEXTFILTER));
		strFilter += strTextFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.txt");
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
	}

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}

*/

LPPROJECTITEM CProjectView::GetProjectItem(HTREEITEM hItem)
{
	HTREEITEM hRoot;

		hRoot = GetTreeCtrl().GetRootItem();
		if (hRoot == hItem)
		{
			return m_pProjectInfo->m_root;
		}
		CString strShowString = GetTreeCtrl().GetItemText( hItem );
		
		LPPROJECTITEM lpSearchItem = m_pProjectInfo->SearchProjectItem(m_pProjectInfo->m_root,strShowString);
		
		return lpSearchItem;
}

void CProjectView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu TailMenu;
	CMenu *popupMenu;
	CRect rectMenu(0,0,100,80);

	
	ClientToScreen(&point);
	TailMenu.LoadMenu(IDR_PROJECTPOPUP);
	popupMenu = TailMenu.GetSubMenu(0);    
	popupMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON ,point.x,point.y,((CAmboApp *)AfxGetApp())->m_pMainWnd,&rectMenu);
	
	CTreeView::OnRButtonDown(nFlags, point);
}

int CProjectView::GetFileType(CString strFileName)
{
	CString strExt;

		_splitpath(strFileName,NULL,NULL,NULL,strExt.GetBuffer(MAX_PATH));
		strExt.ReleaseBuffer();
		if ((strExt.CompareNoCase(".C") == 0)|| (strExt.CompareNoCase(".CPP") == 0))
		{
			return FILE_SOURCE;
		}
		if (strExt.CompareNoCase(".RES") == 0)
		{
			return FILE_SOURCE;
		}
		if ((strExt.CompareNoCase(".H") == 0)|| (strExt.CompareNoCase(".HPP") == 0))
		{
			return FILE_HEADER;
		}
		if (strExt.CompareNoCase(".o") == 0)
		{  // Is Obj file
			return FILE_LIBRARY;
		}
		if (strExt.CompareNoCase(".a") == 0)
		{ // it is static library
			return FILE_LIBRARY;
		}
		if (strExt.CompareNoCase(".so") == 0)
		{ // it is	dynamic link library
			return FILE_LIBRARY;
		}

		return FILE_EXTERN;
}

HTREEITEM CProjectView::GetSingleStageChild(CString strContent)
{
	// 得到一级子目录下指定字串的树
	CString strTreetext;
	HTREEITEM hRoot,hTree;
	hRoot = GetTreeCtrl().GetRootItem();
	hTree = GetTreeCtrl().GetChildItem( hRoot );  // The first child
	while(hTree)
	{
		strTreetext = GetTreeCtrl().GetItemText(hTree);
		if (strTreetext.CompareNoCase(strContent) == 0)
			return hTree;
		hTree = GetTreeCtrl().GetNextItem( hTree ,TVGN_NEXT);   // the Next child
	}
	return NULL;
}

void CProjectView::InsertSpecialFile(CString strFilename)
{
	HTREEITEM hTree;
#define PLAN_1    // 可以在任何位置插入任何文件
//#define PLAN_2  // --- 只能在指定的位置插入指定的文件

	int iType = GetFileType(strFilename);

#ifdef PLAN_1
	TVITEM tvItem;
	// 得到要插入的位置
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree == NULL || hTree == GetTreeCtrl().GetRootItem())
	{
		hTree = GetTreeCtrl().GetRootItem();
		// 如果没有光标或光标在根上，则插入到第一个子项上
		hTree = GetTreeCtrl().GetChildItem( hTree );
	}
	else
	{
		tvItem.mask = TVIF_HANDLE | TVIF_IMAGE;
		tvItem.hItem = hTree;
		GetTreeCtrl().GetItem(&tvItem);
		if (tvItem.iImage == IMAGE_MDL_SOURCE)
		{ // 如果光标在一个指定的文件上，则插在与它并列的项上
			hTree =GetTreeCtrl().GetParentItem( hTree );
		}
	}

#endif  //#ifdef PLAN_1
 
#ifdef PLAN_2
	// The Version 1 :  C, CPP Will Insert The Source Item
	//					H, HPP Will Insert the Header Item
	//					o,so,a Will Insert the library Item
	//					other will Insert the Extern Item
	hTree = GetTreeCtrl().GetRootItem();
	// 如果没有光标或光标在根上，则插入到第一个子项上
	switch(iType)
	{
		case FILE_SOURCE: // C,CPP
			hTree = GetSingleStageChild("Source");  // The Source child
			break;
		case FILE_HEADER: // H,HPP
			hTree = GetSingleStageChild("header");  // The header child
			break;
		case FILE_LIBRARY:	// o,a,so
			hTree = GetSingleStageChild("Library");  // The Library child
			break;
		case FILE_EXTERN:	// other file ,will can't insert to the project, only use by the user
			hTree = GetSingleStageChild("Extern");  // The Extern child
			break;
	}
#endif //#ifdef PLAN_2

	if (hTree == NULL)
		return;
	LPPROJECTITEM lpProjectItem;

	lpProjectItem = GetProjectItem(hTree);  // 得到跟树位置相同的工程子项

	lpProjectItem = m_pProjectInfo->InsertChild(lpProjectItem,strFilename,PROJECT_FILE);  // 将文件插到指定的项上
	if (lpProjectItem)
		InsertProjectItem(hTree,lpProjectItem); // 将子项插入到树上

	switch(iType)
	{
		case FILE_SOURCE: // C,CPP
			m_pProjectInfo->RefreshSourceFile();
			break;
		case FILE_LIBRARY:	// o,a,so
			m_pProjectInfo->RefreshLibraryFile();
			break;
	}
}

void CProjectView::OnDeleteitem() 
{
	// TODO: Add your command handler code here
	HTREEITEM hTree;
	HTREEITEM hParent;
	
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree)
	{
		hParent = GetTreeCtrl().GetParentItem(hTree);
		LPPROJECTITEM lpParentItem = GetProjectItem(hParent);
		LPPROJECTITEM lpProjectItem = GetProjectItem(hTree);

		m_pProjectInfo->DeleteSubProjectItem(lpParentItem,lpProjectItem);

		GetTreeCtrl().DeleteItem(hTree);
	}
	
}

void CProjectView::OnUpdateDeleteitem(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	HTREEITEM hTree,hRoot;
//	TVITEM tvItem;
	
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree)
	{
		hRoot = GetTreeCtrl().GetRootItem();
		if (hTree != hRoot)
		{  //选择的条目不是根，可以删除
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CProjectView::OnInsertfolder() 
{
	// TODO: Add your command handler code here
	HTREEITEM hTree;
	CString strNewFolder;

	CNewFolder dlgNewFolder;

	ReleaseCapture();  //释放Mouse

	if (dlgNewFolder.DoModal() != IDOK)
	{
		return;
	}
	strNewFolder = dlgNewFolder.m_strNewFolderText;
	if (strNewFolder.IsEmpty())
	{
		return;
	}


	TVITEM tvItem;
	// 得到要插入的位置
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree == NULL || hTree == GetTreeCtrl().GetRootItem())
	{
		// 如果没有光标或光标在根上，则插入到根项上
		hTree = GetTreeCtrl().GetRootItem();
	}
	else
	{
		tvItem.mask = TVIF_HANDLE | TVIF_IMAGE;
		tvItem.hItem = hTree;
		GetTreeCtrl().GetItem(&tvItem);
		if (tvItem.iImage == IMAGE_MDL_SOURCE)
		{ // 如果光标在一个指定的文件上，则插在与它并列的项上
			hTree =GetTreeCtrl().GetParentItem( hTree );
		}
	}

	if (hTree == NULL)
		return;
	LPPROJECTITEM lpProjectItem;

	lpProjectItem = GetProjectItem(hTree);  // 得到跟树位置相同的工程子项

	lpProjectItem = m_pProjectInfo->InsertChild(lpProjectItem,strNewFolder,PROJECT_FOLDER);  // 将文件插到指定的项上
	if (lpProjectItem)
		InsertProjectItem(hTree,lpProjectItem); // 将子项插入到树上

}

void CProjectView::OnUpdateInsertfolder(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CProjectView::OnCompile() 
{
	// TODO: Add your command handler code here
	HTREEITEM hTree;
	TVITEM tvItem;
	
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree)
	{
		tvItem.mask = TVIF_HANDLE | TVIF_IMAGE;
		tvItem.hItem = hTree;
		GetTreeCtrl().GetItem(&tvItem);
		if (tvItem.iImage == IMAGE_MDL_SOURCE)
		{
			// 我们将要去打开这个选定的文件
			CString strFileName;
//			CString strFileName = GetTreeCtrl().GetItemText( hTree );
			LPPROJECTITEM lpProjectItem = GetProjectItem(hTree);
			strFileName = lpProjectItem->strFileName;
			int iType = GetFileType(strFileName);
			if (FILE_SOURCE == iType)
			{
				CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;
				ASSERT(pMainFrame);
				pMainFrame->m_outBar.Compile(strFileName);
			}
		}
	}
	return;	
}

void CProjectView::OnUpdateCompile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	HTREEITEM hTree;
	TVITEM tvItem;
	
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree)
	{
		tvItem.mask = TVIF_HANDLE | TVIF_IMAGE;
		tvItem.hItem = hTree;
		GetTreeCtrl().GetItem(&tvItem);
		if (tvItem.iImage == IMAGE_MDL_SOURCE)
		{
			// 我们将要去打开这个选定的文件
			CString strFileName;
//			CString strFileName = GetTreeCtrl().GetItemText( hTree );
			LPPROJECTITEM lpProjectItem = GetProjectItem(hTree);
			strFileName = lpProjectItem->strFileName;
			int iType = GetFileType(strFileName);
			if (FILE_SOURCE == iType)
			{
				pCmdUI->Enable(TRUE);
				return;
			}
		}
	}
	pCmdUI->Enable(FALSE);
	return;
}

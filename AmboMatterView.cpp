// AmboMatterView.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"

//#include "ProjectDoc.h"
#include "AmboMatterView.h"
#include "WorkSpaceInfo.h"
#include "srcData.h"
#include "Infomation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmboMatterView

#define FOLDER_ERROR		0
#define FOLDER_MODULE		1
#define FOLDER_DIRECTORY	2

IMPLEMENT_DYNCREATE(CAmboMatterView, CTreeView)

CAmboMatterView::CAmboMatterView()
{
	imgList = NULL;
	m_pWorkSpaceInfo = NULL;
}

CAmboMatterView::~CAmboMatterView()
{
}


BEGIN_MESSAGE_MAP(CAmboMatterView, CTreeView)
	//{{AFX_MSG_MAP(CAmboMatterView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmboMatterView drawing

void CAmboMatterView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


/////////////////////////////////////////////////////////////////////////////
// CAmboMatterView diagnostics

#ifdef _DEBUG
void CAmboMatterView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CAmboMatterView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
/*
CProjectDoc* CAmboMatterView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectDoc)));
	return (CProjectDoc*)m_pDocument;
}
*/
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAmboMatterView message handlers

void CAmboMatterView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	if (imgList == NULL)
	{  // 创建一个ImageList
//		CBitmap *bitmap = new CBitmap;
		imgList = new CImageList;
		if (imgList == NULL)
			return;
		imgList->Create(16,16,ILC_COLOR8|ILC_MASK,16,16);
		//添加两个ICON到ImageList
		imgList->Add(AfxGetApp()->LoadIcon(IDI_ROOT));
		imgList->Add(AfxGetApp()->LoadIcon(IDI_GROUP));
		imgList->Add(AfxGetApp()->LoadIcon(IDI_LIST));
		imgList->Add(AfxGetApp()->LoadIcon(IDI_MODULE));
		imgList->Add(AfxGetApp()->LoadIcon(IDI_DIRECTORY));
		imgList->Add(AfxGetApp()->LoadIcon(IDI_SOURCE));
		//imgList->Add(AfxGetApp()->LoadIcon(IDI_ICONFILE));
		//bitmap->LoadBitmap(IDB_TABIMAGES);
/*		
		if (! imgList->Create (IDB_TABIMAGES, 16, 1, RGB(255,0,255)))
		{
		//	AfxSetResourceHandle (hOldRes);
			return;
		}
*/		
//		imgList->Add(bitmap,RGB(255,0,255));
		GetTreeCtrl().SetImageList(imgList,TVSIL_NORMAL);
	
		//AfxSetResourceHandle (hOldRes);
	}
/*
	HTREEITEM hTree = GetTreeCtrl().InsertItem (TEXT ("Text"),0,0);
	GetTreeCtrl().InsertItem (TEXT ("Text1"),1,1,hTree);
	GetTreeCtrl().InsertItem (TEXT ("Text2"),1,1,hTree);
	GetTreeCtrl().InsertItem (TEXT ("Text3"),1,1,hTree);
*/	
}

int CAmboMatterView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CAmboMatterView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_LINESATROOT | TVS_HASLINES  |TVS_HASBUTTONS;
//	cs.style |= TVS_LINESATROOT | TVS_HASLINES  |TVS_HASBUTTONS |TVS_SHOWSELALWAYS   ;
	
	return CTreeView::PreCreateWindow(cs);
}

void CAmboMatterView::OnDestroy() 
{
	if (imgList != NULL)
	{
		imgList->DeleteImageList();
		delete imgList;
	}
	CTreeView::OnDestroy();
	
}
BOOL CAmboMatterView::OpenCaretFile()
{
	HTREEITEM hTree;
	TVITEM tvItem;
	CString strFilePath;

	
	hTree = GetTreeCtrl().GetNextItem(TVI_ROOT,TVGN_CARET);
	if (hTree)
	{
		tvItem.mask = TVIF_HANDLE | TVIF_IMAGE;
		tvItem.hItem = hTree;
		GetTreeCtrl().GetItem(&tvItem);
		if (tvItem.iImage == IMAGE_MDL_SOURCE ||
			tvItem.iImage == IMAGE_MDL_LIST ||
			tvItem.iImage == IMAGE_MDL_MODULE)
		{
			GetCurFilePath(hTree,strFilePath);
			// 我们将要去打开这个选定的文件
			CString strFileName = GetTreeCtrl().GetItemText( hTree );
			
			strFilePath +=strFileName;
//			AfxGetApp()->OpenDocumentFile(strFileName);			
			AfxGetApp()->OpenDocumentFile(strFilePath);			
			return TRUE;
		}
	}
	return FALSE;
}

//#define FOLDERNAME "DIRS"
//#define SOURCENAME "SOURCES"
//#define SOURCEFILENAME "sources.mps"

#define FOLDERNAME "LISTS"
#define ROOTSNAME "ROOTS"
#define FOLDERFILENAME "LISTS"

#define SOURCENAME "LOCAL_FILES"
#define SOURCEFILENAME "MODULES"


BOOL CAmboMatterView::OpenWorkSpace(CWorkSpaceInfo *pWorkSpaceInfo)
{
	CString strTitle("WorkSpace: ");

	strTitle += pWorkSpaceInfo->m_strWorkSpaceName;

	HTREEITEM hRoot = GetTreeCtrl().InsertItem (strTitle, 0, 0);
//	LoadFolder(hRoot,pWorkSpaceInfo->m_strWorkSpaceRoot);
	m_pWorkSpaceInfo = pWorkSpaceInfo;
	LoadKingmos(hRoot);
	UpdateWindow();
	return TRUE;
}

int CAmboMatterView::LoadFolder(HTREEITEM hTreeItem,CString strPath)
{
	CsrcData srcData;
	CString strCurFilePath;
	CString strCurFolderName;
	CString strCurFileName;
	int nStart = 0, nEnd = 0;
	int iRet = FOLDER_ERROR;

	// 如果当前目录是arch目录，则我将去装载当前打开的BSP
	{
		CString strArchPath;
		CString strKingmosRoot;

		((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosRoot);

		strArchPath = strKingmosRoot + "\\arch";
		if (strArchPath == strPath)
		{
			// 当前目录是Arch目录，
			HTREEITEM hChild;
			CString strBSPFolder;

			ASSERT(m_pWorkSpaceInfo);
			strCurFilePath = strPath + "\\";
			m_pWorkSpaceInfo->GetPlatformBspName(strBSPFolder);
			strCurFolderName = strCurFilePath + strBSPFolder;
			hChild = GetTreeCtrl().InsertItem (strBSPFolder, IMAGE_MDL_DIRECTORY, IMAGE_MDL_DIRECTORY, hTreeItem);
			LoadFolder(hChild,strCurFolderName);
			return FOLDER_DIRECTORY;
		}

	}

	
	strCurFilePath = strPath + "\\";
	strCurFolderName = strCurFilePath + FOLDERFILENAME;
	strCurFileName = strCurFilePath + SOURCEFILENAME;
	
	if (srcData.Load(strCurFolderName) == TRUE)
	{
		CString strFolderList;
		CString strFolder;
		CString strNewPath;

		GetTreeCtrl().InsertItem (FOLDERFILENAME, IMAGE_MDL_LIST, IMAGE_MDL_LIST, hTreeItem);
		if (srcData.Get (FOLDERNAME, strFolderList))
		{
			HTREEITEM hChild;
				nStart = 0;
				nEnd = 0;
				iRet = FOLDER_DIRECTORY;
				do {
					nEnd = strFolderList.Find (TEXT(' '), nStart);
					if (nEnd == -1)
					{
						strFolder = strFolderList.Mid (nStart, strFolderList.GetLength()-nStart);
						if (strFolder.IsEmpty() == FALSE)
						{
							hChild = GetTreeCtrl().InsertItem (strFolder, IMAGE_MDL_GROUP, IMAGE_MDL_GROUP, hTreeItem);
							/*
							if (IsFullPath(strFolder))
							{
								strNewPath = strFolder;
							}
							else
							{
								strNewPath = strCurFilePath + strFolder;
							}
							*/
							GetFullPath(strCurFilePath , strFolder, strNewPath);
//							LoadFolder(hChild,strNewPath);
							if (LoadFolder(hChild,strNewPath) == FOLDER_DIRECTORY)
							{
								GetTreeCtrl().SetItem(hChild,TVIF_IMAGE | TVIF_SELECTEDIMAGE,NULL,IMAGE_MDL_DIRECTORY,IMAGE_MDL_DIRECTORY,0,0,0);
							}
						}
						break;
					}
					else
					{
						strFolder = strFolderList.Mid (nStart, nEnd-nStart);
						nStart = nEnd+1;
						hChild = GetTreeCtrl().InsertItem (strFolder, IMAGE_MDL_GROUP, IMAGE_MDL_GROUP, hTreeItem);
//						strNewPath = strCurFilePath + strFolder;
/*						if (IsFullPath(strFolder))
						{
							strNewPath = strFolder;
						}
						else
						{
							strNewPath = strCurFilePath + strFolder;
						}
*/
						GetFullPath(strCurFilePath , strFolder, strNewPath);
						if (LoadFolder(hChild,strNewPath) == FOLDER_DIRECTORY)
						{
							GetTreeCtrl().SetItem(hChild,TVIF_IMAGE | TVIF_SELECTEDIMAGE,NULL,IMAGE_MDL_DIRECTORY,IMAGE_MDL_DIRECTORY,0,0,0);
						}
					}
				}while (1);
		}

		if (srcData.Get (ROOTSNAME, strFolderList))
		{
			HTREEITEM hChild;
			CString strCygWinFolder;
//				GetTreeCtrl().InsertItem (FOLDERFILENAME, IMAGE_MDL_LIST, IMAGE_MDL_LIST, hTreeItem);
				iRet = FOLDER_DIRECTORY;
				nStart = 0;
				nEnd = 0;
				do {
					nEnd = strFolderList.Find (TEXT(' '), nStart);
					if (nEnd == -1)
					{
						strCygWinFolder = strFolderList.Mid (nStart, strFolderList.GetLength()-nStart);
						TransCygwinFolderToMSFolder(strCygWinFolder,strFolder);
						if (strFolder.IsEmpty() == FALSE)
						{
							hChild = GetTreeCtrl().InsertItem (strFolder, IMAGE_MDL_GROUP, IMAGE_MDL_GROUP, hTreeItem);
							/*
							if (IsFullPath(strFolder))
							{
								strNewPath = strFolder;
							}
							else
							{
								strNewPath = strCurFilePath + strFolder;
							}
							*/
							GetFullPath(strCurFilePath , strFolder, strNewPath);
//							LoadFolder(hChild,strNewPath);
							if (LoadFolder(hChild,strNewPath) == FOLDER_DIRECTORY)
							{
								GetTreeCtrl().SetItem(hChild,TVIF_IMAGE | TVIF_SELECTEDIMAGE,NULL,IMAGE_MDL_DIRECTORY,IMAGE_MDL_DIRECTORY,0,0,0);
							}
						}
						break;
					}
					else
					{
						strCygWinFolder = strFolderList.Mid (nStart, nEnd-nStart);
						TransCygwinFolderToMSFolder(strCygWinFolder,strFolder);
						nStart = nEnd+1;
						hChild = GetTreeCtrl().InsertItem (strFolder, IMAGE_MDL_GROUP, IMAGE_MDL_GROUP, hTreeItem);
//						strNewPath = strCurFilePath + strFolder;
/*						if (IsFullPath(strFolder))
						{
							strNewPath = strFolder;
						}
						else
						{
							strNewPath = strCurFilePath + strFolder;
						}
*/
						GetFullPath(strCurFilePath , strFolder, strNewPath);
						if (LoadFolder(hChild,strNewPath) == FOLDER_DIRECTORY)
						{
							GetTreeCtrl().SetItem(hChild,TVIF_IMAGE | TVIF_SELECTEDIMAGE,NULL,IMAGE_MDL_DIRECTORY,IMAGE_MDL_DIRECTORY,0,0,0);
						}
					}
				}while (1);
		}
		srcData.RemoveAll();
	}
	else if (srcData.Load(strCurFileName) == TRUE)
	{
		CString strFileList;
//		HTREEITEM hSourceFile = GetTreeCtrl().InsertItem (CString("Source Files"), IMAGE_MDL_FOLDER, IMAGE_MDL_FOLDER, hRoot);
		nStart = 0;
		nEnd = 0;
		GetTreeCtrl().InsertItem (SOURCEFILENAME, IMAGE_MDL_MODULE, IMAGE_MDL_MODULE, hTreeItem);
		iRet = FOLDER_MODULE;
		if (srcData.Get (SOURCENAME, strFileList))
		{
			CString strFile;
			do {
				nEnd = strFileList.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					strFile = strFileList.Mid (nStart, strFileList.GetLength()-nStart);
					if (strFile.IsEmpty() == FALSE)
					{
						GetTreeCtrl().InsertItem (strFile, IMAGE_MDL_SOURCE, IMAGE_MDL_SOURCE, hTreeItem);
					}
					break;
				}
				else
				{
					strFile = strFileList.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetTreeCtrl().InsertItem (strFile, IMAGE_MDL_SOURCE, IMAGE_MDL_SOURCE, hTreeItem);
				}
			}while (1);
		}
		srcData.RemoveAll();
	}
	else
	{
		CString strMsg;
		CString strInvalid;
		strInvalid.LoadString(IDS_INVALID_PATH);
		strMsg.Format( "%s \"%s\"", strInvalid, strPath );
		AfxMessageBox(strMsg);
	}

	return iRet;
}



void CAmboMatterView::GetCurFilePath(HTREEITEM hTree, CString &strFilePath)
{
	CTreeCtrl& cTreeCtrl = GetTreeCtrl();
	HTREEITEM hParent;
	HTREEITEM hRoot;
	CString strKingmosRoot;

		strFilePath = "";

		hRoot = cTreeCtrl.GetRootItem();
		while(1)
		{
			hParent = cTreeCtrl.GetParentItem(hTree);
			if (hParent == NULL)
				break;
			if (hParent == hRoot)
				break;
			CString strPathName = GetTreeCtrl().GetItemText( hParent );
//			strPathName += "\\";
//			strFilePath = strPathName + strFilePath;
			MargePath(strPathName,strFilePath,strFilePath);
			hTree = hParent; 
		}
		((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosRoot);
//		strFilePath = m_pWorkSpaceInfo->m_strWorkSpaceRoot + "\\" + strFilePath;
//		strFilePath = strKingmosRoot + "\\" + strFilePath;
		MargePath(strKingmosRoot,strFilePath,strFilePath);
//		strFilePath += "\\";
}

BOOL CAmboMatterView::HaveSelFeature()
{
	HTREEITEM hTree;

		if (::GetFocus() == GetTreeCtrl().m_hWnd)
		{
			hTree = GetTreeCtrl().GetSelectedItem();
			if (hTree)
				return TRUE;
		}
		return FALSE;
}
int CAmboMatterView::GetSelFeature(CString &strPath)
{
	HTREEITEM hTree,hRoot;
	TVITEM tvItem;

		if (::GetFocus() == GetTreeCtrl().m_hWnd)
		{
			hTree = GetTreeCtrl().GetSelectedItem();
			if (hTree)
			{
				hRoot = GetTreeCtrl().GetRootItem();
				if (hRoot != hTree)
				{
					GetCurFilePath(hTree,strPath);
					// 我们将要去打开这个选定的文件
					CString strFileName = GetTreeCtrl().GetItemText( hTree );
					strPath +=strFileName;

					tvItem.mask = TVIF_HANDLE | TVIF_IMAGE;
					tvItem.hItem = hTree;
					GetTreeCtrl().GetItem(&tvItem);
					if (tvItem.iImage == IMAGE_MDL_SOURCE)
						return FEATURE_FILE;
					else if (tvItem.iImage == IMAGE_MDL_GROUP)
						return FEATURE_MODULE;
					else if (tvItem.iImage == IMAGE_MDL_DIRECTORY)
						return FEATURE_DIR;
				}
				else
				{
//					strPath = m_pWorkSpaceInfo->m_strWorkSpaceRoot;
					((CAmboApp *)AfxGetApp())->GetKingmosRoot(strPath);
					return FEATURE_ROOT;
				}
			}
		}
		return FEATURE_INVALIDATE;
}

void CAmboMatterView::RefreshModules(LPCTSTR lpModulesName)
{
	CTreeCtrl& cTreeCtrl = GetTreeCtrl();
	HTREEITEM hTree,hChild;
	CString strFilePath;
	CString strItemName;
	CString strChild;
	int iStart,ilen,iNext;
	CString strKingmosRoot;

		((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosRoot);
		if (strKingmosRoot.IsEmpty() != 0)
			return;

		strFilePath = lpModulesName;
//		if (strFilePath.Find(m_pWorkSpaceInfo->m_strWorkSpaceRoot,0) != 0)
//			return ;
//		iStart = m_pWorkSpaceInfo->m_strWorkSpaceRoot.GetLength();
		if (strFilePath.Find(strKingmosRoot,0) != 0)
			return ;
		iStart = strKingmosRoot.GetLength();

		hTree = cTreeCtrl.GetRootItem();
		while(1)
		{
			iNext = strFilePath.Find("\\",iStart);
			if (iNext == -1)
			{
				ilen = strFilePath.GetLength() - iStart;
				break;
			}
			ilen = iNext - iStart;
			if (ilen == 0)
			{
				iStart ++;
				continue;
			}
			strItemName = strFilePath.Mid(iStart,ilen);
			iStart = iNext ;
//			iStart += ilen;
		
			hChild = cTreeCtrl.GetChildItem(hTree);
			while(1)
			{
				strChild = cTreeCtrl.GetItemText(hChild);
				if (strChild.CompareNoCase(strItemName) == 0)
				{
					// 找到了要找的子项
					hTree = hChild;
					break;
				}
				hChild = cTreeCtrl.GetNextItem(hChild,TVGN_NEXT);
				if (hChild == NULL)
				{
					// 没有找到了要找的子项
					return ;
				}
			}
		}
		strItemName = strFilePath.Mid(iStart,ilen);  // 找到最后被修改的文件
		if (strItemName.CompareNoCase("LISTS") != 0 && strItemName.CompareNoCase("MODULES") != 0)
		{
			// 不是配置文件，不需要重新刷新
			return;
		}

		strItemName = strFilePath.Left(iStart-1);
		//删除所有的子项，并重新插入
//		hChild = cTreeCtrl.GetChildItem(hTree);
		while(1)
		{
			hChild = cTreeCtrl.GetChildItem(hTree);
			if (hChild == NULL)
			{
				break;
			}
			cTreeCtrl.DeleteItem(hChild);
		}
		LoadFolder(hTree,strItemName);
}

void CAmboMatterView::LoadKingmos(HTREEITEM hRoot)
{
	CString strKingmosRoot;
	((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosRoot);
	if (strKingmosRoot.IsEmpty() != 0)
		return;
	LoadFolder(hRoot,strKingmosRoot);
}

void CAmboMatterView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu TailMenu;
	CMenu *popupMenu;
	CRect rectMenu(0,0,100,80);

//	CTreeView::OnLButtonDown(nFlags, point);
//	CTreeView::OnLButtonUp(nFlags, point);
	UINT uFlags;
	HTREEITEM hItem = GetTreeCtrl().HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		GetTreeCtrl().SelectItem(hItem);
	}
	else
	{
		GetTreeCtrl().SelectItem(NULL);
	}

	ClientToScreen(&point);
	TailMenu.LoadMenu(IDR_MATTERVIEWPOPUP);
	popupMenu = TailMenu.GetSubMenu(0);    
	popupMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON ,point.x,point.y,((CAmboApp *)AfxGetApp())->m_pMainWnd,&rectMenu);

	CTreeView::OnRButtonDown(nFlags, point);
}

BOOL CAmboMatterView::IsFullPath(CString strFolder)
{
	int nNext;

		nNext = strFolder.Find(':',0);
		if (nNext != -1)
		{
			//是全路径，直接拷贝就可以
			return TRUE;
		}
		return FALSE;
}

void CAmboMatterView::GetFullPath(CString strCurPath, CString strFolderName, CString &strFullPath)
{
	CString strCurFolder;
	int nStart,nNext,nEnd;
	if (IsFullPath(strFolderName))
	{
		strFullPath = strFolderName;
	}
	else
	{
		strFullPath = strCurPath;
		nStart = 0;
		while(1)
		{
			nNext = strFolderName.Find('\\',nStart);
			if (nNext == 0)
			{
				// Will Set To Current Drivers,root
				nEnd = strFullPath.Find('\\'); // 得到第一个'\\'的位置
				if (nEnd == -1)
					return;
				strFullPath = strFullPath.Left(nEnd); // 删掉'\'以后的字符，得到盘符。
				strFullPath +=strFolderName; 
				return;
			}
			if (nNext == -1)
			{
//				strFullPath = strFullPath + '\\';  // 将路径加到文件上
				strFullPath +=strFolderName.Mid(nStart);
				return;
			}
			strCurFolder = strFolderName.Mid(nStart,nNext - nStart);
			if (strCurFolder == "..")
			{
				// c:\temp\temp3\temp4\  --> c:\temp\temp3\  OK
				strFullPath.TrimRight('\\'); // 得到最后一个'\\'的位置
				nEnd = strFullPath.ReverseFind('\\'); // 得到最后一个'\\'的位置
				if (nEnd == -1)
					return;
				nEnd ++;
				strFullPath = strFullPath.Left(nEnd); // 得到上一级目录
				nStart = nNext +1;
				continue;
			}
			if (strCurFolder == ".")
			{
				nStart = nNext +1;
				continue;
			}
			//为当前路径下的一个文件夹，直接加到当前路径上就可以了
//			strFullPath = strFullPath + '\\';  // 将路径加到文件上
			strFullPath +=strFolderName.Mid(nStart);
			return;
		}
	}
}
void CAmboMatterView::MargePath(CString strParentPath, CString strChildPath,CString &strFullPath)
{
	CString strNewPath;
	int nStart,nNext,nEnd;
	CString strCurFolder;

	if (IsFullPath(strChildPath))
	{
		strNewPath = strChildPath;
	}
	else
	{
		strNewPath = strParentPath;
		strNewPath += '\\';
		nStart = 0;
		while(1)
		{
			nNext = strChildPath.Find('\\',nStart);
			if (nNext == 0)
			{	// 第一个字符是'\\'
				// Will Set To Current Drivers,root
				nEnd = strNewPath.Find('\\'); // 得到第一个'\\'的位置
				if (nEnd == -1)  // 没有路径，直接返回子路径
				{
					strNewPath = strChildPath;
					break;
				}
				strNewPath = strNewPath.Left(nEnd); // 删掉'\'以后的字符，得到盘符。
				strNewPath +=strChildPath; 
				break;  // 返回全路径
			}
			if (nNext == -1)
			{
//				strFullPath = strFullPath + '\\';  // 将路径加到文件上
				strNewPath +=strChildPath.Mid(nStart);
				break;
			}
			strCurFolder = strChildPath.Mid(nStart,nNext - nStart);
			if (strCurFolder == "..")
			{
				// c:\temp\temp3\temp4\  --> c:\temp\temp3\  OK
				strNewPath.TrimRight('\\'); // 得到最后一个'\\'的位置
				nEnd = strNewPath.ReverseFind('\\'); // 得到最后一个'\\'的位置
				if (nEnd == -1)
				{ // ..\temp\temp2  --> temp\temp2
					strNewPath =strChildPath.Mid(nNext + 1);
					break;
				}
				else
				{
					nEnd ++;
					strNewPath = strNewPath.Left(nEnd); // 得到上一级目录
				}
				nStart = nNext +1;
				continue;
			}
			if (strCurFolder == ".")
			{
				nStart = nNext +1;
				continue;
			}
			//为当前路径下的一个文件夹，直接加到当前路径上就可以了
//			strFullPath = strFullPath + '\\';  // 将路径加到文件上
			strNewPath +=strChildPath.Mid(nStart);
			break;
		}
	}
	strFullPath = strNewPath;
	return ;
}

void CAmboMatterView::TransCygwinFolderToMSFolder(CString &strCygWinFolder, CString &strFolder)
{
	// /cygdrive/d/work/kingmos  -->  d:\work\kingmos
	CString strCygRoot = "/cygdrive/";
	if (strCygWinFolder.Find(strCygRoot,0) == 0)
	{
		strFolder = strCygWinFolder.Mid(strCygRoot.GetLength());
		int driver = strFolder.Find('/',0);
		strFolder.Insert(driver,':');
	}
	else
	{
		//strFolder = strCygWinFolder;
		CString strMsg;
		strMsg.Format("\"%s\" Error \r\n ROOTS must be cygwin Folder",strCygWinFolder);
		AfxMessageBox (strMsg);
	}
	strFolder.Replace("/","\\");
}

void CAmboMatterView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags;
	HTREEITEM hItem = GetTreeCtrl().HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		if (OpenCaretFile() == TRUE)
			return ;
	}
	
	CTreeView::OnLButtonDblClk(nFlags, point);
}

void CAmboMatterView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags;
	HTREEITEM hItem = GetTreeCtrl().HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		GetTreeCtrl().SelectItem(hItem);
	}
	
	CTreeView::OnLButtonDown(nFlags, point);
}

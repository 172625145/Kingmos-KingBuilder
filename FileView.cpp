// FileView.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "FileView.h"
#include "srcData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

IMPLEMENT_DYNCREATE(CFileView, CTreeView)

CFileView::CFileView()
{
	m_pImgList = NULL;
}

CFileView::~CFileView()
{
}


BEGIN_MESSAGE_MAP(CFileView, CTreeView)
	//{{AFX_MSG_MAP(CFileView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileView drawing

void CFileView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CFileView diagnostics

#ifdef _DEBUG
void CFileView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CFileView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileView message handlers
//#define FOLDERNAME "DIRS"
//#define SOURCENAME "SOURCES"

#define FOLDERNAME "LISTS"
#define SOURCENAME "MODULES"

BOOL CFileView::OpenWorkSpace(CWorkSpaceInfo *pWorkSpaceInfo)
{
	int i,j;
	TCHAR pFirstChild[32];
	TCHAR pSecondChild[32];
	HTREEITEM hRoot = GetTreeCtrl().InsertItem ("Root", 0, 0);
	for (i=0;i<20;i++)
	{
		sprintf(pFirstChild,"First Child %d",i);
		HTREEITEM hFirstChild = GetTreeCtrl().InsertItem (pFirstChild, 1, 1,hRoot);
		for (j=0;j<10;j++)
		{
			sprintf(pSecondChild,"Second Child %d",i);
			GetTreeCtrl().InsertItem (pSecondChild, 2, 2,hFirstChild);
		}
	}
/*
	CString strTitle("WorkSpace: ");

	strTitle += pWorkSpaceInfo->m_strWorkSpaceName;

	HTREEITEM hRoot = GetTreeCtrl().InsertItem (strTitle, 0, 0);
	LoadFolder(hRoot,pWorkSpaceInfo->m_strWorkSpaceRoot);
*/
	return TRUE;
}

void CFileView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
	// TODO: Add your specialized code here and/or call the base class

	if (m_pImgList == NULL)
	{  // 创建一个ImageList
		m_pImgList = new CImageList;
		if (m_pImgList == NULL)
			return;
		//imgList->Create(16,16,ILC_MASK,16,16);
		//添加两个ICON到ImageList
		//imgList->Add(AfxGetApp()->LoadIcon(IDI_ICONFOLDER));
		//imgList->Add(AfxGetApp()->LoadIcon(IDI_ICONFILE));
		
		if (! m_pImgList->Create (IDB_TABIMAGES, 16, 1, RGB(255,0,255)))
		{
		//	AfxSetResourceHandle (hOldRes);
			return;
		}
		GetTreeCtrl().SetImageList(m_pImgList,TVSIL_NORMAL);
	
		//AfxSetResourceHandle (hOldRes);
	}
}
BOOL CFileView::LoadFolder(HTREEITEM hTreeItem,CString strPath)
{
	CsrcData srcData;
	CString strCurFilePath;
	CString strCurFolderName;
	CString strCurFileName;
	int nStart = 0, nEnd = 0;
	
	strCurFilePath = strPath + "\\";
	strCurFolderName = strCurFilePath + FOLDERNAME;
	strCurFileName = strCurFilePath + SOURCENAME;
	
	if (srcData.Load(strCurFolderName) == TRUE)
	{
		CString strFolderList;
		CString strFolder;
		CString strNewPath;

		if (srcData.Get (FOLDERNAME, strFolderList))
		{
			HTREEITEM hChild;
				do {
					nEnd = strFolderList.Find (TEXT(' '), nStart);
					if (nEnd == -1)
					{
						strFolder = strFolderList.Mid (nStart, strFolderList.GetLength()-nStart);
						if (strFolder.IsEmpty() == FALSE)
						{
							hChild = GetTreeCtrl().InsertItem (strFolder, IMAGE_MDL_FOLDER, IMAGE_MDL_FOLDER, hTreeItem);
							strNewPath = strCurFilePath + strFolder;
							LoadFolder(hChild,strNewPath);
						}
						break;
					}
					else
					{
						strFolder = strFolderList.Mid (nStart, nEnd-nStart);
						nStart = nEnd+1;
						hChild = GetTreeCtrl().InsertItem (strFolder, IMAGE_MDL_FOLDER, IMAGE_MDL_FOLDER, hTreeItem);
						strNewPath = strCurFilePath + strFolder;
						LoadFolder(hChild,strNewPath);
					}
				}while (1);
			}

		srcData.RemoveAll();
	}
	else if (srcData.Load(strCurFileName) == TRUE)
	{
		CString strFileList;
//		HTREEITEM hSourceFile = GetTreeCtrl().InsertItem (CString("Source Files"), IMAGE_MDL_FOLDER, IMAGE_MDL_FOLDER, hRoot);
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
						GetTreeCtrl().InsertItem (strFile, IMAGE_MDL_FILE, IMAGE_MDL_FILE, hTreeItem);
					}
					break;
				}
				else
				{
					strFile = strFileList.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetTreeCtrl().InsertItem (strFile, IMAGE_MDL_FILE, IMAGE_MDL_FILE, hTreeItem);
				}
			}while (1);
		}
		srcData.RemoveAll();
	}

	return TRUE;
}


void CFileView::OnDestroy() 
{
	if (m_pImgList != NULL)
	{
		m_pImgList->DeleteImageList();
		delete m_pImgList;
	}

	CTreeView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CFileView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	cs.style |= TVS_LINESATROOT | TVS_HASLINES  |TVS_HASBUTTONS;
	return CTreeView::PreCreateWindow(cs);
}

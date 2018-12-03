// AmboConfigView.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"

#include "ProjectDoc.h"
#include "AmboConfigView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmboConfigView

IMPLEMENT_DYNCREATE(CAmboConfigView, CTreeView)

CAmboConfigView::CAmboConfigView()
{
	m_imgList = NULL;
}

CAmboConfigView::~CAmboConfigView()
{
}


BEGIN_MESSAGE_MAP(CAmboConfigView, CTreeView)
	//{{AFX_MSG_MAP(CAmboConfigView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmboConfigView drawing
/*
void CAmboConfigView::OnDraw(CDC* pDC)
{
	CProjectDoc* pDoc = GetDocument();
	// TODO: add draw code here
}
*/
/////////////////////////////////////////////////////////////////////////////
// CAmboConfigView diagnostics

#ifdef _DEBUG
void CAmboConfigView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CAmboConfigView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
CProjectDoc* CAmboConfigView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectDoc)));
	return (CProjectDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAmboConfigView message handlers

void CAmboConfigView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
//	m_strWorkSpaceName = "";
//	m_strKingmosRoot = "";
//	m_strWorkSpaceRoot = "";
	// TODO: Add your specialized code here and/or call the base class
	if (m_imgList == NULL)
	{  // 创建一个ImageList
		m_imgList = new CImageList;
		if (m_imgList == NULL)
			return;
		//imgList->Create(16,16,ILC_MASK,16,16);
		//添加两个ICON到ImageList
		//imgList->Add(AfxGetApp()->LoadIcon(IDI_ICONFOLDER));
		//imgList->Add(AfxGetApp()->LoadIcon(IDI_ICONFILE));
		
//		if (! m_imgList->Create (IDB_TABIMAGES, 16, 1, RGB(255,0,255)))
//		{
//			return;
//		}
		m_imgList->Create(16,16,ILC_COLOR8|ILC_MASK,16,16);
		//添加两个ICON到ImageList
		m_imgList->Add(AfxGetApp()->LoadIcon(IDI_ROOT));
		m_imgList->Add(AfxGetApp()->LoadIcon(IDI_GROUP));
		m_imgList->Add(AfxGetApp()->LoadIcon(IDI_LIST));
		m_imgList->Add(AfxGetApp()->LoadIcon(IDI_MODULE));
		m_imgList->Add(AfxGetApp()->LoadIcon(IDI_DIRECTORY));
		m_imgList->Add(AfxGetApp()->LoadIcon(IDI_SOURCE));
		GetTreeCtrl().SetImageList(m_imgList,TVSIL_NORMAL);
	
	}
	
}

void CAmboConfigView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags;
	HTREEITEM hItem = GetTreeCtrl().HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		if (OpenCaretFile() == TRUE)
			return ;
	}
//	OpenCaretFile();
	CTreeView::OnLButtonDblClk(nFlags, point);
}

BOOL CAmboConfigView::ConfigWorkSpace(CWorkSpaceInfo *pWorkSpaceInfo)
{	
	CString strRoot;

//	m_strWorkSpaceName = pWorkSpaceInfo->m_strWorkSpaceName;
//	m_strWorkSpaceRoot = pWorkSpaceInfo->m_strWorkSpaceRoot;
//	((CAmboApp *)AfxGetApp())->GetKingmosRoot(m_strKingmosRoot);
	m_pWorkSpaceInfo = pWorkSpaceInfo;
	strRoot = pWorkSpaceInfo->m_strWorkSpaceName;
	strRoot += " Config";
	HTREEITEM hRoot = GetTreeCtrl().InsertItem (strRoot,IMAGE_MDL_ROOT,IMAGE_MDL_ROOT);
	HTREEITEM hTree = GetTreeCtrl().InsertItem ("Common Config",IMAGE_MDL_DIRECTORY,IMAGE_MDL_DIRECTORY,hRoot);
					  GetTreeCtrl().InsertItem ("Common.mod",IMAGE_MDL_SOURCE,IMAGE_MDL_SOURCE,hTree);
//					  GetTreeCtrl().InsertItem ("Common.dat",IMAGE_MDL_FILE,IMAGE_MDL_FILE,hTree);
//					  GetTreeCtrl().InsertItem ("Common.db",IMAGE_MDL_FILE,IMAGE_MDL_FILE,hTree);
					  GetTreeCtrl().InsertItem ("Common.reg",IMAGE_MDL_SOURCE,IMAGE_MDL_SOURCE,hTree);

//NOTE: 2003-08-27...ZB
//			  hTree = GetTreeCtrl().InsertItem ("Hardware Config",IMAGE_MDL_DIRECTORY,IMAGE_MDL_DIRECTORY,hRoot);
			  hTree = GetTreeCtrl().InsertItem ("Platform Config",IMAGE_MDL_DIRECTORY,IMAGE_MDL_DIRECTORY,hRoot);
//NOTE: 2003-08-27...ZB
					  GetTreeCtrl().InsertItem ("memory.con",IMAGE_MDL_SOURCE,IMAGE_MDL_SOURCE,hTree);
					  GetTreeCtrl().InsertItem ("Platform.mod",IMAGE_MDL_SOURCE,IMAGE_MDL_SOURCE,hTree);
					  GetTreeCtrl().InsertItem ("Platform.dat",IMAGE_MDL_SOURCE,IMAGE_MDL_SOURCE,hTree);
//					  GetTreeCtrl().InsertItem ("Platform.dat",IMAGE_MDL_FILE,IMAGE_MDL_FILE,hTree);
//					  GetTreeCtrl().InsertItem ("Platform.db",IMAGE_MDL_FILE,IMAGE_MDL_FILE,hTree);
					  GetTreeCtrl().InsertItem ("Platform.reg",IMAGE_MDL_SOURCE,IMAGE_MDL_SOURCE,hTree);


	return TRUE;
}

BOOL CAmboConfigView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	cs.style |= TVS_LINESATROOT | TVS_HASLINES  |TVS_HASBUTTONS;

	return CTreeView::PreCreateWindow(cs);
}

BOOL CAmboConfigView::OpenCaretFile()
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
		if (tvItem.iImage == IMAGE_MDL_SOURCE)
		{
//			GetCurFilePath(hTree,strFilePath);
// !!! Modified By Jami chen in 2003.12.11
//			strFilePath = m_strWorkSpaceRoot;
//			strFilePath = m_strKingmosRoot;
			// Common -- m_strKingmosRoot\config\...
			// Platform -- arch\bsp\config\...
			HTREEITEM hParent = GetTreeCtrl().GetParentItem(hTree);
			CString strParentFeature = GetTreeCtrl().GetItemText( hParent );
			CString strKingmosRoot;
		
			((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosRoot);

			if (strParentFeature == "Common Config")
			{
				strFilePath = strKingmosRoot;
			}
			else if (strParentFeature == "Platform Config")
			{
				//m_pWorkSpaceInfo
				CString strBSPPlatformName;
				strFilePath = strKingmosRoot;
				strFilePath += "\\arch\\";
				m_pWorkSpaceInfo->GetPlatformBspName(strBSPPlatformName);
				strFilePath += strBSPPlatformName;
			}
			else
			{
				return FALSE;
			}
// !!! Endif
			strFilePath += "\\config\\";
			// 我们将要去打开这个选定的文件
			CString strFileName = GetTreeCtrl().GetItemText( hTree );
			
			strFilePath += strFileName;

//			AfxGetApp()->OpenDocumentFile(strFileName);			
			AfxGetApp()->OpenDocumentFile(strFilePath);			
			return TRUE;
		}
	}
	return FALSE;
}

void CAmboConfigView::OnDestroy() 
{
	CTreeView::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_imgList != NULL)
	{
		delete m_imgList;
	}

}

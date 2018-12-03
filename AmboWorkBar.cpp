// mlWorkBar.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "AmboWorkBar.h"

#include "ProjectDoc.h"
#include "AmboMatterView.h"
#include "AmboConfigView.h"
#include "ProjectView.h"
#include "FileView.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmboWorkBar

CAmboWorkBar::CAmboWorkBar()
{
	m_pWorkSpaceInfo = NULL;
	m_pProjectInfo = NULL;
	m_uEnable = 100;
}

CAmboWorkBar::~CAmboWorkBar()
{
}


BEGIN_MESSAGE_MAP(CAmboWorkBar, CmlControlBar)
	//{{AFX_MSG_MAP(CAmboWorkBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmboWorkBar message handlers

int CAmboWorkBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CmlControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	if (! m_ViewTab.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | TCS_BOTTOM | TCS_FIXEDWIDTH | TCS_FOCUSNEVER |TCS_FORCELABELLEFT , 
		CRect(0, 0, 0, 0), this, 32))
	{
		TRACE0("Failed to create Tab view \n");
		return -1;
	}

//	m_ViewTab.SetDocument (RUNTIME_CLASS(CProjectDoc));
	
	// set "normal" GUI-font
	CFont *font = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	m_ViewTab.SetFont(font);

	ReadSeriaoNO();
	return 0;
}

void CAmboWorkBar::OnSize(UINT nType, int cx, int cy) 
{
	CmlControlBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect (&rect);
	m_ViewTab.MoveWindow (&rect,FALSE);
	//m_ViewTab.OnSize(nType,cx,cy);
}

BOOL CAmboWorkBar::OpenDocument(LPCTSTR lpstrDoc)
{
	m_ViewTab.RemoveAllView(); //首先删除原有的View

	ReadSeriaoNO();
	if (m_uEnable)
		return FALSE;

	const TCHAR *lpstrExt = _tcsrchr (lpstrDoc, TEXT('.'));

//	((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(400,10,"Enter Open Document");
	if (lpstrExt != NULL)
	{
		if (_tcscmp (lpstrExt, TEXT(".osp")) == 0)
		{
			// 是不是已经打开了一个WorkSpace
			if (m_pWorkSpaceInfo)
			{
				if (m_pWorkSpaceInfo->IsWorkSpaceOpen())
					m_pWorkSpaceInfo->CloseWorkSpace();
			}
			// 这里是处理一个Project
			if (m_pProjectInfo == NULL)
			{
				m_pProjectInfo = new CProjectInfo();
				if (m_pProjectInfo == NULL)
					return FALSE;
			}
			else
			{
				if (m_pProjectInfo->IsProjectOpen())
					m_pProjectInfo->CloseProject();
			}
			if (m_pProjectInfo->OpenProject(lpstrDoc) == FALSE)
			{
				MessageBox("打开工程失败，可能文件被破坏或者不是工程文件");
				return FALSE;
			}

			
			CProjectView *pProjectView;
			m_ViewTab.AddView (TEXT("Project"), RUNTIME_CLASS(CProjectView), 1, 1);
			m_ViewTab.SetActiveView (0);

			pProjectView = (CProjectView *)m_ViewTab.GetView(0);
			//pProjectView->OpenProject(lpstrDoc);
			pProjectView->UpdateProject(m_pProjectInfo);
		}
		else if (_tcscmp (lpstrExt, TEXT(".osw")) == 0)
		{
			// 是不是已经打开了一个Project
			if (m_pProjectInfo)
			{
				if (m_pProjectInfo->IsProjectOpen())
					m_pProjectInfo->CloseProject();
			}
			// 这里是处理一个WorkSpace
			if (m_pWorkSpaceInfo == NULL)
			{
				m_pWorkSpaceInfo = new CWorkSpaceInfo(lpstrDoc);
			}
			else
			{
				if (m_pWorkSpaceInfo->IsWorkSpaceOpen())
					m_pWorkSpaceInfo->CloseWorkSpace();
//				m_pWorkSpaceInfo->ReadInfo(lpstrDoc);
				m_pWorkSpaceInfo->OpenWorkSpace(lpstrDoc);
			}
			if (m_pWorkSpaceInfo->IsWorkSpaceOpen() == FALSE)
			{
				MessageBox("打开工程失败，可能文件被破坏或者不是工程文件");
				return FALSE;
			}

			CAmboMatterView *pMatterView;
			CAmboConfigView *pConfigView;
//			CFileView *pFileView;
			m_ViewTab.AddView (TEXT("ConfigView"), RUNTIME_CLASS(CAmboConfigView), 1, 1);
			m_ViewTab.AddView (TEXT("MatterView"), RUNTIME_CLASS(CAmboMatterView), 0, 0);
//			m_ViewTab.AddView (TEXT("FileView"), RUNTIME_CLASS(CFileView), 1, 1);
			m_ViewTab.SetActiveView (0);

//			pFileView = (CFileView *)m_ViewTab.GetView(1);
//			pFileView->OpenWorkSpace(m_pWorkSpaceInfo);

			pMatterView = (CAmboMatterView *)m_ViewTab.GetView(0);
			pMatterView->OpenWorkSpace(m_pWorkSpaceInfo);
			
			pConfigView = (CAmboConfigView *)m_ViewTab.GetView(1);
			pConfigView->ConfigWorkSpace(m_pWorkSpaceInfo);
		}
	}
	return TRUE;
}

void CAmboWorkBar::OnDestroy() 
{

	if (m_pProjectInfo)
		delete m_pProjectInfo;
	m_pProjectInfo = NULL;

	if (m_pWorkSpaceInfo)
	{
		m_pWorkSpaceInfo->CloseWorkSpace();
		delete m_pWorkSpaceInfo;
	}
	m_pWorkSpaceInfo = NULL;

	CmlControlBar::OnDestroy();
	
	// TODO: Add your message handler code here	
}

//BOOL CAmboWorkBar::CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, UINT uCPUType)
//BOOL CAmboWorkBar::CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, CString m_strBSPName)
BOOL CAmboWorkBar::CreateNewWorkSpace(CString strWorkSpaceName, CString strLocation, CString m_strBSPName, CString strPlatformRoot)
{
//	m_pWorkSpaceInfo = new CWorkSpaceInfo(strWorkSpaceName,strLocation,uCPUType);
	ReadSeriaoNO();
	if (m_uEnable)
		return FALSE;

	if (m_pWorkSpaceInfo)
	{
		m_pWorkSpaceInfo->CloseWorkSpace();
		delete m_pWorkSpaceInfo;
	}
//	m_pWorkSpaceInfo = new CWorkSpaceInfo(strWorkSpaceName,strLocation,m_strBSPName);
	m_pWorkSpaceInfo = new CWorkSpaceInfo(strWorkSpaceName,strLocation,m_strBSPName,strPlatformRoot);
//	m_pWorkSpaceInfo->WriteInfo();
	m_pWorkSpaceInfo->SaveWorkSpace();
	OpenDocument(m_pWorkSpaceInfo->m_strFileName.LockBuffer());
	m_pWorkSpaceInfo->m_strFileName.UnlockBuffer();
	return TRUE;
}


BOOL CAmboWorkBar::IsWorkSpaceOpen()
{
	if (m_uEnable)
		return FALSE;
	if (m_pProjectInfo)
	{
		if (m_pProjectInfo->IsProjectOpen())  // 已经打开了一个Project
			return TRUE;
	}
	if (m_pWorkSpaceInfo)
	{
		if (m_pWorkSpaceInfo->IsWorkSpaceOpen()) //已经打开了一个WorkSpace
			return TRUE;
	}
	return FALSE;
}

BOOL CAmboWorkBar::CanCompileFeature()
{
	if (m_uEnable)
		return FALSE;
	if (m_pWorkSpaceInfo)
	{
		if (m_pWorkSpaceInfo->IsWorkSpaceOpen()) //已经打开了一个WorkSpace
		{
			if (m_ViewTab.GetCurSel() != 0)
				return FALSE;

			CAmboMatterView *pMatterView;
			pMatterView = (CAmboMatterView *)m_ViewTab.GetView(0);
			return pMatterView->HaveSelFeature();
		}
	}
	return FALSE;
}

int CAmboWorkBar::GetSelFeature(CString &strPath)
{
	if (m_pWorkSpaceInfo)
	{
		if (m_pWorkSpaceInfo->IsWorkSpaceOpen()) //已经打开了一个WorkSpace
		{
			if (m_ViewTab.GetCurSel() != 0)
				return FALSE;

			CAmboMatterView *pMatterView;
			pMatterView = (CAmboMatterView *)m_ViewTab.GetView(0);
			return pMatterView->GetSelFeature(strPath);
		}
	}
	return 0;
}

void CAmboWorkBar::RefreshModules(LPCTSTR lpModulesName)
{
	if (m_pWorkSpaceInfo)
	{
		if (m_pWorkSpaceInfo->IsWorkSpaceOpen()) //已经打开了一个WorkSpace
		{
			CAmboMatterView *pMatterView;
			pMatterView = (CAmboMatterView *)m_ViewTab.GetView(0);
			
			pMatterView->RefreshModules(lpModulesName);
		}
	}
	if (m_pProjectInfo)
	{
//		if (m_pProjectInfo->IsProjectOpen()) //已经打开了一个WorkSpace
//			m_pProjectInfo->CloseProject();
	}
}

void CAmboWorkBar::CloseWorkSpace()
{
	if (m_pWorkSpaceInfo)
	{
		if (m_pWorkSpaceInfo->IsWorkSpaceOpen()) //已经打开了一个WorkSpace
			m_pWorkSpaceInfo->CloseWorkSpace();
	}
	if (m_pProjectInfo)
	{
		if (m_pProjectInfo->IsProjectOpen()) //已经打开了一个Project
			m_pProjectInfo->CloseProject();
	}
	m_ViewTab.RemoveAllView(); //首先删除原有的View
}

BOOL CAmboWorkBar::CreateNewProject(CString strProjectName, CString strProjectType,CString strLocation, CString strCPUType)
{
	if (m_uEnable)
		return FALSE;
	if (m_pProjectInfo == NULL)
	{
		m_pProjectInfo = new CProjectInfo();
		if (m_pProjectInfo == NULL)
			return FALSE;
	}
	else
	{
		if (m_pProjectInfo->IsProjectOpen()) //已经打开了一个Project
			m_pProjectInfo->CloseProject();
	}
	CString strFileName;
	m_pProjectInfo->Create(strProjectName,strProjectType,strLocation,strCPUType);
	m_pProjectInfo->SaveProject();  // 保存新创建的工程
	strFileName =m_pProjectInfo->m_strFileName; // 得到工程的文件名
	OpenDocument((LPCTSTR)strFileName);  // 打开工程文件
	return TRUE;
}

BOOL CAmboWorkBar::CreateNewProject(CString strModulesName, CString strCPUType)
{
	ReadSeriaoNO();
	if (m_uEnable)
		return FALSE;
	if (m_uEnable)
		return FALSE;
	if (m_pProjectInfo == NULL)
	{
		m_pProjectInfo = new CProjectInfo();
		if (m_pProjectInfo == NULL)
			return FALSE;
	}
	else
	{
		if (m_pProjectInfo->IsProjectOpen()) //已经打开了一个Project
			m_pProjectInfo->CloseProject();
	}
	CString strFileName;
	m_pProjectInfo->Create(strModulesName,strCPUType);
	m_pProjectInfo->SaveProject();  // 保存新创建的工程
	strFileName =m_pProjectInfo->m_strFileName; // 得到工程的文件名
	OpenDocument((LPCTSTR)strFileName);  // 打开工程文件
	return TRUE;
}


void CAmboWorkBar::ReadSeriaoNO()
{
	CRegKey reg;
	LPCTSTR lpKey;
	CString strKey;
	DWORD dwCount = 32;
	
	LONG lKey = reg.Open(HKEY_LOCAL_MACHINE, TEXT("Software\\Kingmos\\kingbuilder\\1.00.000"));
	if (lKey != ERROR_SUCCESS)
		return ;

	reg.QueryValue(strKey.GetBuffer(32),TEXT("KeyNO"),&dwCount);
	reg.Close();
	strKey.ReleaseBuffer();
	lpKey = (LPCTSTR)strKey;
	m_uEnable = OBJECTNUM;
	while(*lpKey)
	{
		m_uEnable -= *lpKey++;
	}

}

void CAmboWorkBar::OnInsertfile()
{
	if (m_uEnable)
		return ;
	if (m_pProjectInfo)
	{
		if (m_pProjectInfo->IsProjectOpen()) //已经打开了一个WorkSpace
		{
			CProjectView *pProjectView;
			pProjectView = (CProjectView *)m_ViewTab.GetView(0);
			pProjectView->OnInsertfile();
		}
	}
}

BOOL CAmboWorkBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CmlControlBar::PreCreateWindow(cs);
}

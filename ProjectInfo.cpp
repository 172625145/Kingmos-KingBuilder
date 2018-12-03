// ProjectInfo.cpp: implementation of the CProjectInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "ProjectInfo.h"
#include "MainFrame.h"
#include "AmboDoc.h"
#include "AmboView.h"
#include "AmboFrame.h"
#include "SpecialView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INT_VALUE		0
#define STR_VALUE		1

#define ERROR_VALUE		-1

typedef struct 
{
	CString strVariable;
	CString strValue;
	int iValue;
	int iValidateItem;  // The strValue or iValue is Validate
}OPTIONVARIABLE,*LPOPTIONVARIABLE;


#define FILE_SOURCE		2
#define FILE_HEADER		3
#define FILE_OBJECT		4
#define FILE_LIBRARY	5
#define FILE_DLL		6
#define FILE_EXTERN		7

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectInfo::CProjectInfo()
{
	m_strProjectName = "";
	m_strProjectRoot = "";
	m_strFileName = "";
	m_bOpen = FALSE;
	m_root = NULL;
//	m_iProgramType = PROGRAM_EXE;
	m_strProgramType = "PROGRAM";
	((CAmboApp *)AfxGetApp())->SetProjectPath(m_strProjectRoot);
}

CProjectInfo::CProjectInfo(CString strFileName)
{
	m_strProjectName = "";
	m_strProjectRoot = "";
	m_strFileName = "";
	m_bOpen = FALSE;
//	m_root = NULL;
	m_root = new PROJECTITEM;
	if (m_root == NULL)
		return ;

	m_root->iItemType = PROJECT_FOLDER;
	m_root->strShowString = "Root";
	ReadInfo(strFileName);
}

CProjectInfo::~CProjectInfo()
{
//	CloseProject();
	m_strProjectName = "";
	m_strProjectRoot = "";
	m_strFileName = "";
	m_bOpen = FALSE;
	if (m_root)
		delete m_root;
	m_root = NULL;
	ReleaseOption();
}

BOOL CProjectInfo::Create(CString strProjectName, CString strProjectType,CString strLocation, CString strCPUType)
{
	m_strProjectName = strProjectName;
	m_strProjectRoot = strLocation;
//	m_iProgramType = strProjectType;
	m_strProgramType = strProjectType;
	m_strFileName = "";
	m_bOpen = TRUE;
	if (m_root == NULL)
	{
		m_root = new PROJECTITEM;
		if (m_root == NULL)
			return FALSE;
	}
	m_root->iItemType = PROJECT_FOLDER;
	m_root->strShowString = "Root";
	m_strCPUType = strCPUType;

	LoadDefaultProjectInfo();
//	SetDefaultOptionToBuild();
//	GetCurrentOptionFromBuild();

	((CAmboApp *)AfxGetApp())->SetProjectPath(m_strProjectRoot);
	SetProjectInfoToBuild();
	RefreshSourceFile();
	RefreshLibraryFile();
	return TRUE;
}
BOOL CProjectInfo::Create(CString strModulesName, CString strCPUType)
{
	//从一个KINGMOS的MODULES转化为一个工程文件
	// First Get Project Root
	CString strLocation;
	GetModuleDir(strModulesName,strLocation);
	m_strProjectRoot = strLocation;
	m_strFileName = "";
	m_bOpen = TRUE;
	if (m_root == NULL)
	{
		m_root = new PROJECTITEM;
		if (m_root == NULL)
			return FALSE;
	}
	m_root->iItemType = PROJECT_FOLDER;
	m_root->strShowString = "Root";
	m_strCPUType = strCPUType;

	ReadModule(strModulesName);
	AnalysisModule();

	SetDefaultOptionToBuild();
	GetCurrentOptionFromBuild();

	((CAmboApp *)AfxGetApp())->SetProjectPath(m_strProjectRoot);
	SetProjectInfoToBuild();
	RefreshSourceFile();
	RefreshLibraryFile();

	//读取模块完成
	m_srcData.RemoveAll();

	return TRUE;
}

BOOL CProjectInfo::WriteInfo()
{
	CFile fileProject;
	if (m_strFileName.IsEmpty() == TRUE)
	{
		m_strFileName +=m_strProjectRoot;
		m_strFileName +="\\";
		m_strFileName +=m_strProjectName;
		m_strFileName +=".osp";
	}

	if (fileProject.Open(m_strFileName,CFile::modeCreate|CFile::modeWrite,NULL) == FALSE)
	{
		return FALSE;
	}
	fileProject.Write("OSP",3);
	// 写入CPU Type
	int iLen = m_strCPUType.GetLength() +1;  //包含结束符
	fileProject.Write(&iLen,sizeof(int));
	fileProject.Write((LPCTSTR)m_strCPUType,iLen);

	// 写入Program Type
	iLen = m_strProgramType.GetLength() +1;  //包含结束符
	fileProject.Write(&iLen,sizeof(int));
	fileProject.Write((LPCTSTR)m_strProgramType,iLen);
	
	WriteProjectItem(&fileProject,m_root);
/*	LPPROJECTITEM lpProjectItem;
	int iChildNum;
	if (m_root)
	{
		iChildNum = m_root->arrChild.GetSize();
		fileProject.Write(&iChildNum,sizeof(int));
		for (int iIndex = 0; iIndex < iChildNum; iIndex++)
		{
			lpProjectItem = (LPPROJECTITEM)m_root->arrChild.GetAt(iIndex);
			WriteProjectItem(&fileProject,lpProjectItem);
		}
	}
	*/
	WriteOption(&fileProject);

	WriteCStringArray(&fileProject,m_arrIncludeDirList);
	WriteCStringArray(&fileProject,m_arrLibraryDirList);

	//保存工程已经打开的文件
	SaveOpenedFile(&fileProject);
	fileProject.Close();

	return TRUE;
}

BOOL CProjectInfo::ReadInfo(CString strFileName)
{
	CFile fileProject;

	if (fileProject.Open(strFileName,CFile::modeRead,NULL) == FALSE)
	{
		return FALSE;
	}

	m_strProjectName = fileProject.GetFileTitle();
	m_strProjectRoot = fileProject.GetFilePath();

	//得到工程名称
	int nStart = 0, nNewPos;
	while(1)
	{
		nNewPos = m_strProjectName.Find( '.', nStart );
		if (nNewPos == -1)
			break;
		nStart = nNewPos + 1;
	}
	if (nStart != 0)
	{
		m_strProjectName.Delete(nStart-1,(m_strProjectName.GetLength() - nStart+1));
	}

	//得到工程目录
	nStart = 0;
	while(1)
	{
		nNewPos = m_strProjectRoot.Find( '\\', nStart );
		if (nNewPos == -1)
			break;
		nStart = nNewPos + 1;
	}
	if (nStart != 0)
	{
		m_strProjectRoot.Delete(nStart-1,(m_strProjectRoot.GetLength() - nStart+1));
	}

	m_strFileName = strFileName;

	CString strLog;
	fileProject.Read(strLog.GetBuffer(4),3);
	strLog.ReleaseBuffer();
	strLog.SetAt(3,0); 
	if (strLog.Compare("OSP") != 0)
	{
	    fileProject.Close();
		m_strProjectName = "";
		m_strProjectRoot = "";
		m_strFileName = "";
		m_bOpen = FALSE;
		return FALSE;
	}

	// 读取CPU Type
	int iLen;
	fileProject.Read(&iLen,sizeof(int));
	fileProject.Read(m_strCPUType.GetBuffer(iLen),iLen);
	m_strCPUType.ReleaseBuffer();


	// 读取Program Type
	fileProject.Read(&iLen,sizeof(int));
	fileProject.Read(m_strProgramType.GetBuffer(iLen),iLen);
	m_strProgramType.ReleaseBuffer();

	if (m_root)
	{
		ReadProjectItem(&fileProject,m_root,NULL);
	}
	/*
	LPPROJECTITEM lpProjectItem;
	int iChildNum = 3;
	fileProject.Read(&iChildNum,sizeof(int));
	if (m_root)
	{
		for (int iIndex = 0; iIndex < iChildNum; iIndex++)
		{
			lpProjectItem = new PROJECTITEM;
			if (lpProjectItem == NULL)
				return FALSE;
//			m_root->arrChild.Add(lpProjectItem);
			ReadProjectItem(&fileProject,lpProjectItem,m_root);
		}
	}
 */   
	ReadOption(&fileProject);

	// 读取要包含的目录
	ReadCStringArray(&fileProject,m_arrIncludeDirList);
	ReadCStringArray(&fileProject,m_arrLibraryDirList);

	// 打开工程已经打开的文件
	ReOpenOpenedFile(&fileProject);

	fileProject.Close();
	m_bOpen = TRUE;
	((CAmboApp *)AfxGetApp())->SetProjectPath(m_strProjectRoot);

	SetProjectInfoToBuild();
	RefreshSourceFile();
// **************************************************
	return TRUE;
}


BOOL CProjectInfo::IsProjectOpen()
{
	return m_bOpen;
}
BOOL CProjectInfo::OpenProject(CString strFileName)
{
	m_strProjectName = "";
	m_strProjectRoot = "";
	m_strFileName = "";
	m_bOpen = FALSE;
	if (m_root == NULL)
	{
		m_root = new PROJECTITEM;
		if (m_root == NULL)
			return FALSE;

		m_root->iItemType = PROJECT_FOLDER;
		m_root->strShowString = "Root";
	}
	return ReadInfo(strFileName);
}
BOOL CProjectInfo::SaveProject()
{
	if (m_bOpen == FALSE)
		return FALSE;
	WriteInfo();
	return TRUE;
}
BOOL CProjectInfo::CloseProject()
{

	SaveProject();

	CloseAllWindow();
	int iChildNum;
	LPPROJECTITEM lpProjectItem;
	if (m_root)
	{
		iChildNum = m_root->arrChild.GetSize();
		for (int iIndex = 0; iIndex < iChildNum; iIndex++)
		{
			lpProjectItem = (LPPROJECTITEM)m_root->arrChild.GetAt(iIndex);
			DeleteProjectItem(lpProjectItem);			
		}
		m_root->arrChild.RemoveAll();
	}

	m_arrIncludeDirList.RemoveAll(); //清除数组中的内容
	m_arrLibraryDirList.RemoveAll(); //清除数组中的内容

	m_arrSourceFileList.RemoveAll(); //清除数组中的内容

	m_arrLibFileList.RemoveAll(); //清除数组中的内容
	m_arrDllFileList.RemoveAll(); //清除数组中的内容
	m_arrObjFileList.RemoveAll(); //清除数组中的内容

	m_strProjectName = "";
	m_strProjectRoot = "";
	m_strFileName = "";
	m_bOpen = FALSE;
	return TRUE;
}
BOOL CProjectInfo::WriteProjectItem(CFile *pFile,LPPROJECTITEM lpProjectItem)
{
	int iContentNum;

	// 1 -- 存入条目类型
	pFile->Write(&lpProjectItem->iItemType,sizeof(int));
	// 2 -- 存入条目内容
	if (lpProjectItem->iItemType == PROJECT_FOLDER)
	{
		iContentNum = lpProjectItem->strShowString.GetLength() +1; // include the Termination 
		pFile->Write(&iContentNum,sizeof(int));
		pFile->Write((LPCTSTR)lpProjectItem->strShowString,iContentNum);
	}
	if (lpProjectItem->iItemType == PROJECT_FILE)
	{
		iContentNum = lpProjectItem->strFileName.GetLength() +1; // include the Termination 
		pFile->Write(&iContentNum,sizeof(int));
		pFile->Write((LPCTSTR)lpProjectItem->strFileName,iContentNum);
	}
	// 3 -- 存入子条目数目
	iContentNum = lpProjectItem->arrChild.GetSize();
	pFile->Write(&iContentNum,sizeof(int));
	// 4 -- 存入子条目
	if (iContentNum)
	{
		LPPROJECTITEM lpChildItem;
		for (int iIndex = 0; iIndex < iContentNum; iIndex++)
		{
			lpChildItem = (LPPROJECTITEM)lpProjectItem->arrChild.GetAt(iIndex);
			WriteProjectItem(pFile,lpChildItem);
		}
	}
	return TRUE;
}

BOOL CProjectInfo::ReadProjectItem(CFile *pFile,LPPROJECTITEM lpProjectItem,LPPROJECTITEM lpParent)
{
	int iContentNum;

	// 1 -- 读取条目类型
	pFile->Read(&lpProjectItem->iItemType,sizeof(int));
	// 2 -- 读取条目内容
	pFile->Read(&iContentNum,sizeof(int));
	if (lpProjectItem->iItemType == PROJECT_FOLDER)
	{
		pFile->Read((void *)(LPCTSTR)lpProjectItem->strShowString.GetBuffer(iContentNum),iContentNum);
		lpProjectItem->strShowString.ReleaseBuffer();
	}
	if (lpProjectItem->iItemType == PROJECT_FILE)
	{
		pFile->Read((void *)(LPCTSTR)lpProjectItem->strFileName.GetBuffer(iContentNum),iContentNum);
		lpProjectItem->strFileName.ReleaseBuffer();
		InsertCurFileToSpecialFileList(lpProjectItem->strFileName);
		GetShowString(lpProjectItem);
	}
	//成功读取该条目，加入到列表中
	if (lpParent)
	{
		lpParent->arrChild.Add(lpProjectItem);
	}
	// 3 -- 读取子条目数目
	pFile->Read(&iContentNum,sizeof(int));
	// 4 -- 读取子条目
	if (iContentNum)
	{
		LPPROJECTITEM lpChildItem;
		for (int iIndex = 0; iIndex < iContentNum; iIndex++)
		{
			lpChildItem = new PROJECTITEM;
			if (lpChildItem == NULL)
				break;
//			lpProjectItem->arrChild.Add(lpChildItem);
			ReadProjectItem(pFile,lpChildItem,lpProjectItem);
		}
	}
	return TRUE;
}

LPPROJECTITEM CProjectInfo::InsertChild(LPPROJECTITEM lpProjectItem,CString strChild,int iItemType)
{
	ASSERT(lpProjectItem);
	
	LPPROJECTITEM lpNewItem;

		lpNewItem = new PROJECTITEM;
		if (lpNewItem == NULL)
			return NULL;
		
		lpNewItem->iItemType = iItemType;
		if (iItemType == PROJECT_FOLDER)
		{
			lpNewItem->strShowString = strChild;
		}
		if (iItemType == PROJECT_FILE)
		{
			if (FileNameIsExist(lpProjectItem,strChild))
			{
				//该文件已经存在于该工程中
				delete lpNewItem;
				((CAmboApp *)AfxGetApp())->m_pMainWnd->MessageBox("该文件已经存在于该工程中");
				return NULL;
			}
			InsertCurFileToSpecialFileList(strChild);
			lpNewItem->strFileName = strChild;
			GetShowString(lpNewItem);
		}
		lpProjectItem->arrChild.Add(lpNewItem);
		return lpNewItem;
}
void CProjectInfo::DeleteProjectItem(LPPROJECTITEM lpProjectItem)
{
	int iContentNum;

	ASSERT(lpProjectItem);
	iContentNum = lpProjectItem->arrChild.GetSize();
	if (iContentNum)
	{
		LPPROJECTITEM lpChildItem;
		for (int iIndex = 0; iIndex < iContentNum; iIndex++)
		{
			lpChildItem = (LPPROJECTITEM)lpProjectItem->arrChild.GetAt(iIndex);
			DeleteProjectItem(lpChildItem);
		}
		lpProjectItem->arrChild.RemoveAll();
	}
	if (lpProjectItem->iItemType == PROJECT_FILE)
	{
		DeleteCurFileFromSpecialFileList(lpProjectItem->strFileName);
	}
	delete lpProjectItem;
}

void CProjectInfo::GetShowString(LPPROJECTITEM lpProjectItem)
{
	int iStart = -1,iNext = 0;
	CString strShowString;

	if (lpProjectItem->iItemType == PROJECT_FILE)
	{
		while(1)
		{
			iStart = iNext;
			iNext = lpProjectItem->strFileName.Find('\\',iStart);
			if (iNext == -1)
				break;
			iNext ++; // jump the Char '\\'
		}
		strShowString = lpProjectItem->strFileName.Mid(iStart);
		lpProjectItem->strShowString = strShowString;
		int iIndex = 1;
		while(1)
		{
			if (SearchProjectItem(m_root,lpProjectItem->strShowString) == NULL)
				break;
			// 该显示字串已经存在，使用另一个字串
			lpProjectItem->strShowString.Format("%s %02d",strShowString,iIndex);
			iIndex ++;
		}
	}
}

LPPROJECTITEM CProjectInfo::SearchProjectItem(LPPROJECTITEM lpProjectItem,CString strShowString)
{
		int iContentNum = lpProjectItem->arrChild.GetSize();
		if (iContentNum)
		{
			LPPROJECTITEM lpChildItem;
			LPPROJECTITEM lpSearchItem;
			for (int iIndex = 0; iIndex < iContentNum; iIndex++)
			{
				lpChildItem = (LPPROJECTITEM)lpProjectItem->arrChild.GetAt(iIndex);
				if (strShowString.Compare(lpChildItem->strShowString) ==0)
					return lpChildItem;
				lpSearchItem = SearchProjectItem(lpChildItem,strShowString);
				if (lpSearchItem  != NULL)
				{
					return lpSearchItem;
				}
			}
		}
		return NULL;
}

LPPROJECTITEM CProjectInfo::FileNameIsExist(LPPROJECTITEM lpProjectItem,CString strFileName)
{
		int iContentNum = lpProjectItem->arrChild.GetSize();
		if (iContentNum)
		{
			LPPROJECTITEM lpChildItem;
			LPPROJECTITEM lpSearchItem;
			for (int iIndex = 0; iIndex < iContentNum; iIndex++)
			{
				lpChildItem = (LPPROJECTITEM)lpProjectItem->arrChild.GetAt(iIndex);
				if (lpChildItem->iItemType == PROJECT_FILE)
				{
					if (strFileName.Compare(lpChildItem->strFileName) ==0)
						return lpChildItem;
				}
				else
				{
					lpSearchItem = FileNameIsExist(lpChildItem,strFileName);
					if (lpSearchItem  != NULL)
					{
						return lpSearchItem;
					}
				}
			}
		}
		return NULL;
}

void CProjectInfo::SetOption(CString strVariable, CString strValue)
{
	int iSize;
	int nIndex;
	LPOPTIONVARIABLE lpCurOption;

	iSize = arrOptionVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurOption = (LPOPTIONVARIABLE)arrOptionVar.GetAt(nIndex);
		if (lpCurOption)
		{
			if (lpCurOption->strVariable.CompareNoCase(strVariable) == 0)
			{//该选项变量已经存在，设置新的值
				lpCurOption->strValue = strValue;
				lpCurOption->iValidateItem = STR_VALUE;
				return ;
			}
		}
		nIndex ++;
	}
	//该选项变量不存在，添加新的选项变量
	lpCurOption = new OPTIONVARIABLE;
	if (lpCurOption)
	{
		lpCurOption->strVariable = strVariable;
		lpCurOption->strValue = strValue;
		lpCurOption->iValidateItem = STR_VALUE;
		arrOptionVar.Add(lpCurOption);
	}
}

void CProjectInfo::SetOption(CString strVariable, int iValue)
{
	int iSize;
	int nIndex;
	LPOPTIONVARIABLE lpCurOption;

	iSize = arrOptionVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurOption = (LPOPTIONVARIABLE)arrOptionVar.GetAt(nIndex);
		if (lpCurOption)
		{
			if (lpCurOption->strVariable.CompareNoCase(strVariable) == 0)
			{//该选项变量已经存在，设置新的值
				lpCurOption->iValue = iValue;
				lpCurOption->iValidateItem = INT_VALUE;
				return ;
			}
		}
		nIndex ++;
	}
	//该选项变量不存在，添加新的选项变量
	lpCurOption = new OPTIONVARIABLE;
	if (lpCurOption)
	{
		lpCurOption->strVariable = strVariable;
		lpCurOption->iValue = iValue;
		lpCurOption->iValidateItem = INT_VALUE;
		arrOptionVar.Add(lpCurOption);
	}
}

void CProjectInfo::GetOption(CString strVariable, CString &strValue)
{
	int iSize;
	int nIndex;
	LPOPTIONVARIABLE lpCurOption;

	strValue = "";
	iSize = arrOptionVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurOption = (LPOPTIONVARIABLE)arrOptionVar.GetAt(nIndex);
		if (lpCurOption)
		{
			if (lpCurOption->strVariable.CompareNoCase(strVariable) == 0)
			{//该选项变量已经找到，得到新的值
				if (lpCurOption->iValidateItem == STR_VALUE)
				{
					strValue = lpCurOption->strValue ;
				}
				else
				{
					// 该变量不含有一个CString的值
					strValue = "";
				}
				return ;
			}
		}
		nIndex ++;
	}
}

void CProjectInfo::GetOption(CString strVariable, int &iValue)
{
	int iSize;
	int nIndex;
	LPOPTIONVARIABLE lpCurOption;

	iValue = ERROR_VALUE;
	iSize = arrOptionVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurOption = (LPOPTIONVARIABLE)arrOptionVar.GetAt(nIndex);
		if (lpCurOption)
		{
			if (lpCurOption->strVariable.CompareNoCase(strVariable) == 0)
			{//该选项变量已经找到，得到新的值
				if (lpCurOption->iValidateItem == INT_VALUE)
				{
					iValue = lpCurOption->iValue ;
				}
				else
				{
					// 该变量不含有一个INT的值
					iValue = ERROR_VALUE;
				}
				return ;
			}
		}
		nIndex ++;
	}
}

void CProjectInfo::ReleaseOption()
{
	int iSize, nIndex;
	LPOPTIONVARIABLE lpCurOption;
	iSize = arrOptionVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurOption = (LPOPTIONVARIABLE)arrOptionVar.GetAt(nIndex);
		if (lpCurOption)
		{
			delete lpCurOption;
		}
		nIndex ++;
	}
	arrOptionVar.RemoveAll();
}

void CProjectInfo::ReadOption(CFile *pFile)
{
	int iSize;
	int nIndex,iLen,iValidateItem,iValue;
	CString strVariable,strValue;

	pFile->Read(&iSize,sizeof(int));
	nIndex = 0;
	while(nIndex < iSize)
	{
		// 读取变量名
		pFile->Read(&iLen,sizeof(int));
		pFile->Read(strVariable.GetBuffer(iLen),iLen);
		strVariable.ReleaseBuffer();
		
		// 读取变量值
		
		// 读取变量类型
		pFile->Read(&iValidateItem,sizeof(int));
		
		if (iValidateItem == STR_VALUE)
		{
			pFile->Read(&iLen,sizeof(int));
			pFile->Read(strValue.GetBuffer(iLen),iLen);
			strValue.ReleaseBuffer();
			SetOption(strVariable, strValue);
		}
		else
		{
			pFile->Read(&iValue,sizeof(int));
			SetOption(strVariable, iValue);
		}
		nIndex ++;
	}
}

void CProjectInfo::WriteOption(CFile *pFile)
{
	int iSize;
	int nIndex,iLen;
	LPOPTIONVARIABLE lpCurOption;

	iSize = arrOptionVar.GetSize();
	// 写入变量树目
	pFile->Write(&iSize,sizeof(int));
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurOption = (LPOPTIONVARIABLE)arrOptionVar.GetAt(nIndex);
		if (lpCurOption)
		{	// 写入变量名
			iLen = lpCurOption->strVariable.GetLength() +1;  //包含结束符
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)lpCurOption->strVariable,iLen);
			
			// 写入变量值

			// 写入变量类型 -- 字串 或者 数值
			pFile->Write(&lpCurOption->iValidateItem,sizeof(int));
			
			if (lpCurOption->iValidateItem == STR_VALUE)
			{//写入字串
				iLen = lpCurOption->strValue.GetLength() +1;  //包含结束符
				pFile->Write(&iLen,sizeof(int));
				pFile->Write((LPCTSTR)lpCurOption->strValue,iLen);
			}
			else
			{ // 写入数值
				pFile->Write(&lpCurOption->iValue,sizeof(int));
			}
		}
		nIndex ++;
	}
}

void CProjectInfo::WriteCStringArray(CFile *pFile, CStringArray &arrString)
{
	int nIndex,iLen;
	int iSize = arrString.GetSize();
	CString strString;
	// 写入变量树目
	pFile->Write(&iSize,sizeof(int));
	nIndex = 0;
	while(nIndex < iSize)
	{
		strString = arrString.GetAt(nIndex);
		// 写入内容
		iLen = strString.GetLength() +1;  //包含结束符
		pFile->Write(&iLen,sizeof(int));
		pFile->Write((LPCTSTR)strString,iLen);
			
		nIndex ++;
	}
}

void CProjectInfo::ReadCStringArray(CFile *pFile, CStringArray &arrString)
{
	int iSize;
	int nIndex,iLen;
	CString strString;

	arrString.RemoveAll();

	pFile->Read(&iSize,sizeof(int));
	nIndex = 0;
	while(nIndex < iSize)
	{
		// 读取变量名
		pFile->Read(&iLen,sizeof(int));
		pFile->Read(strString.GetBuffer(iLen),iLen);
		strString.ReleaseBuffer();
		arrString.Add(strString);
		nIndex ++;
	}
}
void CProjectInfo::SetProjectInfoToBuild()
{
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
		CString strCPUType,strBSPName;

			pMainFrame->m_outBar.m_BuildClass.SetBuildForDefault(); // Load Default Setting
			pMainFrame->m_outBar.m_BuildClass.SetWorkType(FALSE);  //设置不是一个Platform

			pMainFrame->m_outBar.m_BuildClass.SetProjectName(m_strProjectName);					//project name
			pMainFrame->m_outBar.m_BuildClass.SetProjectLocation(m_strProjectRoot);
			pMainFrame->m_outBar.m_BuildClass.SetProjectCpu(m_strCPUType);

/*			switch(m_iProgramType)
			{
				case PROGRAM_EXE:
					pMainFrame->m_outBar.m_BuildClass.SetProjectType("PROGRAM");				//exe,so,a
					break;
				case PROGRAM_DLL:
					pMainFrame->m_outBar.m_BuildClass.SetProjectType("DYNAMIC");				//exe,so,a
					break;
				case PROGRAM_LIB:
					pMainFrame->m_outBar.m_BuildClass.SetProjectType("LIBRARY");				//exe,so,a
					break;
			}
			*/
			pMainFrame->m_outBar.m_BuildClass.SetProjectType(m_strProgramType);				//exe,so,a
			SetCurrentOptionToBuild();
}

void CProjectInfo::RefreshSourceFile()
{
	// Refresh the Source File to the Build Class
	CString csFiles;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);
/*	
	if (m_root == NULL)
	{
		return ;
	}
	LPPROJECTITEM lpProjectItem;
	LPPROJECTITEM lpSourceItem = NULL;
	int iChildNum, iIndex;

	iChildNum = m_root->arrChild.GetSize();
	for (iIndex = 0; iIndex < iChildNum; iIndex++)
	{
		lpProjectItem = (LPPROJECTITEM)m_root->arrChild.GetAt(iIndex);

		ASSERT(lpProjectItem);

		if ((lpProjectItem->iItemType == PROJECT_FOLDER) && (lpProjectItem->strShowString.CompareNoCase("Source") == 0))
		{
			lpSourceItem = lpProjectItem; 
			break;
		}
	}
	if (lpSourceItem == NULL)
		return ;

	InsertFileString(lpSourceItem,csFiles);  // 将指定条目下的文件添加到指定的串中
*/
	InsertPathListToString(csFiles,m_arrSourceFileList);
	//设置该PROJECT的源文件
	pMainFrame->m_outBar.m_BuildClass.SetProjectCodeFiles(csFiles);			//!所有的OBJ文件
}
void CProjectInfo::RefreshLibraryFile()
{
	// Refresh the Library File to the Build Class
	CString csLibFiles,csDllFiles,csObjFiles;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);
/*	
	if (m_root == NULL)
	{
		return ;
	}
	LPPROJECTITEM lpProjectItem;
	LPPROJECTITEM lpSourceItem = NULL;
	int iChildNum, iIndex;

	iChildNum = m_root->arrChild.GetSize();
	for (iIndex = 0; iIndex < iChildNum; iIndex++)
	{
		lpProjectItem = (LPPROJECTITEM)m_root->arrChild.GetAt(iIndex);

		ASSERT(lpProjectItem);

		if ((lpProjectItem->iItemType == PROJECT_FOLDER) && (lpProjectItem->strShowString.CompareNoCase("Library") == 0))
		{
			lpSourceItem = lpProjectItem; 
			break;
		}
	}
	if (lpSourceItem == NULL)
		return ;

	InsertLibToFileString(lpSourceItem,csLibFiles,csDllFiles,csObjFiles);  // 将指定条目下的文件添加到指定的串中
*/
	InsertPathListToString(csLibFiles,m_arrLibFileList);
	InsertPathListToString(csDllFiles,m_arrDllFileList);
	InsertPathListToString(csObjFiles,m_arrObjFileList);

	pMainFrame->m_outBar.m_BuildClass.ClrLinkLibrary(LIB_TYPE_STATIC);							//!!清除所有记录
	pMainFrame->m_outBar.m_BuildClass.SetLinkLibrary(csLibFiles,LIB_TYPE_STATIC);		//!!可以多条记录

	pMainFrame->m_outBar.m_BuildClass.ClrLinkLibrary(LIB_TYPE_DYNLINK);							//!!清除所有记录
	pMainFrame->m_outBar.m_BuildClass.SetLinkLibrary(csDllFiles,LIB_TYPE_DYNLINK);		//!!可以多条记录


	//设置该PROJECT的OBJ文件
	pMainFrame->m_outBar.m_BuildClass.SetProjectObjFiles(csObjFiles);		//!所有的源文件
}
/*
void CProjectInfo::InsertFileString(LPPROJECTITEM lpSourceItem,CString &csFile)  // 将指定条目下的文件添加到指定的串中
{
	LPPROJECTITEM lpProjectItem;
	int iChildNum, iIndex;
	iChildNum = lpSourceItem->arrChild.GetSize();
	for (iIndex = 0; iIndex < iChildNum; iIndex++)
	{
		lpProjectItem = (LPPROJECTITEM)lpSourceItem->arrChild.GetAt(iIndex);

		ASSERT(lpProjectItem);

		if (lpProjectItem->iItemType == PROJECT_FOLDER)
		{
			InsertFileString(lpProjectItem,csFile);  // 将指定条目下的文件添加到指定的串中
			break;
		}
		csFile += lpProjectItem->strFileName;
		csFile += " ";
	}
}
*/
/*
void CProjectInfo::InsertLibToFileString(LPPROJECTITEM lpSourceItem,CString &csLibFile,CString &csDllFile,CString &csObjFile)  // 将指定条目下的文件添加到指定的串中
{
	LPPROJECTITEM lpProjectItem;
	int iChildNum, iIndex;
	iChildNum = lpSourceItem->arrChild.GetSize();
	for (iIndex = 0; iIndex < iChildNum; iIndex++)
	{
		lpProjectItem = (LPPROJECTITEM)lpSourceItem->arrChild.GetAt(iIndex);

		ASSERT(lpProjectItem);

		if (lpProjectItem->iItemType == PROJECT_FOLDER)
		{
			InsertLibToFileString(lpSourceItem,csLibFile,csDllFile,csObjFile);  // 将指定条目下的文件添加到指定的串中
			break;
		}
		int iType = GetFileType(lpProjectItem->strFileName);
		switch(iType)
		{
			case FILE_OBJECT:
				csObjFile += lpProjectItem->strFileName;
				csObjFile += " ";
				break;
			case FILE_LIBRARY:
				csLibFile += lpProjectItem->strFileName;
				csLibFile += " ";
				break;
			case FILE_DLL:
				csDllFile += lpProjectItem->strFileName;
				csDllFile += " ";
				break;
		}
	}
}
*/
int CProjectInfo::GetFileType(CString strFileName)
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
			return FILE_OBJECT;
		}
		if (strExt.CompareNoCase(".a") == 0)
		{ // it is static library
			return FILE_LIBRARY;
		}
		if (strExt.CompareNoCase(".so") == 0)
		{ // it is	dynamic link library
			return FILE_DLL;
		}

		return FILE_EXTERN;
}


void CProjectInfo::RefreshIncludePath()
{
	CString csInclude;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);

	InsertPathListToString(csInclude,m_arrIncludeDirList);
	pMainFrame->m_outBar.m_BuildClass.SetProjectInclude(csInclude);	//所有includes path
}
void CProjectInfo::RefreshLibraryPath()
{
	CString csLibpath;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);

	InsertPathListToString(csLibpath,m_arrLibraryDirList);
	pMainFrame->m_outBar.m_BuildClass.SetProjectLibpath(csLibpath);	//所有libs path
}
void CProjectInfo::InsertPathListToString(CString &strList, CStringArray &arrString)
{
	int nIndex;//,iLen;
	int iSize = arrString.GetSize();
	CString strString;
	CString strKingBuilderRoot;

	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKingBuilderRoot);
	
	nIndex = 0;
	while(nIndex < iSize)
	{
		strString = arrString.GetAt(nIndex);
		if (!strString.IsEmpty())		
		{
//			strString.Replace("$KINGBUILDERROOT",strKingBuilderRoot);
			strList += strString;
			strList += " ";
		}
		nIndex ++;
	}
}

int CProjectInfo::InsertCurFileToSpecialFileList(CString strFileName)
{
		int iType = GetFileType(strFileName);
		switch(iType)
		{
			case FILE_SOURCE:  // 将此文件添加到Source文件列表中
				m_arrSourceFileList.Add(strFileName);
				break;
			case FILE_HEADER:
				break;
			case FILE_OBJECT:  // 将此文件添加到Object文件列表中
				m_arrObjFileList.Add(strFileName);  
				break;
			case FILE_LIBRARY: // 将此文件添加到Library文件列表中
				m_arrLibFileList.Add(strFileName);  
				break;
			case FILE_DLL:		// 将此文件添加到dynamic link library文件列表中
				m_arrDllFileList.Add(strFileName);  
				break;
		}
		return -1;
}

//在lpParentItem下删除它的子项lpProjectItem
void CProjectInfo::DeleteSubProjectItem(LPPROJECTITEM lpParentItem,LPPROJECTITEM lpProjectItem)
{
	LPPROJECTITEM lpCurrentItem;

		ASSERT(lpParentItem);
		ASSERT(lpProjectItem);
		int iChildNum = lpParentItem->arrChild.GetSize();
		for (int iIndex = 0; iIndex < iChildNum; iIndex++)
		{
			lpCurrentItem = (LPPROJECTITEM)lpParentItem->arrChild.GetAt(iIndex);
			if (lpCurrentItem == lpProjectItem)
			{
				//找到要删除的项
				lpParentItem->arrChild.RemoveAt(iIndex);
				break;
			}
		}
		DeleteProjectItem(lpProjectItem);			
		RefreshSourceFile();
		RefreshLibraryFile();
}

void CProjectInfo::DeleteCurFileFromSpecialFileList(CString strFileName)
{
	CStringArray *pSpecialFileList = NULL;
	CString strCurString;
		int iType = GetFileType(strFileName);
		switch(iType)
		{
			case FILE_SOURCE:  // 将此文件添加到Source文件列表中
				pSpecialFileList = &m_arrSourceFileList;
				break;
			case FILE_OBJECT:  // 将此文件添加到Object文件列表中
				pSpecialFileList = &m_arrObjFileList;  
				break;
			case FILE_LIBRARY: // 将此文件添加到Library文件列表中
				pSpecialFileList = &m_arrLibFileList;  
				break;
			case FILE_DLL:		// 将此文件添加到dynamic link library文件列表中
				pSpecialFileList = &m_arrDllFileList;  
				break;
			default :
				pSpecialFileList = NULL;
		}
		if (pSpecialFileList)
		{
			int iSize = pSpecialFileList->GetSize( ) ;
			for (int iIndex = 0; iIndex < iSize; iIndex ++)
			{
				strCurString = pSpecialFileList->GetAt(iIndex);
				if (strCurString == strFileName)
				{
					pSpecialFileList->RemoveAt(iIndex);
					break;
				}
			}
		}
}

void CProjectInfo::GetCurrentOptionFromBuild()
{
	//得到Current的各Option的值
	BUILD_SETTING BuildSetting;

		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		ASSERT(pMainFrame);

		pMainFrame->m_outBar.m_BuildClass.GetCurBuildSetting(&BuildSetting); // Load Default Setting

		
		SetOption("BUILDTYPE",BuildSetting.csBuildType);
		SetOption("CompileWarnLevel",BuildSetting.uGccWarnLevel);
		SetOption("CompileOptimization",BuildSetting.csGccOpt);			//opt: default,disable, Maximize Speed,Minimize Size
		SetOption("CompilePredefines",BuildSetting.csGccPredefines);	//compile 预定义
		SetOption("CompileOptions",BuildSetting.csGccOtherOption);	//compile 其他设置

		SetOption("AssemberWarnLevel",BuildSetting.uAsWarnLevel);		//assember warning level：0 ~ 2级	
		SetOption("AssemberPredefines",BuildSetting.csAsPredefines);		//assember 预定义
		SetOption("AsseberOptions",BuildSetting.csAsOtherOption);	//assember 其他设置

		SetOption("LinkOutputMap",BuildSetting.bOutputMap);			//link: 输出map文件.
		SetOption("LinkOptions",BuildSetting.csLinkOtherOption);	//link  其他设置
}

void CProjectInfo::SetDefaultOptionToBuild()
{
	// 将系统设置到Default状态
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		ASSERT(pMainFrame);
		pMainFrame->m_outBar.m_BuildClass.SetBuildForDefault(); // Load Default Setting

		// 添加默认的Include Path
		CString strPath;
		CString strKingBuilderRoot;

		((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKingBuilderRoot);
		
		strPath.Format("%s\\include",strKingBuilderRoot); // kingbuilder\include
//		strPath.Format("%s\\include","$KINGBUILDERROOT"); // kingbuilder\include
		m_arrIncludeDirList.Add(strPath);
}
void CProjectInfo::SetCurrentOptionToBuild()
{
	// 将系统设置为当前最新状态
	CString strValue;
	int iValue;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		ASSERT(pMainFrame);
		GetOption("BUILDTYPE",strValue);
		pMainFrame->m_outBar.m_BuildClass.SetBuildVersion(strValue);

		GetOption("CompileWarnLevel",iValue);
		pMainFrame->m_outBar.m_BuildClass.SetCompileWarnLevel(iValue);	//warning level

		GetOption("CompileOptimization",strValue);			//opt: default,disable, Maximize Speed,Minimize Size
		pMainFrame->m_outBar.m_BuildClass.SetCompileOptimization(strValue);

		GetOption("CompilePredefines",strValue);	//compile 预定义
		pMainFrame->m_outBar.m_BuildClass.SetCompilePredefines(strValue);

		GetOption("CompileOptions",strValue);	//compile 其他设置
		pMainFrame->m_outBar.m_BuildClass.SetCompileOptions(strValue);

		GetOption("AssemberWarnLevel",iValue);		//assember warning level：0 ~ 2级	
		pMainFrame->m_outBar.m_BuildClass.SetAssemberWarnLevel(iValue);

		GetOption("AssemberPredefines",strValue);		//assember 预定义
		pMainFrame->m_outBar.m_BuildClass.SetAssemberPredefines(strValue);

		GetOption("AsseberOptions",strValue);	//assember 其他设置
		pMainFrame->m_outBar.m_BuildClass.SetAsseberOptions(strValue);

		GetOption("LinkOutputFileName", strValue);
		pMainFrame->m_outBar.m_BuildClass.SetLinkOutputName(strValue);

		GetOption("LinkOutputMap",iValue);			//link: 输出map文件.
		pMainFrame->m_outBar.m_BuildClass.SetLinkOutputMap(iValue);

		GetOption("LinkOptions",strValue);	//link  其他设置
		pMainFrame->m_outBar.m_BuildClass.SetLinkOptions(strValue);

		RefreshLibraryFile();
		RefreshIncludePath();
		RefreshLibraryPath();
}

void CProjectInfo::LoadDefaultProjectInfo()
{
		SetDefaultOptionToBuild();
		GetCurrentOptionFromBuild();

		// 设置默认的输出文件名
		CString strLinkOutputName;
		strLinkOutputName = m_strProjectName;
		/*
		switch(m_iProgramType)
		{
			case PROGRAM_EXE:
				strLinkOutputName += ".exe";				//exe,so,a
				break;
			case PROGRAM_DLL:
				strLinkOutputName += ".so";
				break;
			case PROGRAM_LIB:
				strLinkOutputName += ".a";
				break;
		}
		*/
		if (m_strProgramType.Compare("PROGRAM") == 0)
		{
			strLinkOutputName += ".exe";				//exe,so,a
		}
		else if (m_strProgramType.Compare("DYNAMIC") == 0)
		{
				strLinkOutputName += ".so";
		}
		else if (m_strProgramType.Compare("LIBRARY") == 0)
		{
				strLinkOutputName += ".a";
		}
		//pMainFrame->m_outBar.m_BuildClass.SetLinkOutputName(strLinkOutputName);
		SetOption("LinkOutputFileName", strLinkOutputName);

		// 设置默认的文件需要
		InsertChild(m_root,"Source",PROJECT_FOLDER);
		InsertChild(m_root,"Header",PROJECT_FOLDER);
		InsertChild(m_root,"Library",PROJECT_FOLDER);
		InsertChild(m_root,"Extern",PROJECT_FOLDER);

//		strLibName;
//		InsertChild(m_root,,PROJECT_FILE);
}

BOOL CProjectInfo::ReadModule(CString strModuleFile)
{
	// 读取模块数据
	m_srcData.Load(strModuleFile);
	return FALSE;
}

BOOL CProjectInfo::AnalysisModule()
{
	CString csTitle("Project: ");
	CString csName;
	CString csType;
	CString csSources;
	CString csFullName;
//	UINT iRet;

	// 得到工程文件名
	if (m_srcData.Get (TEXT("MODULENAME"), csName)
		&&m_srcData.Get (TEXT("MODULETYPE"), csType))
	{
		m_strProjectName = csName;

		m_strProgramType = csType;
		// 设置默认的输出文件名
		CString strLinkOutputName;
		strLinkOutputName = m_strProjectName;

		if (m_strProgramType.Compare("PROGRAM") == 0)
		{
			strLinkOutputName += ".exe";				//exe,so,a
		}
		else if (m_strProgramType.Compare("DYNAMIC") == 0)
		{
				strLinkOutputName += ".so";
		}
		else if (m_strProgramType.Compare("LIBRARY") == 0)
		{
				strLinkOutputName += ".a";
		}
		SetOption("LinkOutputFileName", strLinkOutputName);

		// 设置默认的文件需要
		LPPROJECTITEM pSourceItem = InsertChild(m_root,"Source",PROJECT_FOLDER);

		// 插入源文件
		if (m_srcData.Get (TEXT("LOCAL_FILES"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					GetFullFileName(csFile,csFullName);
					InsertChild(pSourceItem,csFullName,PROJECT_FILE);
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetFullFileName(csFile,csFullName);
					InsertChild(pSourceItem,csFullName,PROJECT_FILE);
				}
			}while (1);
		}


		LPPROJECTITEM pHeaderItem = InsertChild(m_root,"Header",PROJECT_FOLDER);
		// 插入头文件
		{
			// Module 只支持Header path
			// 插入Include Path
			if (m_srcData.Get (TEXT("MODULE_INCLUDES"), csSources))
			{
				CString csFile;
				int nStart = 0, nEnd;
				do {
					nEnd = csSources.Find (TEXT(' '), nStart);
					if (nEnd == -1)
					{
						csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
						m_arrIncludeDirList.Add(csFile);
						break;
					}
					else
					{
						csFile = csSources.Mid (nStart, nEnd-nStart);
						nStart = nEnd+1;
						m_arrIncludeDirList.Add(csFile);
					}
				}while (1);
			}
		}
		LPPROJECTITEM pLibraryItem = InsertChild(m_root,"Library",PROJECT_FOLDER);
		// 插入库文件
		// 得到工程需要的静态库
		if (m_srcData.Get (TEXT("STATICLIBS"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
				}
			}while (1);
		}
		// 得到工程需要的动态库
		if (m_srcData.Get (TEXT("DYNAMICLIBS"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
				}
			}while (1);
		}
		// 得到工程需要的本地静态库
		if (m_srcData.Get (TEXT("LOCAL_STATICLIBS"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
				}
			}while (1);
		}
		// 得到工程需要的本地动态库
		if (m_srcData.Get (TEXT("LOCAL_DYNAMICLIBS"), csSources))
		{
			CString csFile;
			int nStart = 0, nEnd;
			do {
				nEnd = csSources.Find (TEXT(' '), nStart);
				if (nEnd == -1)
				{
					csFile = csSources.Mid (nStart, csSources.GetLength()-nStart);
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
					break;
				}
				else
				{
					csFile = csSources.Mid (nStart, nEnd-nStart);
					nStart = nEnd+1;
					GetFullFileName(csFile,csFullName);
					InsertChild(pLibraryItem,csFullName,PROJECT_FILE);
				}
			}while (1);
		}

		LPPROJECTITEM pExternItem = InsertChild(m_root,"Extern",PROJECT_FOLDER);
		// 插入其他文件
		{
			// Module 中不包含其他文件
		}
		return TRUE;
	}
	return FALSE;
}

void CProjectInfo::GetModuleDir(CString strModulesName,CString &strLocation)
{
	int nCount = 0;
	
		nCount = strModulesName.ReverseFind('\\'); // 得到最后一个'\\'的位置
		strLocation = strModulesName.Left(nCount);
}

void CProjectInfo::GetFullFileName(CString csFile, CString &csFullName)
{
	// 查找有没有盘符，确定是不是全路径
	int nStart = 0,nNext,nEnd;
	CString strPath,strFolder;
	
		if (csFile.IsEmpty())
			return ;

		if (csFile.GetAt(0) == '$')
		{
			CString strSysPath;
			// 这是一个带系统路径的文件  arm -- CPU
			//$(__SDK_TARGETS)  --  Kingmos\sdk\target\arm\release
			//$(__SDK_LIBS) -- Kingmos\sdk\lib\arm\release
			//$(__PUBLIC_LIBS) -- kingBuilder\sdk\lib\arm\Release
			//$(__PUBLIC_TARGETS) -- kingBuilder\sdk\lib\arm\Release
			strSysPath = csFile.Left(csFile.Find('/',0) +1);
			if (strSysPath == "$(__SDK_TARGETS)/")
			{
				CString strKingmosRoot;
				((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosRoot);
				csFullName = strKingmosRoot;
				csFullName += "\\sdk\\target\\";
				csFullName += m_strCPUType;
				csFullName += "\\RELEASE\\";
				csFullName += csFile.Mid(strSysPath.GetLength());;
			}
			else if (strSysPath == "$(__SDK_LIBS)/")
			{
				CString strKingmosRoot;
				((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosRoot);
				csFullName = strKingmosRoot;
				csFullName += "\\sdk\\lib\\";
				csFullName += m_strCPUType;
				csFullName += "\\RELEASE\\";
				csFullName += csFile.Mid(strSysPath.GetLength());;
			}
			else if (strSysPath == "$(__PUBLIC_LIBS)/")
			{
				CString strKingBuilderRoot;
				((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKingBuilderRoot);
				csFullName = strKingBuilderRoot;
				csFullName += "\\sdk\\lib\\";
				csFullName += m_strCPUType;
				csFullName += "\\RELEASE\\";
				csFullName += csFile.Mid(strSysPath.GetLength());;
			}
			else if (strSysPath == "$(__PUBLIC_TARGETS)/")
			{
				CString strKingBuilderRoot;
				((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKingBuilderRoot);
				csFullName = strKingBuilderRoot;
				csFullName += "\\sdk\\target\\";
				csFullName += m_strCPUType;
				csFullName += "\\RELEASE\\";
				csFullName += csFile.Mid(strSysPath.GetLength());;
			}
			else
			{
				CString strMsg;
				strMsg.Format("路径: %s 有错",csFile);
				AfxMessageBox(strMsg);
			}
			return;
		}

		strPath = m_strProjectRoot;
		nNext = csFile.Find(':',0);
		if (nNext != -1)
		{
			//是全路径，直接拷贝就可以
			csFullName = csFile;
			return ;
		}
		while(1)
		{
			nNext = csFile.Find('\\',nStart);
			if (nNext == -1)
			{
				csFullName = strPath + '\\';  // 将路径加到文件上
				csFullName +=csFile.Mid(nStart);
				return;
			}
			strFolder = csFile.Mid(nStart,nNext - nStart);
			if (strFolder == "..")
			{
				nEnd = strPath.ReverseFind('\\'); // 得到最后一个'\\'的位置
				strPath = strPath.Left(nEnd); // 得到上一级目录
				nStart = nNext +1;
				continue;
			}
			if (strFolder == ".")
			{
				nStart = nNext +1;
				continue;
			}
			//为当前路径下的一个文件夹，直接加到当前路径上就可以了
			csFullName = strPath + '\\';  // 将路径加到文件上
			csFullName +=csFile.Mid(nStart);
			return;
		}
		
}
/*
void CProjectInfo::SaveOpenedFile(CFile *pFile)
{
   POSITION pos;
   CSpecialView * pView;
   CAmboDoc * pDoc,*pCurDoc;
   int iLen ;
   CPoint ptCaret;
   int iCount;
		
	CMainFrame* pMainFrame = (CMainFrame*)((CAmboApp *)AfxGetApp())->m_pMainWnd;

	ASSERT(pMainFrame);
	CFrameWnd* pChildFrame = pMainFrame->MDIGetActive();
	if (pChildFrame)
	{
		pDoc = (CAmboDoc*)pChildFrame->GetActiveDocument();
		
		if (pDoc == NULL)
			return;
		
		iCount = 0;
		pos = pDoc->GetFirstViewPosition();
		while (pos != NULL)
		{			 
			pView = (CSpecialView*)pDoc->GetNextView(pos); // get first one
			if (pView)
			{
				iCount ++;
			}
		}

		// 写入文件总数
		pFile->Write(&iCount,sizeof(int));
		pos = pDoc->GetFirstViewPosition();
		while (pos != NULL)
		{
			pView = (CSpecialView *)pDoc->GetNextView(pos); // get first one
			if (pView)
			{
				pCurDoc = (CAmboDoc *)pView->GetDocument();
				CString strFileName;
				strFileName = pCurDoc->GetPathName();

				// 写入strFileName
				iLen = strFileName.GetLength() +1;  //包含结束符
				pFile->Write(&iLen,sizeof(int));
				pFile->Write((LPCTSTR)strFileName,iLen);

				pView->h_Edit->GetEditCaretPos(ptCaret);

				// 写入光标位置
				pFile->Write(&ptCaret.x,sizeof(int));
				pFile->Write(&ptCaret.y,sizeof(int));

			}
		}   
	}
}

*/
void CProjectInfo::SaveOpenedFile(CFile *pFile)
{
   POSITION pos,posDocTemp,posDoc;
   CSpecialView * pView;
   CAmboDoc * pDoc;
   int iLen ;
   CPoint ptCaret;
   int iCount;
   

    // 得到文档模板
    posDocTemp = AfxGetApp()->GetFirstDocTemplatePosition();
	if (posDocTemp == NULL)
		return;
	CDocTemplate * pDocTemplate = AfxGetApp()->GetNextDocTemplate(posDocTemp);

	if (pDocTemplate == NULL)
		return;
	
    // 得到文档个数
	iCount = 0;
	posDoc = pDocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		pDoc = (CAmboDoc*)pDocTemplate->GetNextDoc(posDoc);
		if (pDoc)
		{
			iCount ++;
		}
	}
	// 写入文件总数
	pFile->Write(&iCount,sizeof(int));

    // 得到每个文档资料
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CAmboFrame *pAmboFrame = (CAmboFrame *)pMainFrame->GetActiveFrame();
	if (pAmboFrame == NULL)
		return;
	CAmboDoc *pActiveDoc = (CAmboDoc *)pAmboFrame->GetActiveDocument();

	posDoc = pDocTemplate->GetFirstDocPosition();
	while (posDoc != NULL)
	{
		pDoc = (CAmboDoc*)pDocTemplate->GetNextDoc(posDoc);
		if (pActiveDoc == pDoc)
			continue; // 激活文档将放在最后
		if (pDoc)
		{
			pos = pDoc->GetFirstViewPosition();
			if (pos == NULL)
				continue;
			pView = (CSpecialView*)pDoc->GetNextView(pos); // get first one
			if (pView == NULL)
				continue;
//			pCurDoc = (CAmboDoc *)pView->GetDocument();
			CString strFileName;
			strFileName = pDoc->GetPathName();

			// 写入strFileName
			iLen = strFileName.GetLength() +1;  //包含结束符
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)strFileName,iLen);

			pView->h_Edit->GetEditCaretPos(ptCaret);

			// 写入光标位置
			pFile->Write(&ptCaret.x,sizeof(int));
			pFile->Write(&ptCaret.y,sizeof(int));
		}
	}
	// 存入激活文档
	if (pActiveDoc)
	{
			pos = pActiveDoc->GetFirstViewPosition();
			if (pos == NULL)
				return;
			pView = (CSpecialView*)pActiveDoc->GetNextView(pos); // get first one
			if (pView == NULL)
				return;
//			pCurDoc = (CAmboDoc *)pView->GetDocument();
			CString strFileName;
			strFileName = pActiveDoc->GetPathName();

			// 写入strFileName
			iLen = strFileName.GetLength() +1;  //包含结束符
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)strFileName,iLen);

			pView->h_Edit->GetEditCaretPos(ptCaret);

			// 写入光标位置
			pFile->Write(&ptCaret.x,sizeof(int));
			pFile->Write(&ptCaret.y,sizeof(int));
	}
}

void CProjectInfo::ReOpenOpenedFile(CFile *pFile)
{
	int iCount = 0, i;
	int iLen;
	CString strFileName;
	CPoint ptCaret;

	// 读取文件总数
	if (pFile->Read(&iCount,sizeof(int)) == 0)
		iCount = 0;

	// 保留当前的窗口最大化模式
	BOOL bMaxWindow = ((CAmboApp *)AfxGetApp())->GetWindowMax();
	// 设置当前的窗口不需要最大化
	((CAmboApp *)AfxGetApp())->SetWindowMax(FALSE);
	for (i = 0; i < iCount ; i++)
	{
		// 读取文件名
		pFile->Read(&iLen,sizeof(int));
		pFile->Read(strFileName.GetBuffer(iLen),iLen);
		strFileName.ReleaseBuffer();
		// 写入光标位置
		pFile->Read(&ptCaret.x,sizeof(int));
		pFile->Read(&ptCaret.y,sizeof(int));
		// 打开这个文件
		if (i == iCount - 1)
		{
			// 最后一个窗口，恢复最大化模式
			((CAmboApp *)AfxGetApp())->SetWindowMax(bMaxWindow);
		}
		if (AfxGetApp()->OpenDocumentFile(strFileName) != NULL)
		{
			((CAmboApp *)AfxGetApp())->SetCaretPos(ptCaret);
		}
	}
}

void CProjectInfo::Save()
{
	SaveProject();
}

void CProjectInfo::CloseAllWindow()
{
   POSITION pos;
   
    // 得到文档模板
    pos = AfxGetApp()->GetFirstDocTemplatePosition();
	if (pos == NULL)
		return;
	CDocTemplate * pDocTemplate = AfxGetApp()->GetNextDocTemplate(pos);

	if (pDocTemplate == NULL)
		return;
	pDocTemplate->CloseAllDocuments(TRUE);

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMainFrame->UpdateWindow();
	
}

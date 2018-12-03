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
	//��һ��KINGMOS��MODULESת��Ϊһ�������ļ�
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

	//��ȡģ�����
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
	// д��CPU Type
	int iLen = m_strCPUType.GetLength() +1;  //����������
	fileProject.Write(&iLen,sizeof(int));
	fileProject.Write((LPCTSTR)m_strCPUType,iLen);

	// д��Program Type
	iLen = m_strProgramType.GetLength() +1;  //����������
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

	//���湤���Ѿ��򿪵��ļ�
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

	//�õ���������
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

	//�õ�����Ŀ¼
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

	// ��ȡCPU Type
	int iLen;
	fileProject.Read(&iLen,sizeof(int));
	fileProject.Read(m_strCPUType.GetBuffer(iLen),iLen);
	m_strCPUType.ReleaseBuffer();


	// ��ȡProgram Type
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

	// ��ȡҪ������Ŀ¼
	ReadCStringArray(&fileProject,m_arrIncludeDirList);
	ReadCStringArray(&fileProject,m_arrLibraryDirList);

	// �򿪹����Ѿ��򿪵��ļ�
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

	m_arrIncludeDirList.RemoveAll(); //��������е�����
	m_arrLibraryDirList.RemoveAll(); //��������е�����

	m_arrSourceFileList.RemoveAll(); //��������е�����

	m_arrLibFileList.RemoveAll(); //��������е�����
	m_arrDllFileList.RemoveAll(); //��������е�����
	m_arrObjFileList.RemoveAll(); //��������е�����

	m_strProjectName = "";
	m_strProjectRoot = "";
	m_strFileName = "";
	m_bOpen = FALSE;
	return TRUE;
}
BOOL CProjectInfo::WriteProjectItem(CFile *pFile,LPPROJECTITEM lpProjectItem)
{
	int iContentNum;

	// 1 -- ������Ŀ����
	pFile->Write(&lpProjectItem->iItemType,sizeof(int));
	// 2 -- ������Ŀ����
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
	// 3 -- ��������Ŀ��Ŀ
	iContentNum = lpProjectItem->arrChild.GetSize();
	pFile->Write(&iContentNum,sizeof(int));
	// 4 -- ��������Ŀ
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

	// 1 -- ��ȡ��Ŀ����
	pFile->Read(&lpProjectItem->iItemType,sizeof(int));
	// 2 -- ��ȡ��Ŀ����
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
	//�ɹ���ȡ����Ŀ�����뵽�б���
	if (lpParent)
	{
		lpParent->arrChild.Add(lpProjectItem);
	}
	// 3 -- ��ȡ����Ŀ��Ŀ
	pFile->Read(&iContentNum,sizeof(int));
	// 4 -- ��ȡ����Ŀ
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
				//���ļ��Ѿ������ڸù�����
				delete lpNewItem;
				((CAmboApp *)AfxGetApp())->m_pMainWnd->MessageBox("���ļ��Ѿ������ڸù�����");
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
			// ����ʾ�ִ��Ѿ����ڣ�ʹ����һ���ִ�
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
			{//��ѡ������Ѿ����ڣ������µ�ֵ
				lpCurOption->strValue = strValue;
				lpCurOption->iValidateItem = STR_VALUE;
				return ;
			}
		}
		nIndex ++;
	}
	//��ѡ����������ڣ�����µ�ѡ�����
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
			{//��ѡ������Ѿ����ڣ������µ�ֵ
				lpCurOption->iValue = iValue;
				lpCurOption->iValidateItem = INT_VALUE;
				return ;
			}
		}
		nIndex ++;
	}
	//��ѡ����������ڣ�����µ�ѡ�����
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
			{//��ѡ������Ѿ��ҵ����õ��µ�ֵ
				if (lpCurOption->iValidateItem == STR_VALUE)
				{
					strValue = lpCurOption->strValue ;
				}
				else
				{
					// �ñ���������һ��CString��ֵ
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
			{//��ѡ������Ѿ��ҵ����õ��µ�ֵ
				if (lpCurOption->iValidateItem == INT_VALUE)
				{
					iValue = lpCurOption->iValue ;
				}
				else
				{
					// �ñ���������һ��INT��ֵ
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
		// ��ȡ������
		pFile->Read(&iLen,sizeof(int));
		pFile->Read(strVariable.GetBuffer(iLen),iLen);
		strVariable.ReleaseBuffer();
		
		// ��ȡ����ֵ
		
		// ��ȡ��������
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
	// д�������Ŀ
	pFile->Write(&iSize,sizeof(int));
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurOption = (LPOPTIONVARIABLE)arrOptionVar.GetAt(nIndex);
		if (lpCurOption)
		{	// д�������
			iLen = lpCurOption->strVariable.GetLength() +1;  //����������
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)lpCurOption->strVariable,iLen);
			
			// д�����ֵ

			// д��������� -- �ִ� ���� ��ֵ
			pFile->Write(&lpCurOption->iValidateItem,sizeof(int));
			
			if (lpCurOption->iValidateItem == STR_VALUE)
			{//д���ִ�
				iLen = lpCurOption->strValue.GetLength() +1;  //����������
				pFile->Write(&iLen,sizeof(int));
				pFile->Write((LPCTSTR)lpCurOption->strValue,iLen);
			}
			else
			{ // д����ֵ
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
	// д�������Ŀ
	pFile->Write(&iSize,sizeof(int));
	nIndex = 0;
	while(nIndex < iSize)
	{
		strString = arrString.GetAt(nIndex);
		// д������
		iLen = strString.GetLength() +1;  //����������
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
		// ��ȡ������
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
			pMainFrame->m_outBar.m_BuildClass.SetWorkType(FALSE);  //���ò���һ��Platform

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

	InsertFileString(lpSourceItem,csFiles);  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
*/
	InsertPathListToString(csFiles,m_arrSourceFileList);
	//���ø�PROJECT��Դ�ļ�
	pMainFrame->m_outBar.m_BuildClass.SetProjectCodeFiles(csFiles);			//!���е�OBJ�ļ�
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

	InsertLibToFileString(lpSourceItem,csLibFiles,csDllFiles,csObjFiles);  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
*/
	InsertPathListToString(csLibFiles,m_arrLibFileList);
	InsertPathListToString(csDllFiles,m_arrDllFileList);
	InsertPathListToString(csObjFiles,m_arrObjFileList);

	pMainFrame->m_outBar.m_BuildClass.ClrLinkLibrary(LIB_TYPE_STATIC);							//!!������м�¼
	pMainFrame->m_outBar.m_BuildClass.SetLinkLibrary(csLibFiles,LIB_TYPE_STATIC);		//!!���Զ�����¼

	pMainFrame->m_outBar.m_BuildClass.ClrLinkLibrary(LIB_TYPE_DYNLINK);							//!!������м�¼
	pMainFrame->m_outBar.m_BuildClass.SetLinkLibrary(csDllFiles,LIB_TYPE_DYNLINK);		//!!���Զ�����¼


	//���ø�PROJECT��OBJ�ļ�
	pMainFrame->m_outBar.m_BuildClass.SetProjectObjFiles(csObjFiles);		//!���е�Դ�ļ�
}
/*
void CProjectInfo::InsertFileString(LPPROJECTITEM lpSourceItem,CString &csFile)  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
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
			InsertFileString(lpProjectItem,csFile);  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
			break;
		}
		csFile += lpProjectItem->strFileName;
		csFile += " ";
	}
}
*/
/*
void CProjectInfo::InsertLibToFileString(LPPROJECTITEM lpSourceItem,CString &csLibFile,CString &csDllFile,CString &csObjFile)  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
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
			InsertLibToFileString(lpSourceItem,csLibFile,csDllFile,csObjFile);  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
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
	pMainFrame->m_outBar.m_BuildClass.SetProjectInclude(csInclude);	//����includes path
}
void CProjectInfo::RefreshLibraryPath()
{
	CString csLibpath;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);

	InsertPathListToString(csLibpath,m_arrLibraryDirList);
	pMainFrame->m_outBar.m_BuildClass.SetProjectLibpath(csLibpath);	//����libs path
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
			case FILE_SOURCE:  // �����ļ���ӵ�Source�ļ��б���
				m_arrSourceFileList.Add(strFileName);
				break;
			case FILE_HEADER:
				break;
			case FILE_OBJECT:  // �����ļ���ӵ�Object�ļ��б���
				m_arrObjFileList.Add(strFileName);  
				break;
			case FILE_LIBRARY: // �����ļ���ӵ�Library�ļ��б���
				m_arrLibFileList.Add(strFileName);  
				break;
			case FILE_DLL:		// �����ļ���ӵ�dynamic link library�ļ��б���
				m_arrDllFileList.Add(strFileName);  
				break;
		}
		return -1;
}

//��lpParentItem��ɾ����������lpProjectItem
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
				//�ҵ�Ҫɾ������
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
			case FILE_SOURCE:  // �����ļ���ӵ�Source�ļ��б���
				pSpecialFileList = &m_arrSourceFileList;
				break;
			case FILE_OBJECT:  // �����ļ���ӵ�Object�ļ��б���
				pSpecialFileList = &m_arrObjFileList;  
				break;
			case FILE_LIBRARY: // �����ļ���ӵ�Library�ļ��б���
				pSpecialFileList = &m_arrLibFileList;  
				break;
			case FILE_DLL:		// �����ļ���ӵ�dynamic link library�ļ��б���
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
	//�õ�Current�ĸ�Option��ֵ
	BUILD_SETTING BuildSetting;

		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		ASSERT(pMainFrame);

		pMainFrame->m_outBar.m_BuildClass.GetCurBuildSetting(&BuildSetting); // Load Default Setting

		
		SetOption("BUILDTYPE",BuildSetting.csBuildType);
		SetOption("CompileWarnLevel",BuildSetting.uGccWarnLevel);
		SetOption("CompileOptimization",BuildSetting.csGccOpt);			//opt: default,disable, Maximize Speed,Minimize Size
		SetOption("CompilePredefines",BuildSetting.csGccPredefines);	//compile Ԥ����
		SetOption("CompileOptions",BuildSetting.csGccOtherOption);	//compile ��������

		SetOption("AssemberWarnLevel",BuildSetting.uAsWarnLevel);		//assember warning level��0 ~ 2��	
		SetOption("AssemberPredefines",BuildSetting.csAsPredefines);		//assember Ԥ����
		SetOption("AsseberOptions",BuildSetting.csAsOtherOption);	//assember ��������

		SetOption("LinkOutputMap",BuildSetting.bOutputMap);			//link: ���map�ļ�.
		SetOption("LinkOptions",BuildSetting.csLinkOtherOption);	//link  ��������
}

void CProjectInfo::SetDefaultOptionToBuild()
{
	// ��ϵͳ���õ�Default״̬
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		ASSERT(pMainFrame);
		pMainFrame->m_outBar.m_BuildClass.SetBuildForDefault(); // Load Default Setting

		// ���Ĭ�ϵ�Include Path
		CString strPath;
		CString strKingBuilderRoot;

		((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKingBuilderRoot);
		
		strPath.Format("%s\\include",strKingBuilderRoot); // kingbuilder\include
//		strPath.Format("%s\\include","$KINGBUILDERROOT"); // kingbuilder\include
		m_arrIncludeDirList.Add(strPath);
}
void CProjectInfo::SetCurrentOptionToBuild()
{
	// ��ϵͳ����Ϊ��ǰ����״̬
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

		GetOption("CompilePredefines",strValue);	//compile Ԥ����
		pMainFrame->m_outBar.m_BuildClass.SetCompilePredefines(strValue);

		GetOption("CompileOptions",strValue);	//compile ��������
		pMainFrame->m_outBar.m_BuildClass.SetCompileOptions(strValue);

		GetOption("AssemberWarnLevel",iValue);		//assember warning level��0 ~ 2��	
		pMainFrame->m_outBar.m_BuildClass.SetAssemberWarnLevel(iValue);

		GetOption("AssemberPredefines",strValue);		//assember Ԥ����
		pMainFrame->m_outBar.m_BuildClass.SetAssemberPredefines(strValue);

		GetOption("AsseberOptions",strValue);	//assember ��������
		pMainFrame->m_outBar.m_BuildClass.SetAsseberOptions(strValue);

		GetOption("LinkOutputFileName", strValue);
		pMainFrame->m_outBar.m_BuildClass.SetLinkOutputName(strValue);

		GetOption("LinkOutputMap",iValue);			//link: ���map�ļ�.
		pMainFrame->m_outBar.m_BuildClass.SetLinkOutputMap(iValue);

		GetOption("LinkOptions",strValue);	//link  ��������
		pMainFrame->m_outBar.m_BuildClass.SetLinkOptions(strValue);

		RefreshLibraryFile();
		RefreshIncludePath();
		RefreshLibraryPath();
}

void CProjectInfo::LoadDefaultProjectInfo()
{
		SetDefaultOptionToBuild();
		GetCurrentOptionFromBuild();

		// ����Ĭ�ϵ�����ļ���
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

		// ����Ĭ�ϵ��ļ���Ҫ
		InsertChild(m_root,"Source",PROJECT_FOLDER);
		InsertChild(m_root,"Header",PROJECT_FOLDER);
		InsertChild(m_root,"Library",PROJECT_FOLDER);
		InsertChild(m_root,"Extern",PROJECT_FOLDER);

//		strLibName;
//		InsertChild(m_root,,PROJECT_FILE);
}

BOOL CProjectInfo::ReadModule(CString strModuleFile)
{
	// ��ȡģ������
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

	// �õ������ļ���
	if (m_srcData.Get (TEXT("MODULENAME"), csName)
		&&m_srcData.Get (TEXT("MODULETYPE"), csType))
	{
		m_strProjectName = csName;

		m_strProgramType = csType;
		// ����Ĭ�ϵ�����ļ���
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

		// ����Ĭ�ϵ��ļ���Ҫ
		LPPROJECTITEM pSourceItem = InsertChild(m_root,"Source",PROJECT_FOLDER);

		// ����Դ�ļ�
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
		// ����ͷ�ļ�
		{
			// Module ֻ֧��Header path
			// ����Include Path
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
		// ������ļ�
		// �õ�������Ҫ�ľ�̬��
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
		// �õ�������Ҫ�Ķ�̬��
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
		// �õ�������Ҫ�ı��ؾ�̬��
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
		// �õ�������Ҫ�ı��ض�̬��
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
		// ���������ļ�
		{
			// Module �в����������ļ�
		}
		return TRUE;
	}
	return FALSE;
}

void CProjectInfo::GetModuleDir(CString strModulesName,CString &strLocation)
{
	int nCount = 0;
	
		nCount = strModulesName.ReverseFind('\\'); // �õ����һ��'\\'��λ��
		strLocation = strModulesName.Left(nCount);
}

void CProjectInfo::GetFullFileName(CString csFile, CString &csFullName)
{
	// ������û���̷���ȷ���ǲ���ȫ·��
	int nStart = 0,nNext,nEnd;
	CString strPath,strFolder;
	
		if (csFile.IsEmpty())
			return ;

		if (csFile.GetAt(0) == '$')
		{
			CString strSysPath;
			// ����һ����ϵͳ·�����ļ�  arm -- CPU
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
				strMsg.Format("·��: %s �д�",csFile);
				AfxMessageBox(strMsg);
			}
			return;
		}

		strPath = m_strProjectRoot;
		nNext = csFile.Find(':',0);
		if (nNext != -1)
		{
			//��ȫ·����ֱ�ӿ����Ϳ���
			csFullName = csFile;
			return ;
		}
		while(1)
		{
			nNext = csFile.Find('\\',nStart);
			if (nNext == -1)
			{
				csFullName = strPath + '\\';  // ��·���ӵ��ļ���
				csFullName +=csFile.Mid(nStart);
				return;
			}
			strFolder = csFile.Mid(nStart,nNext - nStart);
			if (strFolder == "..")
			{
				nEnd = strPath.ReverseFind('\\'); // �õ����һ��'\\'��λ��
				strPath = strPath.Left(nEnd); // �õ���һ��Ŀ¼
				nStart = nNext +1;
				continue;
			}
			if (strFolder == ".")
			{
				nStart = nNext +1;
				continue;
			}
			//Ϊ��ǰ·���µ�һ���ļ��У�ֱ�Ӽӵ���ǰ·���ϾͿ�����
			csFullName = strPath + '\\';  // ��·���ӵ��ļ���
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

		// д���ļ�����
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

				// д��strFileName
				iLen = strFileName.GetLength() +1;  //����������
				pFile->Write(&iLen,sizeof(int));
				pFile->Write((LPCTSTR)strFileName,iLen);

				pView->h_Edit->GetEditCaretPos(ptCaret);

				// д����λ��
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
   

    // �õ��ĵ�ģ��
    posDocTemp = AfxGetApp()->GetFirstDocTemplatePosition();
	if (posDocTemp == NULL)
		return;
	CDocTemplate * pDocTemplate = AfxGetApp()->GetNextDocTemplate(posDocTemp);

	if (pDocTemplate == NULL)
		return;
	
    // �õ��ĵ�����
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
	// д���ļ�����
	pFile->Write(&iCount,sizeof(int));

    // �õ�ÿ���ĵ�����
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
			continue; // �����ĵ����������
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

			// д��strFileName
			iLen = strFileName.GetLength() +1;  //����������
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)strFileName,iLen);

			pView->h_Edit->GetEditCaretPos(ptCaret);

			// д����λ��
			pFile->Write(&ptCaret.x,sizeof(int));
			pFile->Write(&ptCaret.y,sizeof(int));
		}
	}
	// ���뼤���ĵ�
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

			// д��strFileName
			iLen = strFileName.GetLength() +1;  //����������
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)strFileName,iLen);

			pView->h_Edit->GetEditCaretPos(ptCaret);

			// д����λ��
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

	// ��ȡ�ļ�����
	if (pFile->Read(&iCount,sizeof(int)) == 0)
		iCount = 0;

	// ������ǰ�Ĵ������ģʽ
	BOOL bMaxWindow = ((CAmboApp *)AfxGetApp())->GetWindowMax();
	// ���õ�ǰ�Ĵ��ڲ���Ҫ���
	((CAmboApp *)AfxGetApp())->SetWindowMax(FALSE);
	for (i = 0; i < iCount ; i++)
	{
		// ��ȡ�ļ���
		pFile->Read(&iLen,sizeof(int));
		pFile->Read(strFileName.GetBuffer(iLen),iLen);
		strFileName.ReleaseBuffer();
		// д����λ��
		pFile->Read(&ptCaret.x,sizeof(int));
		pFile->Read(&ptCaret.y,sizeof(int));
		// ������ļ�
		if (i == iCount - 1)
		{
			// ���һ�����ڣ��ָ����ģʽ
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
   
    // �õ��ĵ�ģ��
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

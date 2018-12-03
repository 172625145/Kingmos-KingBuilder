// WorkSpaceInfo.cpp: implementation of the CWorkSpaceInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ambo.h"
#include "WorkSpaceInfo.h"
#include "MainFrame.h"
#include "AmboDoc.h"
#include "AmboView.h"
#include "AmboFrame.h"
#include "SpecialView.h"

#include <atlbase.h>

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ����һ����WorkSpace
CWorkSpaceInfo::CWorkSpaceInfo()
{
	m_strWorkSpaceName = "";
	m_strWorkSpaceRoot = "";
	m_strKingmosRoot = "";
	m_strFileName = "";
//	m_uCPUType = 0;
	m_strBSPFileName = "";
	m_bBuild = FALSE;
	m_iVersion = VERSION_B;
	m_bOpen = FALSE;
//	((CAmboApp *)AfxGetApp())->SetWorkspacePath(m_strWorkSpaceRoot);
	m_iBuilderVersion = 1;
}

// ����һ��֪��WorkSpace
//CWorkSpaceInfo::CWorkSpaceInfo(CString strName,CString strRoot,UINT uType)
//CWorkSpaceInfo::CWorkSpaceInfo(CString strName,CString strRoot,CString m_strBSPName)
CWorkSpaceInfo::CWorkSpaceInfo(CString strName,CString strRoot,CString m_strBSPName,CString strPlatformRoot)
{
	m_strWorkSpaceName = strName;
	m_strWorkSpaceRoot = strRoot;
//	m_strKingmosRoot = "";
//	((CAmboApp *)AfxGetApp())->GetKingmosRoot(m_strKingmosRoot);
	m_strKingmosRoot = strPlatformRoot;
	m_strFileName = "";
//	m_uCPUType = uType;
	m_strBSPFileName = m_strBSPName;
	m_BSpConfig.LoadBSP(m_strBSPFileName);
	m_bBuild = FALSE;
	m_iVersion = VERSION_B;
	m_bOpen = TRUE;
	m_iBuilderVersion = 1;

	LoadModuleSet();
	LoadArchEnvironment();
	//((CAmboApp *)AfxGetApp())->SetWorkspacePath(m_strWorkSpaceRoot);
	SetDefaultOptionToBuild();
	GetCurrentOptionFromBuild();
	SetWorkSpaceInfoToBuild();
}

// ��һ���ļ�����һ��WorkSpace
CWorkSpaceInfo::CWorkSpaceInfo(CString strFileName)
{
	m_strWorkSpaceName = "";
	m_strWorkSpaceRoot = "";
	m_strKingmosRoot = "";
	m_strFileName = "";
	m_bBuild = FALSE;
	m_iVersion = VERSION_B;
	m_bOpen = FALSE;
	m_iBuilderVersion = 1;

	ReadInfo(strFileName);
	LoadModuleSet();
	LoadArchEnvironment();
//	((CAmboApp *)AfxGetApp())->SetWorkspacePath(m_strWorkSpaceRoot);
//	SetWorkSpaceInfoToBuild();
}

// ����һ��WorkSpace
CWorkSpaceInfo::~CWorkSpaceInfo()
{
	m_bOpen = FALSE;
}

// ����һ��WorkSpace
//BOOL CWorkSpaceInfo::Create(CString strName, CString strRoot, UINT uType)
//BOOL CWorkSpaceInfo::Create(CString strName, CString strRoot,CString m_strBSPName)
BOOL CWorkSpaceInfo::Create(CString strName, CString strRoot,CString m_strBSPName,CString strPlatformRoot)
{
	m_strWorkSpaceName = strName;
	m_strWorkSpaceRoot = strRoot;
//	m_strKingmosRoot = "";
//	((CAmboApp *)AfxGetApp())->GetKingmosRoot(m_strKingmosRoot);
	m_strKingmosRoot = strPlatformRoot;
//	m_uCPUType = uType;
	m_strBSPFileName = m_strBSPName;
	m_BSpConfig.LoadBSP(m_strBSPFileName);
	m_bBuild = FALSE;
	m_iVersion = VERSION_B;
	m_bOpen = TRUE;
	LoadModuleSet();
	LoadArchEnvironment();
//	((CAmboApp *)AfxGetApp())->SetWorkspacePath(m_strWorkSpaceRoot);
	SetDefaultOptionToBuild();
	GetCurrentOptionFromBuild();
	SetWorkSpaceInfoToBuild();
	return TRUE;
}

BOOL CWorkSpaceInfo::WriteInfo()
{
	CFile fileWorkSpace;

	if (m_strFileName.IsEmpty() == TRUE)
	{
		m_strFileName +=m_strWorkSpaceRoot;
		m_strFileName +="\\";
		m_strFileName +=m_strWorkSpaceName;
		m_strFileName +=".osw";
	}

	if (fileWorkSpace.Open(m_strFileName,CFile::modeCreate|CFile::modeWrite,NULL) == FALSE)
	{
		return FALSE;
	}

// !!! Add By Jami chen in 2004.10.25
	fileWorkSpace.Write("KOSW",4);  //  ��ǰ�ļ���KINGMOS WORKSPACE FILE
	fileWorkSpace.Write(&m_iBuilderVersion,4);  //  ��ǰ�������İ汾,�����ж��Ƿ��뵱ǰ���е�KINGBUILDER�汾һ��,������ǰ��İ汾
	// ����ƽ̨��Ŀ¼
	int iLen = m_strKingmosRoot.GetLength() +1;  //����������
	fileWorkSpace.Write(&iLen,sizeof(int));
	fileWorkSpace.Write((LPCTSTR)m_strKingmosRoot,iLen);
// !!! Add End

	fileWorkSpace.Write(&m_bBuild,4);
	fileWorkSpace.Write(&m_iVersion,4);
//	fileWorkSpace.Write(m_strBSPFileName);
	// д��Bsp File Name
	//  ���ǽ�����Ҫ�����ļ����Ϳ�����
	int iPos = m_strBSPFileName.ReverseFind('\\');
	CString strBSPFile = m_strBSPFileName.Mid(iPos +1);
	
	iLen = strBSPFile.GetLength() +1;  //����������
	fileWorkSpace.Write(&iLen,sizeof(int));
	fileWorkSpace.Write((LPCTSTR)strBSPFile,iLen);

	// ���滷������
	WriteEnvironment(&fileWorkSpace);
	WriteOption(&fileWorkSpace);
	WriteCStringArray(&fileWorkSpace,m_arrIncludeDirList);
	WriteCStringArray(&fileWorkSpace,m_arrLibraryDirList);

	// ���浱ǰ�Ѿ��򿪵��ļ�
	SaveOpenedFile(&fileWorkSpace);
	fileWorkSpace.Close();
	return TRUE;
}

BOOL CWorkSpaceInfo::ReadInfo(CString strFileName)
{
	CFile fileWorkSpace;

	if (fileWorkSpace.Open(strFileName,CFile::modeRead,NULL) == FALSE)
	{
		return FALSE;
	}

	m_strWorkSpaceName = fileWorkSpace.GetFileTitle();
	m_strWorkSpaceRoot = fileWorkSpace.GetFilePath();

	fileWorkSpace.Read(&m_bBuild,4);

	if (memcmp(&m_bBuild,"KOSW", 4) == 0)
	{
		int iBuilderVersion;
		// �����°汾
		fileWorkSpace.Read(&iBuilderVersion,4);  //  �õ����湤�̵ı������汾
		// ����ƽ̨��Ŀ¼
		CString strKingmosRoot;
		int iLen;
		fileWorkSpace.Read(&iLen,sizeof(int));
		fileWorkSpace.Read(strKingmosRoot.GetBuffer(iLen),iLen);
		strKingmosRoot.ReleaseBuffer();
		m_strKingmosRoot = strKingmosRoot;

		fileWorkSpace.Read(&m_bBuild,4);
	}
	else
	{
//		((CAmboApp *)AfxGetApp())->GetKingmosRoot(m_strKingmosRoot);
		CRegKey reg;
		CString strRoot;
		
		LONG lKey = reg.Open(HKEY_LOCAL_MACHINE, TEXT("Software\\Kingmos\\Kingmos"));
		if (lKey == ERROR_SUCCESS)
		{
			DWORD dwCount = MAX_PATH;
	//		TCHAR lpKingBuilderRoot[MAX_PATH];
			reg.QueryValue(strRoot.GetBuffer(MAX_PATH),TEXT("KINGMOS_ROOT"),&dwCount);
			reg.Close();
			strRoot.ReleaseBuffer();
			strRoot.TrimRight(' ');
		}
		else
		{

			lKey = reg.Open(HKEY_CURRENT_USER, TEXT("Software\\Kingmos\\Kingmos"));
			if (lKey == ERROR_SUCCESS)
			{
				DWORD dwCount = MAX_PATH;
		//		TCHAR lpKingBuilderRoot[MAX_PATH];
				reg.QueryValue(strRoot.GetBuffer(MAX_PATH),TEXT("KINGMOS_ROOT"),&dwCount);
				reg.Close();
				strRoot.ReleaseBuffer();
				strRoot.TrimRight(' ');
			}
		}
		m_strKingmosRoot = strRoot;
	}

	fileWorkSpace.Read(&m_iVersion,4);
//	((CAmboApp *)AfxGetApp())->ShowMessageAtDesktop(400,70,"YYYY");
	//�õ���������
	int nStart = 0, nNewPos;
	while(1)
	{
		nNewPos = m_strWorkSpaceName.Find( '.', nStart );
		if (nNewPos == -1)
			break;
		nStart = nNewPos + 1;
	}
	if (nStart != 0)
	{
		m_strWorkSpaceName.Delete(nStart-1,(m_strWorkSpaceName.GetLength() - nStart+1));
	}

	//�õ�����Ŀ¼
	nStart = 0;
	while(1)
	{
		nNewPos = m_strWorkSpaceRoot.Find( '\\', nStart );
		if (nNewPos == -1)
			break;
		nStart = nNewPos + 1;
	}
	if (nStart != 0)
	{
		m_strWorkSpaceRoot.Delete(nStart-1,(m_strWorkSpaceRoot.GetLength() - nStart +1));
	}

	m_strFileName = strFileName;

	// ��ȡBsp File Name
	//  ���ǽ�����Ҫ��ȡ���ļ���
	CString strBspFile;
	int iLen;
	fileWorkSpace.Read(&iLen,sizeof(int));
	fileWorkSpace.Read(strBspFile.GetBuffer(iLen),iLen);
	strBspFile.ReleaseBuffer();
	// Ϊ�˼�����ǰ�İ汾
	int iPos = strBspFile.ReverseFind('\\');
	CString strBSPFile = strBspFile.Mid(iPos +1);

	CString strKingBuilderRoot;
	((CAmboApp *)AfxGetApp())->GetKingbuilderRoot(strKingBuilderRoot);
	
	m_strBSPFileName = strKingBuilderRoot + "\\bspconfig\\";
	m_strBSPFileName += strBSPFile;

	m_BSpConfig.LoadBSP(m_strBSPFileName);
//	SetBSPEnvironment();

	// ��ȡ��������
	ReadEnvironment(&fileWorkSpace);
	ReadOption(&fileWorkSpace);
	
	// ��ȡҪ������Ŀ¼
	ReadCStringArray(&fileWorkSpace,m_arrIncludeDirList);
	ReadCStringArray(&fileWorkSpace,m_arrLibraryDirList);

	// ��ȡ�Ѿ��򿪵��ļ�
	ReOpenOpenedFile(&fileWorkSpace);
    fileWorkSpace.Close();
	m_bOpen = TRUE;
//	((CAmboApp *)AfxGetApp())->SetWorkspacePath(m_strWorkSpaceRoot);
	SetWorkSpaceInfoToBuild();
	return TRUE;
	// **************************************************
}
void CWorkSpaceInfo::SetBuildFlag(BOOL bBuild)
{
	m_bBuild = bBuild;
//	WriteInfo();
}
BOOL CWorkSpaceInfo::GetBuildFlag(void )
{
	return m_bBuild ;
}
BOOL CWorkSpaceInfo::Save()
{
//	WriteInfo();
	SaveWorkSpace();
	return TRUE;
}

void CWorkSpaceInfo::SetVersion(int iVersion)
{
	m_iVersion  = iVersion ;
}

int CWorkSpaceInfo::GetVersion(CString &strVersion)
{
	strVersion = "1.0.0.456";
	return 0 ;
}

void CWorkSpaceInfo::LoadModuleSet()
{
	CString strModuleSetFile;

	m_ModuleRead.Clear();
	strModuleSetFile = m_strWorkSpaceRoot + "\\script\\";
	strModuleSetFile += MODULESETFILE;
	m_ModuleRead.LoadEnvironment(strModuleSetFile);
}

void CWorkSpaceInfo::CloseWorkSpace()
{
//	m_ModuleRead.CloseModule();
	// �����Ѿ��ر��˹���
	if (m_bOpen == FALSE)
		return ;
	m_ModuleRead.Clear();
	m_ArchEnvironment.Clear();
	SaveWorkSpace();
	
	CloseAllWindow();

	// �ͷ��Ѿ�����Ŀռ�
	ReleaseEnvionment();
	ReleaseOption();
	m_bOpen = FALSE;
}

void CWorkSpaceInfo::OpenWorkSpace(CString strFileName)
{
	m_strWorkSpaceName = "";
	m_strWorkSpaceRoot = "";
	m_strKingmosRoot = "";
	m_strFileName = "";
	m_bBuild = FALSE;
	m_iVersion = VERSION_B;
	m_bOpen = FALSE;
	ReadInfo(strFileName);
	LoadModuleSet();
	LoadArchEnvironment();
}

void CWorkSpaceInfo::SaveWorkSpace()
{
	if (m_bOpen == FALSE)
		return ;
	WriteInfo();
}

int CWorkSpaceInfo::GetModuleValue(CString strModuleName)
{
	CString strCurValue;
//	return m_ModuleRead.GetModuleValue(strModuleName);
	if (m_ModuleRead.GetEnvironmentValue(strModuleName,strCurValue) == TRUE)
	{
		// �ɹ���ȡ
		if (strCurValue == "1")
			return 1;
	}
	return 0;
}

BOOL CWorkSpaceInfo::IsWorkSpaceOpen()
{
	return 	m_bOpen ;
}

						
void CWorkSpaceInfo::SetEnvironment(CString strVariable, CString strValue)
{
	int iSize;
	int nIndex;
	LPENVIRONMENTVARIABLE lpCurEnv;

	iSize = arrEnvironmentVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurEnv = (LPENVIRONMENTVARIABLE)arrEnvironmentVar.GetAt(nIndex);
		if (lpCurEnv)
		{
			if (lpCurEnv->strVariable.CompareNoCase(strVariable) == 0)
			{//�û��������Ѿ����ڣ������µ�ֵ
				lpCurEnv->strValue = strValue;
				return ;
			}
		}
		nIndex ++;
	}
	//�û������������ڣ�����µĻ�������
	lpCurEnv = new ENVIRONMENTVARIABLE;
	if (lpCurEnv)
	{
		lpCurEnv->strVariable = strVariable;
		lpCurEnv->strValue = strValue;
		arrEnvironmentVar.Add(lpCurEnv);
	}
}

void CWorkSpaceInfo::WriteEnvironment(CFile *pFile)
{
	int iSize;
	int nIndex,iLen;
	LPENVIRONMENTVARIABLE lpCurEnv;

	iSize = arrEnvironmentVar.GetSize();
	// д�������Ŀ
	pFile->Write(&iSize,sizeof(int));
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurEnv = (LPENVIRONMENTVARIABLE)arrEnvironmentVar.GetAt(nIndex);
		if (lpCurEnv)
		{	// д�������
			iLen = lpCurEnv->strVariable.GetLength() +1;  //����������
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)lpCurEnv->strVariable,iLen);
			
			// д�����ֵ
			
			iLen = lpCurEnv->strValue.GetLength() +1;  //����������
			pFile->Write(&iLen,sizeof(int));
			pFile->Write((LPCTSTR)lpCurEnv->strValue,iLen);
		}
		nIndex ++;
	}

}

void CWorkSpaceInfo::ReadEnvironment(CFile *pFile)
{
	int iSize;
	int nIndex,iLen;
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
		
		pFile->Read(&iLen,sizeof(int));
		pFile->Read(strValue.GetBuffer(iLen),iLen);
		strValue.ReleaseBuffer();
		SetEnvironment(strVariable, strValue);
		nIndex ++;
	}

}

void CWorkSpaceInfo::SetOption(CString strVariable, CString strValue)
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

void CWorkSpaceInfo::SetOption(CString strVariable, int iValue)
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

void CWorkSpaceInfo::GetOption(CString strVariable, CString &strValue)
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
					// �ñ���������һ��INT��ֵ
					strValue = "";
				}
				return ;
			}
		}
		nIndex ++;
	}
}

void CWorkSpaceInfo::GetOption(CString strVariable, int &iValue)
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

void CWorkSpaceInfo::ReleaseEnvionment()
{
	int iSize, nIndex;
	LPENVIRONMENTVARIABLE lpCurEnv;
	iSize = arrEnvironmentVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurEnv = (LPENVIRONMENTVARIABLE)arrEnvironmentVar.GetAt(nIndex);
		if (lpCurEnv)
		{
			delete lpCurEnv;
		}
		nIndex ++;
	}
	arrEnvironmentVar.RemoveAll();
}

void CWorkSpaceInfo::ReleaseOption()
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

void CWorkSpaceInfo::ReadOption(CFile *pFile)
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

void CWorkSpaceInfo::WriteOption(CFile *pFile)
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


void CWorkSpaceInfo::SetWorkSpaceInfoToBuild()
{
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
		CString strCPUType,strBSPPlatformName;

			m_BSpConfig.GetCPUType(strCPUType);
			//m_BSpConfig.GetBSPName(strBSPName);
			m_BSpConfig.GetPlatformName(strBSPPlatformName);

			pMainFrame->m_outBar.m_BuildClass.SetKingmosPath(m_strKingmosRoot);		//����kingmos·��

			pMainFrame->m_outBar.m_BuildClass.SetBuildForDefault();  // Clear The other project Setting

			pMainFrame->m_outBar.m_BuildClass.SetWorkType(TRUE);
			pMainFrame->m_outBar.m_BuildClass.SetPlatformCpuType(strCPUType);	//  CPU ����
			pMainFrame->m_outBar.m_BuildClass.SetPlatformName(m_strWorkSpaceName);
			pMainFrame->m_outBar.m_BuildClass.SetPlatformLocation(m_strWorkSpaceRoot);
			pMainFrame->m_outBar.m_BuildClass.SetPlatformBspName(strBSPPlatformName); // arch �µ�����

			SetBSPEnvironment(m_strBSPFileName);
			SetCurrentOptionToBuild();
}

void CWorkSpaceInfo::WriteCStringArray(CFile *pFile, CStringArray &arrString)
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

void CWorkSpaceInfo::ReadCStringArray(CFile *pFile, CStringArray &arrString)
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

void CWorkSpaceInfo::GetCurrentOptionFromBuild()
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

// ����Ĭ�ϵ�Option,���½����̵�ʱ����Ҫ
void CWorkSpaceInfo::SetDefaultOptionToBuild()
{
	// ��ϵͳ���õ�Default״̬
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		ASSERT(pMainFrame);
		pMainFrame->m_outBar.m_BuildClass.SetBuildForDefault(); // Load Default Setting

		// ���Ĭ�ϵ�Include Path
		CString strPath;
		CString strBSPPlatformName;
		
		strPath.Format("%s\\include",m_strKingmosRoot); // kingmos\include
		m_arrIncludeDirList.Add(strPath);
		strPath.Format("%s\\drivers\\include",m_strKingmosRoot); // kingmos\drivers\include
		m_arrIncludeDirList.Add(strPath);

		m_BSpConfig.GetPlatformName(strBSPPlatformName);
		strPath.Format("%s\\arch\\%s\\include",m_strKingmosRoot,strBSPPlatformName); // kingmos\drivers\include

/*		strPath.Format("%s\\include","$KINGMOSROOT"); // kingmos\include
		m_arrIncludeDirList.Add(strPath);
		strPath.Format("%s\\drivers\\include","$KINGMOSROOT"); // kingmos\drivers\include
		m_arrIncludeDirList.Add(strPath);

		m_BSpConfig.GetPlatformName(strBSPPlatformName);
		strPath.Format("%s\\arch\\%s\\include","$KINGMOSROOT",strBSPPlatformName); // kingmos\drivers\include
*/
		m_arrIncludeDirList.Add(strPath);

		// Copy Platform Config File To WorkSpace
		//CopyPlatformConfigFile();
}
void CWorkSpaceInfo::SetCurrentOptionToBuild()
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

		RefreshIncludePath();
		RefreshLibraryPath();

		// ������Ҫ���û�������
		SetEnvironmentToBuild();
}

void CWorkSpaceInfo::RefreshIncludePath()
{
	CString csInclude;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);

	InsertPathListToString(csInclude,m_arrIncludeDirList);
	pMainFrame->m_outBar.m_BuildClass.SetProjectInclude(csInclude);	//����includes path
}
void CWorkSpaceInfo::RefreshLibraryPath()
{
	CString csLibpath;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);

	InsertPathListToString(csLibpath,m_arrLibraryDirList);
	pMainFrame->m_outBar.m_BuildClass.SetProjectLibpath(csLibpath);	//����libs path
}

void CWorkSpaceInfo::InsertPathListToString(CString &strList, CStringArray &arrString)
{
	int nIndex;
	int iSize = arrString.GetSize();
	CString strString;
	
	nIndex = 0;
	while(nIndex < iSize)
	{
		strString = arrString.GetAt(nIndex);
		if (!strString.IsEmpty())		
		{
//			strString.Replace("$KINGMOSROOT",m_strKingmosRoot);
			strList += strString;
			strList += " ";
		}
		nIndex ++;
	}
}

void CWorkSpaceInfo::SetEnvironmentToBuild()
{
	int iSize;
	int nIndex;
	LPENVIRONMENTVARIABLE lpCurEnv;
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);

	//����ɵĻ�������
	pMainFrame->m_outBar.m_BuildClass.ClearPlatformEnvironment();
	iSize = arrEnvironmentVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurEnv = (LPENVIRONMENTVARIABLE)arrEnvironmentVar.GetAt(nIndex);
		if (lpCurEnv)
		{	// ����һ����������
			pMainFrame->m_outBar.m_BuildClass.SetPlatformEnvironment(lpCurEnv->strVariable,lpCurEnv->strValue);
		}
		nIndex ++;
	}
}

void CWorkSpaceInfo::ClearEnvionment()
{
	ReleaseEnvionment();
}

void CWorkSpaceInfo::GetPlatformBspName(CString &strBSPPlatformName)
{
	m_BSpConfig.GetPlatformName(strBSPPlatformName);
}

void CWorkSpaceInfo::CopyPlatformConfigFile()
{
	CString strKingmosConfig,strWorkSpaceConfig;

	strWorkSpaceConfig = m_strWorkSpaceRoot + "\\Config";
	CreateDirectory(strWorkSpaceConfig,NULL);
	strWorkSpaceConfig += "\\";

	((CAmboApp *)AfxGetApp())->GetKingmosRoot(strKingmosConfig);
	strKingmosConfig += "\\Config\\";

	CString strKingmosFile,strWorkSpaceFile;

	// Kingmos\config\memory.con  --> WorkSpace\Config\memory.con
	strKingmosFile =strKingmosConfig + "memory.con"; 
	strWorkSpaceFile = strWorkSpaceConfig + "memory.con"; 
	CopyFile(strKingmosFile,strWorkSpaceFile,FALSE);

	// Kingmos\config\Platform.mod  --> WorkSpace\Config\Platform.mod
	strKingmosFile =strKingmosConfig + "Platform.mod"; 
	strWorkSpaceFile = strWorkSpaceConfig + "Platform.mod"; 
	CopyFile(strKingmosFile,strWorkSpaceFile,FALSE);

	// Kingmos\config\Platform.dat  --> WorkSpace\Config\Platform.dat
	strKingmosFile =strKingmosConfig + "Platform.dat"; 
	strWorkSpaceFile = strWorkSpaceConfig + "Platform.dat"; 
	CopyFile(strKingmosFile,strWorkSpaceFile,FALSE);

	// Kingmos\config\Platform.reg  --> WorkSpace\Config\Platform.reg
	strKingmosFile =strKingmosConfig + "Platform.reg"; 
	strWorkSpaceFile = strWorkSpaceConfig + "Platform.reg"; 
	CopyFile(strKingmosFile,strWorkSpaceFile,FALSE);

	// Kingmos\config\Common.reg  --> WorkSpace\Config\Common.reg
	strKingmosFile =strKingmosConfig + "Common.reg"; 
	strWorkSpaceFile = strWorkSpaceConfig + "Common.reg"; 
	CopyFile(strKingmosFile,strWorkSpaceFile,FALSE);

	// Kingmos\config\Common.mod  --> WorkSpace\Config\Common.mod
	strKingmosFile =strKingmosConfig + "Common.mod"; 
	strWorkSpaceFile = strWorkSpaceConfig + "Common.mod"; 
	CopyFile(strKingmosFile,strWorkSpaceFile,FALSE);
}

void CWorkSpaceInfo::GetSystemEnvironmentValue(CString strEnvVar, CString &strValue)
{
	CString strCurValue = "";
	strValue = strCurValue;
	//  First Module Set
	if (m_ModuleRead.GetEnvironmentValue(strEnvVar,strCurValue) == TRUE)
	{
		// �ɹ���ȡ
		strValue = strCurValue;
	}

	// Second Arch Environment
	if (m_ArchEnvironment.GetEnvironmentValue(strEnvVar,strCurValue) == TRUE)
	{
		// �ɹ���ȡ
		strValue = strCurValue;
	}

	// Last WorkSpace Emvironment
	if (GetWorkSpaceEnvironment(strEnvVar, strCurValue) == TRUE)
	{
		// �ɹ���ȡ
		strValue = strCurValue;
	}
}

void CWorkSpaceInfo::LoadArchEnvironment()
{
	CString strModuleSetFile;
	CString strBSPPlatformName;

	strModuleSetFile = m_strKingmosRoot + "\\";
	strModuleSetFile += "arch\\";
	m_BSpConfig.GetPlatformName(strBSPPlatformName);
	strModuleSetFile += strBSPPlatformName;
	strModuleSetFile += "\\";
	strModuleSetFile += strBSPPlatformName;
	strModuleSetFile += ".bat";
	m_ArchEnvironment.LoadEnvironment(strModuleSetFile);
}

BOOL CWorkSpaceInfo::GetWorkSpaceEnvironment(CString strVariable, CString &strValue)
{
	int iSize;
	int nIndex;
	LPENVIRONMENTVARIABLE lpCurEnv;

	iSize = arrEnvironmentVar.GetSize();
	nIndex = 0;
	while(nIndex < iSize)
	{
		lpCurEnv = (LPENVIRONMENTVARIABLE)arrEnvironmentVar.GetAt(nIndex);
		if (lpCurEnv)
		{
			if (lpCurEnv->strVariable.CompareNoCase(strVariable) == 0)
			{//�û��������Ѿ����ڣ������µ�ֵ
				strValue = lpCurEnv->strValue;
				return TRUE;
			}
		}
		nIndex ++;
	}
	return FALSE;
}

void CWorkSpaceInfo::SetBSPEnvironment(CString strBSPName)
{
	//  ���ǽ�����Ҫ�����ļ�������Ϳ�����
	int iPos = strBSPName.ReverseFind('\\');
	CString strBSPFile = strBSPName.Mid(iPos +1);
	iPos = strBSPFile.ReverseFind('.');
	if (iPos != -1)
	{
		strBSPFile = strBSPFile.Left(iPos);
	}
	
	CString strBSPEnvironment = strBSPFile + "_BSP";
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	ASSERT(pMainFrame);
	strBSPEnvironment.MakeUpper();
	pMainFrame->m_outBar.m_BuildClass.SetVariable(strBSPEnvironment,1);
}

void CWorkSpaceInfo::RefreshModuleSet()
{
	LoadModuleSet();
	LoadArchEnvironment();
}

void CWorkSpaceInfo::SaveOpenedFile(CFile *pFile)
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
	/*
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
	*/
	iCount = AfxGetApp()->GetOpenDocumentCount();
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
			{
				iLen = 0;  // û������
				pFile->Write(&iLen,sizeof(int));
				continue;
			}
			pView = (CSpecialView*)pDoc->GetNextView(pos); // get first one
			if (pView == NULL)
			{
				iLen = 0;  // û������
				pFile->Write(&iLen,sizeof(int));
				continue;
			}
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
			{
				iLen = 0;  // û������
				pFile->Write(&iLen,sizeof(int));
				return;
			}
			pView = (CSpecialView*)pActiveDoc->GetNextView(pos); // get first one
			if (pView == NULL)
			{
				iLen = 0;  // û������
				pFile->Write(&iLen,sizeof(int));
				return;
			}
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

void CWorkSpaceInfo::ReOpenOpenedFile(CFile *pFile)
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
		iLen = 0;
		strFileName = "";
		pFile->Read(&iLen,sizeof(int));
		if (iLen == 0)
			continue;
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
		if (strFileName.IsEmpty() == FALSE)
		{
			if (AfxGetApp()->OpenDocumentFile(strFileName) != NULL)
			{
				((CAmboApp *)AfxGetApp())->SetCaretPos(ptCaret);
			}
		}
	}
}

void CWorkSpaceInfo::CloseAllWindow()
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

void CWorkSpaceInfo::GetKingmosRoot(CString &strKingmosRoot)
{
	strKingmosRoot = m_strKingmosRoot;
}

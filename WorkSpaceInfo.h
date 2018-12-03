// WorkSpaceInfo.h: interface for the CWorkSpaceInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEINFO_H__947ABF01_88D0_4996_B4EB_5C6BFB2ABA66__INCLUDED_)
#define AFX_WORKSPACEINFO_H__947ABF01_88D0_4996_B4EB_5C6BFB2ABA66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define IMAGE_MDL_ROOT					0
#define IMAGE_MDL_GROUP                 1
#define IMAGE_MDL_LIST	                2
#define IMAGE_MDL_MODULE                3
#define IMAGE_MDL_DIRECTORY             4
#define IMAGE_MDL_SOURCE	            5

#define VERSION_B		0
#define VERSION_C		1

typedef struct 
{
	CString strVariable;
	CString strValue;
}ENVIRONMENTVARIABLE,*LPENVIRONMENTVARIABLE;


//#define IMAGE_MDL_DYNLINK               1
//#define IMAGE_MDL_LIBRARY               2
//#include "ModuleRead.h"
#include "BSPConfig.h"
#include "EnvironmentSet.h"

class CWorkSpaceInfo  
{
public:
	void GetKingmosRoot(CString &strKingmosRoot);
	void CloseAllWindow();
	void RefreshModuleSet();
	BOOL GetWorkSpaceEnvironment(CString strVariable, CString &strValue);
	void GetSystemEnvironmentValue(CString strEnvVar, CString &strValue);
	void CopyPlatformConfigFile();
	void GetPlatformBspName(CString &strBSPPlatformName);
	void ClearEnvionment();
	void SetEnvironmentToBuild();

	void ReadCStringArray(CFile *pFile, CStringArray &arrString);
	void WriteCStringArray(CFile *pFile,CStringArray &arrString);

	void WriteOption(CFile *pFile);
	void ReadOption(CFile *pFile);
	void ReleaseOption();
	void ReleaseEnvionment();
	void GetOption(CString strVariable, int &iValue);
	void GetOption(CString strVariable, CString &strValue);
	void SetOption(CString strVariable, int iValue);
	void SetOption(CString strVariable, CString strValue);
	// Get and Set Option from Build System
	void SetDefaultOptionToBuild();
	void GetCurrentOptionFromBuild();
	void SetCurrentOptionToBuild();

	void RefreshIncludePath();
	void RefreshLibraryPath();
	
	void ReadEnvironment(CFile *pFile);
	void WriteEnvironment(CFile *pFile);
	void SetEnvironment(CString strVariable,CString strValue);
	BOOL IsWorkSpaceOpen();
	int GetModuleValue(CString strModuleName);
	void SaveWorkSpace();
	void OpenWorkSpace(CString strFileName);
	void CloseWorkSpace();
	int GetVersion(CString &strVersion);
	void SetVersion(int iVersion);
//	BOOL Create(CString strName,CString strRoot,UINT uType);
	BOOL Create(CString strName,CString strRoot,CString m_strBSPName,CString strPlatformRoot);
	CWorkSpaceInfo();
	CWorkSpaceInfo(CString strFileName);
//	CWorkSpaceInfo(CString strName,CString strRoot,UINT uType);
//	CWorkSpaceInfo(CString strName,CString strRoot,CString m_strBSPName);
	CWorkSpaceInfo(CString strName,CString strRoot,CString m_strBSPName,CString strPlatformRoot);
	virtual ~CWorkSpaceInfo();
	void SetBuildFlag(BOOL bBuild);
	BOOL GetBuildFlag(void );
	BOOL Save(void);

	CString m_strFileName;    // 当前打开的工程文件的文件名
	CString m_strWorkSpaceName;  // 工程文件名
	CString m_strWorkSpaceRoot;  // 工程文件路径
	CString m_strKingmosRoot;  // Kingmos 路径
	CString m_strBSPFileName;  // 当前工程所使用的BSP 文件

	CPtrArray arrEnvironmentVar;  //存放当前所设置的环境变量
	
	CStringArray m_arrIncludeDirList;  //存放当前设定的INCLUDE目录
	CStringArray m_arrLibraryDirList;  //存放当前设定的LIBRARY目录

private:
	void ReOpenOpenedFile(CFile *pFile);
	void SaveOpenedFile(CFile *pFile);
	void SetBSPEnvironment(CString strBSPPlatformName);
	void LoadArchEnvironment();
	void InsertPathListToString(CString &strList, CStringArray &arrString);

	void LoadModuleSet();
	BOOL ReadInfo(CString strFileName);
	BOOL WriteInfo();
	void SetWorkSpaceInfoToBuild();

	BOOL m_bBuild;
//	UINT m_uCPUType;
	BOOL m_bOpen;

	int m_iVersion ;

	int m_iBuilderVersion;

	CEnvironmentSet m_ModuleRead;
	CEnvironmentSet m_ArchEnvironment;
	CPtrArray arrOptionVar;  //存放当前所设置的环境变量


	// BSP File 内容
	CBSPConfig m_BSpConfig;
};

#endif // !defined(AFX_WORKSPACEINFO_H__947ABF01_88D0_4996_B4EB_5C6BFB2ABA66__INCLUDED_)

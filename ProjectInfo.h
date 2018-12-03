// ProjectInfo.h: interface for the CProjectInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTINFO_H__CF1D315D_687B_4D48_AA93_56A4F4B08EDB__INCLUDED_)
#define AFX_PROJECTINFO_H__CF1D315D_687B_4D48_AA93_56A4F4B08EDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PROJECT_ROOT			0
#define PROJECT_FOLDER			1
#define PROJECT_FILE			2

#define PROGRAM_EXE				0
#define PROGRAM_DLL				1
#define PROGRAM_LIB				2

#include "srcdata.h"

typedef struct projectItem{
	CString strShowString; // 要显示的字串
	CString strFileName; // 关联的文件名
	int iItemType;  //这是个文件夹或者文件   PROJECT_FOLDER or PROJECT_FILE
//	struct projectItem * lpChild ;// 他的子项
	CPtrArray arrChild ;// 他的子项
}PROJECTITEM, *LPPROJECTITEM;

class CProjectInfo  
{
public:
	void CloseAllWindow();
	void Save();
	CProjectInfo();
	CProjectInfo(CString strProjectName);
	virtual ~CProjectInfo();

	void DeleteSubProjectItem(LPPROJECTITEM lpParentItem,LPPROJECTITEM lpProjectItem);
	void DeleteCurFileFromSpecialFileList(CString strFileName);

	void RefreshSourceFile();
	void RefreshLibraryFile();
	void RefreshIncludePath();
	void RefreshLibraryPath();

	void ReadCStringArray(CFile *pFile, CStringArray &arrString);
	void WriteCStringArray(CFile *pFile,CStringArray &arrString);

	BOOL Create(CString strProjectName, CString strProjectType,CString strLocation, CString strCPUType);
	BOOL Create(CString strModulesName, CString strCPUType);
	BOOL IsProjectOpen();
	BOOL OpenProject(CString strFileName);
	BOOL SaveProject();
	BOOL CloseProject();

	void WriteOption(CFile *pFile);
	void ReadOption(CFile *pFile);
	void ReleaseOption();
	// Get and Set Option from ProjectInfo
	void GetOption(CString strVariable, int &iValue);
	void GetOption(CString strVariable, CString &strValue);
	void SetOption(CString strVariable, int iValue);
	void SetOption(CString strVariable, CString strValue);

	// Get and Set Option from Build System
	void SetDefaultOptionToBuild();
	void GetCurrentOptionFromBuild();
	void SetCurrentOptionToBuild();


	LPPROJECTITEM InsertChild(LPPROJECTITEM lpProjectItem,CString strChild,int iItemType); // 插入一个条目，返回新插入的条目指针
	LPPROJECTITEM SearchProjectItem(LPPROJECTITEM lpProjectItem,CString strShowString);

	CString m_strFileName;  // 工程文件的文件名
	CString m_strProjectName; // 工程名称
	CString m_strProjectRoot;  // 工程路径

	CStringArray m_arrIncludeDirList;  //存放当前设定的INCLUDE目录
	CStringArray m_arrLibraryDirList;  //存放当前设定的LIBRARY目录

	CStringArray m_arrSourceFileList;  //存放当前设定的Source文件  c,cpp

	CStringArray m_arrLibFileList;  //存放当前设定的LIBRARY文件  a
	CStringArray m_arrDllFileList;  //存放当前设定的dynamic link library文件  sp
	CStringArray m_arrObjFileList;  //存放当前设定的LIBRARY文件  o

	LPPROJECTITEM m_root;

//	int m_iProgramType ; // EXE DLL LIB
	CString m_strProgramType ; // EXE DLL LIB

	CPtrArray arrFindText;  // 存放已经使用过的查找字串
	CPtrArray arrFindFolder;  // 存放已经使用过的查找目录

private:
	void ReOpenOpenedFile(CFile *pFile);
	void SaveOpenedFile(CFile *pFile);
	void GetFullFileName(CString csFile,CString &csFullName);
	void GetModuleDir(CString strModulesName,CString &strLocation);
	BOOL AnalysisModule();
	BOOL ReadModule(CString strModuleFile);
	void LoadDefaultProjectInfo();
	int	InsertCurFileToSpecialFileList(CString strFileName);
	void InsertPathListToString(CString &strList, CStringArray &arrString);
//	void InsertFileString(LPPROJECTITEM lpSourceItem,CString &csFile);  // 将指定条目下的文件添加到指定的串中
//	void InsertLibToFileString(LPPROJECTITEM lpSourceItem,CString &csLibFile,CString &csDllFile,CString &csObjFile);  // 将指定条目下的文件添加到指定的串中
	int GetFileType(CString strFileName);
	BOOL WriteInfo();    // 将当前的工程写入到文件中
	void MakeModules();
	BOOL ReadInfo(CString strProjectName);  // 读取工程文件
	BOOL WriteProjectItem(CFile *pFile,LPPROJECTITEM lpProjectItem);
	BOOL ReadProjectItem(CFile *pFile,LPPROJECTITEM lpProjectItem,LPPROJECTITEM lpParent);
	void DeleteProjectItem(LPPROJECTITEM lpProjectItem);
	void GetShowString(LPPROJECTITEM lpProjectItem);
	LPPROJECTITEM FileNameIsExist(LPPROJECTITEM lpProjectItem,CString strFileName);

	void SetProjectInfoToBuild();
//	UINT m_uCPUType;  // 当前工程支持的CPU类型
	CString m_strCPUType;
	BOOL m_bOpen;   // 当前工程是否被打开
	CPtrArray arrOptionVar;  //存放当前所设置的环境变量

	CsrcData m_srcData;


};

#endif // !defined(AFX_PROJECTINFO_H__CF1D315D_687B_4D48_AA93_56A4F4B08EDB__INCLUDED_)

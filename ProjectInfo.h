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
	CString strShowString; // Ҫ��ʾ���ִ�
	CString strFileName; // �������ļ���
	int iItemType;  //���Ǹ��ļ��л����ļ�   PROJECT_FOLDER or PROJECT_FILE
//	struct projectItem * lpChild ;// ��������
	CPtrArray arrChild ;// ��������
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


	LPPROJECTITEM InsertChild(LPPROJECTITEM lpProjectItem,CString strChild,int iItemType); // ����һ����Ŀ�������²������Ŀָ��
	LPPROJECTITEM SearchProjectItem(LPPROJECTITEM lpProjectItem,CString strShowString);

	CString m_strFileName;  // �����ļ����ļ���
	CString m_strProjectName; // ��������
	CString m_strProjectRoot;  // ����·��

	CStringArray m_arrIncludeDirList;  //��ŵ�ǰ�趨��INCLUDEĿ¼
	CStringArray m_arrLibraryDirList;  //��ŵ�ǰ�趨��LIBRARYĿ¼

	CStringArray m_arrSourceFileList;  //��ŵ�ǰ�趨��Source�ļ�  c,cpp

	CStringArray m_arrLibFileList;  //��ŵ�ǰ�趨��LIBRARY�ļ�  a
	CStringArray m_arrDllFileList;  //��ŵ�ǰ�趨��dynamic link library�ļ�  sp
	CStringArray m_arrObjFileList;  //��ŵ�ǰ�趨��LIBRARY�ļ�  o

	LPPROJECTITEM m_root;

//	int m_iProgramType ; // EXE DLL LIB
	CString m_strProgramType ; // EXE DLL LIB

	CPtrArray arrFindText;  // ����Ѿ�ʹ�ù��Ĳ����ִ�
	CPtrArray arrFindFolder;  // ����Ѿ�ʹ�ù��Ĳ���Ŀ¼

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
//	void InsertFileString(LPPROJECTITEM lpSourceItem,CString &csFile);  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
//	void InsertLibToFileString(LPPROJECTITEM lpSourceItem,CString &csLibFile,CString &csDllFile,CString &csObjFile);  // ��ָ����Ŀ�µ��ļ���ӵ�ָ���Ĵ���
	int GetFileType(CString strFileName);
	BOOL WriteInfo();    // ����ǰ�Ĺ���д�뵽�ļ���
	void MakeModules();
	BOOL ReadInfo(CString strProjectName);  // ��ȡ�����ļ�
	BOOL WriteProjectItem(CFile *pFile,LPPROJECTITEM lpProjectItem);
	BOOL ReadProjectItem(CFile *pFile,LPPROJECTITEM lpProjectItem,LPPROJECTITEM lpParent);
	void DeleteProjectItem(LPPROJECTITEM lpProjectItem);
	void GetShowString(LPPROJECTITEM lpProjectItem);
	LPPROJECTITEM FileNameIsExist(LPPROJECTITEM lpProjectItem,CString strFileName);

	void SetProjectInfoToBuild();
//	UINT m_uCPUType;  // ��ǰ����֧�ֵ�CPU����
	CString m_strCPUType;
	BOOL m_bOpen;   // ��ǰ�����Ƿ񱻴�
	CPtrArray arrOptionVar;  //��ŵ�ǰ�����õĻ�������

	CsrcData m_srcData;


};

#endif // !defined(AFX_PROJECTINFO_H__CF1D315D_687B_4D48_AA93_56A4F4B08EDB__INCLUDED_)

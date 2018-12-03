// ModuleRead.h: interface for the CModuleRead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULEREAD_H__82FFA05D_A1FF_4808_852E_A49A6A365C31__INCLUDED_)
#define AFX_MODULEREAD_H__82FFA05D_A1FF_4808_852E_A49A6A365C31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct {
	CString VarName;
	int     iValue;
}MODULEVAR,*LPMODULEVAR;

typedef struct {
	CString strModuleName;
	LPMODULEVAR lpModuleVar;
	CPtrArray supportArray;
}DEFMODULE,*LPDEFMODULE;

typedef struct FeatureStruct {
	CString strFeatureName;
	CPtrArray moduleArray;
	struct FeatureStruct *next;
	struct FeatureStruct *child;
}FEATURESTRUCT,*LPFEATURESTRUCT;

class CModuleRead  
{

public:
	BOOL IsExistDirectory(CString strDirectory);
	int GetModuleValue(CString strModuleName);
	void LoadCurModuleState(CString strFileName);
	void SaveCurModuleState(CString strFileName);
	void CloseModule();
	void LoadModule(CString strFileName);
	CModuleRead();
	virtual ~CModuleRead();

	LPFEATURESTRUCT m_root;

private:
	void SaveModuleSet(CString strFileName);
	void InsertFeatureToMoudleSet(LPFEATURESTRUCT lpCurFeature);
	void SetModuleVar(LPMODULEVAR lpModuleVar,int iValue);
	void InsertModuleDefine(LPDEFMODULE lpModuleDefine);
	LPMODULEVAR InsertModuleVar(LPMODULEVAR lpModuleVar);
	void DeleteFeature(LPFEATURESTRUCT lpCurFeature);
	void DeleteModuleDefine(LPDEFMODULE lpModuleDefine);
	LPDEFMODULE GetModuleDefine(CString line);
	LPFEATURESTRUCT InsertFeature(CString line);
	int GetWord(CString line,int nStart,CString &strWord);
	LPFEATURESTRUCT FeatureExist(LPFEATURESTRUCT lpCurFeature,CString strFeatureName);
	BOOL ReadLine(CString &line);
	void LoadFile(CString strFileName);

	CFile m_ModuleFile;
	LPTSTR m_lpFileData;
	LPTSTR m_lpCurPos;
	DWORD m_dwFileSize;
	CPtrArray m_aModuleSetArray;

};

#endif // !defined(AFX_MODULEREAD_H__82FFA05D_A1FF_4808_852E_A49A6A365C31__INCLUDED_)

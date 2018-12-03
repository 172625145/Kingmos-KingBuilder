// SetEnv.cpp : implementation file
//

#include "stdafx.h"
#include "ambo.h"
#include "SetEnv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetEnv dialog


CSetEnv::CSetEnv(CWnd* pParent /*=NULL*/)
	: CDialog(CSetEnv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetEnv)
	m_strVariable = _T("");
	m_strValue = _T("");
	//}}AFX_DATA_INIT
}


void CSetEnv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetEnv)
	DDX_Text(pDX, IDC_EDIT1, m_strVariable);
	DDX_Text(pDX, IDC_EDIT2, m_strValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetEnv, CDialog)
	//{{AFX_MSG_MAP(CSetEnv)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetEnv message handlers

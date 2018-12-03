// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E8A44B3F_4EAF_4B33_B654_728016096B26__INCLUDED_)
#define AFX_STDAFX_H__E8A44B3F_4EAF_4B33_B654_728016096B26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
//#include <afxdisp.h>        // MFC Automation classes
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcview.h>		// MFC Views

#include "mlViewTab.h"	// Added by ClassView
//#include "../mlClass/mlControlBar.h" // Added by ClassView
#include "mlControlBar.h" // Added by ClassView
//#include "mlBuildView.h"
#include "resource.h"

//++++++++++++++++++++++
#define IDW_AMBOCONTROLBAR_FIRST	(AFX_IDW_CONTROLBAR_FIRST+33)
#define IDW_AMBOWORKBAR				(IDW_AMBOCONTROLBAR_FIRST+1)
#define IDW_AMBOOUTBAR				(IDW_AMBOCONTROLBAR_FIRST+2)
#define IDW_MAINTOOLBAR				(IDW_AMBOCONTROLBAR_FIRST+3)
#define IDW_EDITBAR					(IDW_AMBOCONTROLBAR_FIRST+4)
#define IDW_BUILDBAR				(IDW_AMBOCONTROLBAR_FIRST+5)
//----------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E8A44B3F_4EAF_4B33_B654_728016096B26__INCLUDED_)

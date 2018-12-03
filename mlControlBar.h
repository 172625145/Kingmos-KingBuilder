#if !defined(AFX_MLCONTROLBAR_H__3AC509E3_BC2E_48CF_A756_92B447DDA8D7__INCLUDED_)
#define AFX_MLCONTROLBAR_H__3AC509E3_BC2E_48CF_A756_92B447DDA8D7__INCLUDED_

#include <afxpriv.h>    // for CDockContext
#include <afxtempl.h>   // for CTypedPtrArray

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mlControlBar.h : header file
//

/////////////////////////////////////////////////////////////////////////
// CmlDockBar dummy class for access to protected members

class  CmlDockBar : public CDockBar
{
    friend class CmlControlBar;
};

class CmlButton
{
public:
    CmlButton();

    void Move(CPoint ptTo) {ptOrg = ptTo; };
    CRect GetRect() { return CRect(ptOrg, CSize(11, 11)); };
    void Paint(CDC* pDC);

    BOOL    bPushed;
    BOOL    bRaised;

protected:
    CPoint  ptOrg;
};

/////////////////////////////////////////////////////////////////////////
// CmlControlBar control bar styles

#define SCBS_EDGELEFT       0x00000001
#define SCBS_EDGERIGHT      0x00000002
#define SCBS_EDGETOP        0x00000004
#define SCBS_EDGEBOTTOM     0x00000008
#define SCBS_EDGEALL        0x0000000F
#define SCBS_SHOWEDGES      0x00000010
#define SCBS_SIZECHILD      0x00000020

/////////////////////////////////////////////////////////////////////////
// CmlControlBar control bar

class CmlControlBar;
typedef CTypedPtrArray <CPtrArray, CmlControlBar*> CmlCBArray;

class CmlControlBar : public CControlBar
{
// Construction
public:
    CmlControlBar();

 //   virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
 //       CSize sizeDefault, BOOL bHasGripper,
 //       UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);
    virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
        UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);

// Attributes
public:
    const BOOL IsFloating() const;
    const BOOL IsHorzDocked() const;
    const BOOL IsVertDocked() const;
    const BOOL IsSideTracking() const;
    const BOOL GetSCBStyle() const {return m_dwSCBStyle;}

// Operations
public:

    void EnableDocking(DWORD dwDockStyle);

    virtual BOOL HasGripper() const;

    virtual void LoadState(LPCTSTR lpszProfileName);
    virtual void SaveState(LPCTSTR lpszProfileName);
    static void GlobalLoadState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
    static void GlobalSaveState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
    void SetSCBStyle(DWORD dwSCBStyle)
        {m_dwSCBStyle = (dwSCBStyle & ~SCBS_EDGEALL);}

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CmlControlBar)
    public:
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CmlControlBar();
    
protected:
    // implementation helpers
    UINT GetEdgeHTCode(int nEdge);
    BOOL GetEdgeRect(CRect rcWnd, UINT nHitTest, CRect& rcEdge);
    virtual void StartTracking(UINT nHitTest);
    virtual void StopTracking();
    virtual void OnTrackUpdateSize(CPoint& point);
    virtual void OnTrackInvertTracker();
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
    virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

    virtual void AlignControlBars();
    void GetRowInfo(int& nFirst, int& nLast, int& nThis);
    void GetRowSizingBars(CmlCBArray& arrSCBars);
    BOOL NegotiateSpace(int nLengthTotal, BOOL bHorz);

protected:
    DWORD   m_dwSCBStyle;
    UINT    m_htEdge;

    CSize   m_szHorz;
    CSize   m_szVert;
    CSize   m_szFloat;
    CSize   m_szMinHorz;
    CSize   m_szMinVert;
    CSize   m_szMinFloat;
    CSize   m_szMinT;
    CSize   m_szMaxT;
    CSize   m_szOld;
    CPoint  m_ptOld;
    BOOL    m_bTracking;
    BOOL    m_bKeepSize;
    BOOL    m_bParentSizing;
    BOOL    m_bDragShowContent;
    UINT    m_nDockBarID;
    int     m_cxEdge;

	
    int     m_cyGripper;

    CmlButton m_biHide;

// Generated message map functions
protected:
    //{{AFX_MSG(CmlControlBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnNcPaint();
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
    afx_msg UINT OnNcHitTest(CPoint point);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnPaint();
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

    friend class CmlMiniDockFrameWnd;
};

/////////////////////////////////////////////////////////////////////////
// CmlDockContext dockcontext

class CmlDockContext : public CDockContext
{
public:
// Construction
    CmlDockContext(CControlBar* pBar) : CDockContext(pBar) {}

// Drag Operations
    virtual void StartDrag(CPoint pt);
};

/////////////////////////////////////////////////////////////////////////
// CmlMiniDockFrameWnd miniframe

class CmlMiniDockFrameWnd : public CMiniDockFrameWnd
{
 
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CmlMiniDockFrameWnd)
    public:
    virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
    //}}AFX_VIRTUAL

// Implementation
public:
    CmlControlBar* GetSizingControlBar();

    //{{AFX_MSG(CmlMiniDockFrameWnd)
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLCONTROLBAR_H__3AC509E3_BC2E_48CF_A756_92B447DDA8D7__INCLUDED_)

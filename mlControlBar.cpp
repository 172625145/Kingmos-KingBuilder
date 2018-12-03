// mlControlBar.cpp : implementation file
//

#include "stdafx.h"
//#include "mlClass.h"
#include "mlControlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CmlControlBar

//IMPLEMENT_DYNAMIC(CmlControlBar, CControlBar);

CmlControlBar::CmlControlBar()
{
    m_szMinHorz = CSize(33, 32);
    m_szMinVert = CSize(33, 32);
    m_szMinFloat = CSize(37, 32);
    m_szHorz = CSize(220, 200);
    m_szVert = CSize(220, 200);
    m_szFloat = CSize(220, 200);
    m_bTracking = FALSE;
    m_bKeepSize = FALSE;
    m_bParentSizing = FALSE;
    m_cxEdge = 5;
    m_bDragShowContent = FALSE;
    m_nDockBarID = 0;
    m_dwSCBStyle = 0;

    m_cyGripper = 12;
}

CmlControlBar::~CmlControlBar()
{
    if (m_pDockContext)
		delete m_pDockContext;
	m_pDockContext = NULL;
}

BEGIN_MESSAGE_MAP(CmlControlBar, CControlBar)
    //{{AFX_MSG_MAP(CmlControlBar)
    ON_WM_CREATE()
    ON_WM_NCPAINT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCHITTEST()
    ON_WM_CAPTURECHANGED()
    ON_WM_SETTINGCHANGE()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_PAINT()
    ON_WM_CLOSE()
    ON_WM_SIZE()
	ON_WM_NCLBUTTONUP()
    ON_WM_NCMOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// old creation method, still here for compatibility reasons
//BOOL CmlControlBar::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
//                               CSize sizeDefault, BOOL bHasGripper,
 //                              UINT nID, DWORD dwStyle)
//{
 //   UNUSED_ALWAYS(bHasGripper);
//
 //   m_szHorz = m_szVert = m_szFloat = sizeDefault;
 //   return Create(lpszWindowName, pParentWnd, nID, dwStyle);
//}

// preffered creation method
BOOL CmlControlBar::Create(LPCTSTR lpszWindowName,
                               CWnd* pParentWnd, UINT nID,
                               DWORD dwStyle)
{
    // must have a parent
    ASSERT_VALID(pParentWnd);
    // cannot be both fixed and dynamic
    // (CBRS_SIZE_DYNAMIC is used for resizng when floating)
    ASSERT (!((dwStyle & CBRS_SIZE_FIXED) &&
		(dwStyle & CBRS_SIZE_DYNAMIC)));
	
    m_dwStyle = dwStyle & CBRS_ALL; // save the control bar styles
	
    // register and create the window - skip CControlBar::Create()
    CString wndclass = ::AfxRegisterWndClass(CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
        ::GetSysColorBrush(COLOR_BTNFACE), 0);
	
    dwStyle &= ~CBRS_ALL;		// keep only the generic window styles
    dwStyle |= WS_CLIPCHILDREN; // prevents flashing
    if (!CWnd::Create(wndclass, lpszWindowName, dwStyle,
        CRect(0, 0, 0, 0), pParentWnd, nID))
        return FALSE;
	
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// CmlControlBar operations

void CmlControlBar::EnableDocking(DWORD dwDockStyle)
{
    // must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
    ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
    // cannot have the CBRS_FLOAT_MULTI style
    ASSERT((dwDockStyle & CBRS_FLOAT_MULTI) == 0);
    // the bar must have CBRS_SIZE_DYNAMIC style
    ASSERT((m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);

    m_dwDockStyle = dwDockStyle;
    if (m_pDockContext == NULL)
        m_pDockContext = new CmlDockContext(this);

    // permanently wire the bar's owner to its current parent
    if (m_hWndOwner == NULL)
        m_hWndOwner = ::GetParent(m_hWnd);
}

/////////////////////////////////////////////////////////////////////////
// CmlControlBar message handlers

int CmlControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CControlBar::OnCreate(lpCreateStruct) == -1)
        return -1;

    // query SPI_GETDRAGFULLWINDOWS system parameter
    // OnSettingChange() will update m_bDragShowContent
    m_bDragShowContent = FALSE;
//    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
//        &m_bDragShowContent, 0);

    // uncomment this line if you want raised borders
    m_dwSCBStyle |= SCBS_SHOWEDGES;

    return 0;
}


LRESULT CmlControlBar::OnSetText(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);

    LRESULT lResult = CWnd::Default();

    if (IsFloating())
    {
        m_pDockBar->SetWindowText((LPCTSTR) lParam); // update dockbar
        GetParentFrame()->DelayRecalcLayout(); // refresh miniframe
    }

    return lResult;
}

const BOOL CmlControlBar::IsFloating() const
{
    return !IsHorzDocked() && !IsVertDocked();
}

const BOOL CmlControlBar::IsHorzDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP ||
        m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

const BOOL CmlControlBar::IsVertDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_LEFT ||
        m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT);
}

const BOOL CmlControlBar::IsSideTracking() const
{
    // don't call this when not tracking
    ASSERT(m_bTracking && !IsFloating());

    return (m_htEdge == HTLEFT || m_htEdge == HTRIGHT) ?
        IsHorzDocked() : IsVertDocked();
}

CSize CmlControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    if (bStretch)
	{
		// the bar is stretched (is not the child of a dockbar)
        if (bHorz)
            return CSize(32767, m_szHorz.cy);
        else
            return CSize(m_szVert.cx, 32767);
	}

    // dirty cast - we need access to protected CDockBar members
    CmlDockBar* pDockBar = (CmlDockBar*) m_pDockBar;

    // force imediate RecalcDelayShow() for all sizing bars on the row
    // with delayShow/delayHide flags set to avoid IsVisible() problems
    CmlCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP = pDockBar->m_bLayoutQuery ?
        NULL : ::BeginDeferWindowPos(arrSCBars.GetSize());
    for (int i = 0; i < arrSCBars.GetSize(); i++)
        if (arrSCBars[i]->m_nStateFlags & (delayHide|delayShow))
            arrSCBars[i]->RecalcDelayShow(&layout);
    if (layout.hDWP != NULL)
        ::EndDeferWindowPos(layout.hDWP);

    // get available length
    CRect rc = pDockBar->m_rectLayout;
    if (rc.IsRectEmpty())
        m_pDockSite->GetClientRect(&rc);
    int nLengthTotal = bHorz ? rc.Width() + 2 : rc.Height() - 2;

    if (IsVisible() && !IsFloating() &&
        m_bParentSizing && arrSCBars[0] == this)
        if (NegotiateSpace(nLengthTotal, (bHorz != FALSE)))
            AlignControlBars();

    m_bParentSizing = FALSE;

    if (bHorz)
        return CSize(max(m_szMinHorz.cx, m_szHorz.cx),
                     max(m_szMinHorz.cy, m_szHorz.cy));

    return CSize(max(m_szMinVert.cx, m_szVert.cx),
                 max(m_szMinVert.cy, m_szVert.cy));
}

CSize CmlControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK)) // docked ?
    {
        if (nLength == -1)
            m_bParentSizing = TRUE;

        return CControlBar::CalcDynamicLayout(nLength, dwMode);
    }

    if (dwMode & LM_MRUWIDTH)
		return m_szFloat;

    if (dwMode & LM_COMMIT)
		return m_szFloat; // already committed

//#ifndef _SCB_REPLACE_MINIFRAME
    // check for dialgonal resizing hit test
    int nHitTest = m_pDockContext->m_nHitTest;
    if (IsFloating() &&
        (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT ||
        nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT))
    {
        CPoint ptCursor;
        ::GetCursorPos(&ptCursor);

        CRect rFrame, rBar;
        GetParentFrame()->GetWindowRect(&rFrame);
        GetWindowRect(&rBar);
        
        if (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT)
        {
            m_szFloat.cx = rFrame.left + rBar.Width() - ptCursor.x;
            m_pDockContext->m_rectFrameDragHorz.left = 
                min(ptCursor.x, rFrame.left + rBar.Width() - m_szMinFloat.cx);
        }

        if (nHitTest == HTTOPLEFT || nHitTest == HTTOPRIGHT)
        {
            m_szFloat.cy = rFrame.top + rBar.Height() - ptCursor.y;
            m_pDockContext->m_rectFrameDragHorz.top =
                min(ptCursor.y, rFrame.top + rBar.Height() - m_szMinFloat.cy);
        }

        if (nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT)
            m_szFloat.cx = rBar.Width() + ptCursor.x - rFrame.right;

        if (nHitTest == HTBOTTOMLEFT || nHitTest == HTBOTTOMRIGHT)
            m_szFloat.cy = rBar.Height() + ptCursor.y - rFrame.bottom;
    }
    else
//#endif //_SCB_REPLACE_MINIFRAME
        ((dwMode & LM_LENGTHY) ? m_szFloat.cy : m_szFloat.cx) = nLength;

    m_szFloat.cx = max(m_szFloat.cx, m_szMinFloat.cx);
    m_szFloat.cy = max(m_szFloat.cy, m_szMinFloat.cy);

    return m_szFloat;
}

void CmlControlBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    // force non-client recalc if moved or resized
    lpwndpos->flags |= SWP_FRAMECHANGED;

    CControlBar::OnWindowPosChanging(lpwndpos);

    // find on which side are we docked
    m_nDockBarID = GetParent()->GetDlgCtrlID();

    if (!IsFloating())
        if (lpwndpos->flags & SWP_SHOWWINDOW)
            m_bKeepSize = TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void CmlControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
//	if (m_pDockBar != NULL && OnToolHitTest(point, NULL) == -1)
    if (m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
        m_pDockContext->StartDrag(point);
    }
    else
        CWnd::OnLButtonDown(nFlags, point);
}

void CmlControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_pDockBar != NULL)
    {
        // toggle docking
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    }
    else
        CWnd::OnLButtonDblClk(nFlags, point);
}

void CmlControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
    UNUSED_ALWAYS(point);

    if (m_bTracking || IsFloating())
        return;

    if ((nHitTest >= HTSIZEFIRST) && (nHitTest <= HTSIZELAST))
        StartTracking(nHitTest); // sizing edge hit
}

void CmlControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        StopTracking();

    CControlBar::OnLButtonUp(nFlags, point);
}

void CmlControlBar::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        StopTracking();

    CControlBar::OnRButtonDown(nFlags, point);
}

void CmlControlBar::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        OnTrackUpdateSize(point);

    CControlBar::OnMouseMove(nFlags, point);
}

void CmlControlBar::OnCaptureChanged(CWnd *pWnd)
{
    if (m_bTracking && (pWnd != this))
        StopTracking();

    CControlBar::OnCaptureChanged(pWnd);
}

void CmlControlBar::OnNcCalcSize(BOOL bCalcValidRects,
                                     NCCALCSIZE_PARAMS FAR* lpncsp)
{
    UNUSED_ALWAYS(bCalcValidRects);

//#ifndef _SCB_REPLACE_MINIFRAME
    // Enable diagonal resizing for floating miniframe
    if (IsFloating())
    {
        CFrameWnd* pFrame = GetParentFrame();
        if (pFrame != NULL)
        {
            DWORD dwStyle = ::GetWindowLong(pFrame->m_hWnd, GWL_STYLE);
            if ((dwStyle & MFS_4THICKFRAME) != 0)
            {
                pFrame->ModifyStyle(MFS_4THICKFRAME, 0); // clear
                GetParent()->ModifyStyle(0, WS_CLIPCHILDREN);
            }
        }
    }
//#endif _SCB_REPLACE_MINIFRAME

    // compute the the client area
    m_dwSCBStyle &= ~SCBS_EDGEALL;

    if (!IsFloating() && m_pDockBar != NULL)
    {
        CmlCBArray arrSCBars;
        GetRowSizingBars(arrSCBars);

        for (int i = 0; i < arrSCBars.GetSize(); i++)
            if (arrSCBars[i] == this)
            {
                if (i > 0)
                    m_dwSCBStyle |= IsHorzDocked() ?
                        SCBS_EDGELEFT : SCBS_EDGETOP;
                if (i < arrSCBars.GetSize() - 1)
                    m_dwSCBStyle |= IsHorzDocked() ?
                        SCBS_EDGERIGHT : SCBS_EDGEBOTTOM;
            }
    }

    NcCalcClient(&lpncsp->rgrc[0], m_nDockBarID);
}

void CmlControlBar::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
    CRect rc(pRc);

    rc.DeflateRect(3, 5, 3, 3);
    if (nDockBarID != AFX_IDW_DOCKBAR_FLOAT)
        rc.DeflateRect(2, 0, 2, 2);

    switch(nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        m_dwSCBStyle |= SCBS_EDGEBOTTOM;
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        m_dwSCBStyle |= SCBS_EDGETOP;
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        m_dwSCBStyle |= SCBS_EDGERIGHT;
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        m_dwSCBStyle |= SCBS_EDGELEFT;
        break;
    }

    // make room for edges only if they will be painted
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rc.DeflateRect(
            (m_dwSCBStyle & SCBS_EDGELEFT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGETOP) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGERIGHT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGEBOTTOM) ? m_cxEdge : 0);

    *pRc = rc;

    if (HasGripper())
	{

    CRect rc(pRc);

    BOOL bHorz = (nDockBarID == AFX_IDW_DOCKBAR_TOP) ||
                 (nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);

    if (bHorz)
        rc.DeflateRect(m_cyGripper, 0, 0, 0);
    else
        rc.DeflateRect(0, m_cyGripper, 0, 0);

    // set position for the "x" (hide bar) button
    CPoint ptOrgBtn;
    if (bHorz)
        ptOrgBtn = CPoint(rc.left - 8, rc.top + 5);
    else
        ptOrgBtn = CPoint(rc.right - 7, rc.top - 8);

    m_biHide.Move(ptOrgBtn - CRect(pRc).TopLeft());
    *pRc = rc;
	}
}

void CmlControlBar::OnNcPaint()
{
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this);

    CRect rcClient, rcBar;
    GetClientRect(rcClient);
    ClientToScreen(rcClient);
    GetWindowRect(rcBar);
    rcClient.OffsetRect(-rcBar.TopLeft());
    rcBar.OffsetRect(-rcBar.TopLeft());

    CDC mdc;
    mdc.CreateCompatibleDC(&dc);
    
    CBitmap bm;
    bm.CreateCompatibleBitmap(&dc, rcBar.Width(), rcBar.Height());
    CBitmap* pOldBm = mdc.SelectObject(&bm);

    // client area is not our bussiness :)
    dc.ExcludeClipRect(rcClient);

    // draw borders in non-client area
    CRect rcDraw = rcBar;
    DrawBorders(&mdc, rcDraw);

    // erase parts not drawn
    mdc.IntersectClipRect(rcDraw);

    // erase the NC background
    mdc.FillRect(rcDraw, CBrush::FromHandle(
        (HBRUSH) GetClassLong(m_hWnd, GCL_HBRBACKGROUND)));

    if (m_dwSCBStyle & SCBS_SHOWEDGES)
    {
        CRect rcEdge; // paint the sizing edges
        for (int i = 0; i < 4; i++)
            if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
                mdc.Draw3dRect(rcEdge, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                    ::GetSysColor(COLOR_BTNSHADOW));
    }

    NcPaintGripper(&mdc, rcClient);

    dc.BitBlt(0, 0, rcBar.Width(), rcBar.Height(), &mdc, 0, 0, SRCCOPY);

    ReleaseDC(&dc);

    mdc.SelectObject(pOldBm);
    bm.DeleteObject();
    mdc.DeleteDC();
}

void CmlControlBar::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    if (!HasGripper())
        return;

    // paints a simple "two raised lines" gripper
    // override this if you want a more sophisticated gripper
    CRect gripper = rcClient;
    CRect rcbtn = m_biHide.GetRect();
    BOOL bHorz = IsHorzDocked();

    gripper.DeflateRect(1, 1);
    if (bHorz)
    {   // gripper at left
        gripper.left -= m_cyGripper;
        gripper.right = gripper.left + 3;
        gripper.top = rcbtn.bottom + 3;
    }
    else
    {   // gripper at top
        gripper.top -= m_cyGripper;
        gripper.bottom = gripper.top + 3;
        gripper.right = rcbtn.left - 3;
    }

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    gripper.OffsetRect(bHorz ? 4 : 0, bHorz ? 0 : 4);

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    m_biHide.Paint(pDC);
}

void CmlControlBar::OnPaint()
{
    // overridden to skip border painting based on clientrect
    CPaintDC dc(this);
}

UINT CmlControlBar::OnNcHitTest(CPoint point)
{
    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);
	UINT nRet = HTCLIENT;

    if (!IsFloating())
        for (int i = 0; i < 4; i++)
            if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
                if (rcEdge.PtInRect(point))
                {
					nRet = GetEdgeHTCode(i);
					break;
				}

    if (nRet != HTCLIENT)
        return nRet;

    CRect rc = m_biHide.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTCLOSE;

    return HTCLIENT;
}

void CmlControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CControlBar::OnSettingChange(uFlags, lpszSection);

    m_bDragShowContent = FALSE;
//    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
//        &m_bDragShowContent, 0); // update
}

void CmlControlBar::OnSize(UINT nType, int cx, int cy)
{
    UNUSED_ALWAYS(nType);
    
    if ((m_dwSCBStyle & SCBS_SIZECHILD) != 0)
    {
        // automatic child resizing - only one child is allowed
        CWnd* pWnd = GetWindow(GW_CHILD);
        if (pWnd != NULL)
        {
            pWnd->MoveWindow(0, 0, cx, cy);
            ASSERT(pWnd->GetWindow(GW_HWNDNEXT) == NULL);
        }
    }
}

void CmlControlBar::OnClose()
{
    // do nothing: protection against accidentally destruction by the
    //   child control (i.e. if user hits Esc in a child editctrl)
//    if (m_pDockContext)
//		delete m_pDockContext;
//	m_pDockContext = NULL;
}

/////////////////////////////////////////////////////////////////////////
// CmlControlBar implementation helpers

void CmlControlBar::StartTracking(UINT nHitTest)
{
    SetCapture();

    // make sure no updates are pending
    if (!m_bDragShowContent)
        RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

    BOOL bHorz = IsHorzDocked();

    m_szOld = bHorz ? m_szHorz : m_szVert;

    CRect rc;
    GetWindowRect(&rc);
    CRect rcEdge;
    VERIFY(GetEdgeRect(rc, nHitTest, rcEdge));
    m_ptOld = rcEdge.CenterPoint();

    m_htEdge = nHitTest;
    m_bTracking = TRUE;

    CmlCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);

    // compute the minsize as the max minsize of the sizing bars on row
    m_szMinT = bHorz ? m_szMinHorz : m_szMinVert;
    for (int i = 0; i < arrSCBars.GetSize(); i++)
        if (bHorz)
            m_szMinT.cy = max(m_szMinT.cy, arrSCBars[i]->m_szMinHorz.cy);
        else
            m_szMinT.cx = max(m_szMinT.cx, arrSCBars[i]->m_szMinVert.cx);

    m_szMaxT = m_szOld;
    if (!IsSideTracking())
    {
        // the control bar cannot grow with more than the size of
        // remaining client area of the mainframe
        m_pDockSite->RepositionBars(0, 0xFFFF, AFX_IDW_PANE_FIRST,
            reposQuery, &rc, NULL, TRUE);
        m_szMaxT += rc.Size() - CSize(4, 4);
    }
    else
    {
        // side tracking: max size is the actual size plus the amount
        // the neighbour bar can be decreased to reach its minsize
        for (int i = 0; i < arrSCBars.GetSize(); i++)
            if (arrSCBars[i] == this) break;

        CmlControlBar* pBar = arrSCBars[i +
            ((m_htEdge == HTTOP || m_htEdge == HTLEFT) ? -1 : 1)];

        m_szMaxT += (bHorz ? pBar->m_szHorz : pBar->m_szVert) -
            CSize(pBar->m_szMinHorz.cx, pBar->m_szMinVert.cy);
    }

    OnTrackInvertTracker(); // draw tracker
}

void CmlControlBar::StopTracking()
{
    OnTrackInvertTracker(); // erase tracker

    m_bTracking = FALSE;
    ReleaseCapture();

    m_pDockSite->DelayRecalcLayout();
}

void CmlControlBar::OnTrackUpdateSize(CPoint& point)
{
    ASSERT(!IsFloating());

    CPoint pt = point;
    ClientToScreen(&pt);
    CSize szDelta = pt - m_ptOld;

    CSize sizeNew = m_szOld;
    switch (m_htEdge)
    {
    case HTLEFT:    sizeNew -= CSize(szDelta.cx, 0); 
		break;
    case HTTOP:     sizeNew -= CSize(0, szDelta.cy);
		break;
    case HTRIGHT:   sizeNew += CSize(szDelta.cx, 0);
		break;
    case HTBOTTOM:  sizeNew += CSize(0, szDelta.cy);
		break;
    }

    // enforce the limits
    sizeNew.cx = max(m_szMinT.cx, min(m_szMaxT.cx, sizeNew.cx));
    sizeNew.cy = max(m_szMinT.cy, min(m_szMaxT.cy, sizeNew.cy));

    BOOL bHorz = IsHorzDocked();
    szDelta = sizeNew - (bHorz ? m_szHorz : m_szVert);

    if (szDelta == CSize(0, 0))
        return; // no size change

    OnTrackInvertTracker(); // erase tracker

    (bHorz ? m_szHorz : m_szVert) = sizeNew; // save the new size

    CmlCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);

    for (int i = 0; i < arrSCBars.GetSize(); i++)
        if (!IsSideTracking())
        {   // track simultaneously
            CmlControlBar* pBar = arrSCBars[i];
            (bHorz ? pBar->m_szHorz.cy : pBar->m_szVert.cx) =
                bHorz ? sizeNew.cy : sizeNew.cx;
        }
        else
        {   // adjust the neighbour's size too
            if (arrSCBars[i] != this) continue;

            CmlControlBar* pBar = arrSCBars[i +
                ((m_htEdge == HTTOP || m_htEdge == HTLEFT) ? -1 : 1)];

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) -=
                bHorz ? szDelta.cx : szDelta.cy;
        }

    OnTrackInvertTracker(); // redraw tracker at new pos

    if (m_bDragShowContent)
        m_pDockSite->DelayRecalcLayout();
}

void CmlControlBar::OnTrackInvertTracker()
{
    ASSERT(m_bTracking);

    if (m_bDragShowContent)
        return; // don't show tracker if DragFullWindows is on

    BOOL bHorz = IsHorzDocked();
    CRect rc, rcBar, rcDock, rcFrame;
    GetWindowRect(rcBar);
    m_pDockBar->GetWindowRect(rcDock);
    m_pDockSite->GetWindowRect(rcFrame);
    VERIFY(GetEdgeRect(rcBar, m_htEdge, rc));
    if (!IsSideTracking())
        rc = bHorz ? 
            CRect(rcDock.left + 1, rc.top, rcDock.right - 1, rc.bottom) :
            CRect(rc.left, rcDock.top + 1, rc.right, rcDock.bottom - 1);

    rc.OffsetRect(-rcFrame.TopLeft());

    CSize sizeNew = bHorz ? m_szHorz : m_szVert;
    CSize sizeDelta = sizeNew - m_szOld;
    if (m_nDockBarID == AFX_IDW_DOCKBAR_LEFT && m_htEdge == HTTOP ||
        m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT && m_htEdge != HTBOTTOM ||
        m_nDockBarID == AFX_IDW_DOCKBAR_TOP && m_htEdge == HTLEFT ||
        m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM && m_htEdge != HTRIGHT)
        sizeDelta = -sizeDelta;
    rc.OffsetRect(sizeDelta);

    CDC *pDC = m_pDockSite->GetDCEx(NULL,
        DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    CBrush* pBrush = CDC::GetHalftoneBrush();
    CBrush* pBrushOld = pDC->SelectObject(pBrush);

    pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATINVERT);
    
    pDC->SelectObject(pBrushOld);
    m_pDockSite->ReleaseDC(pDC);
}

BOOL CmlControlBar::GetEdgeRect(CRect rcWnd, UINT nHitTest,
                                    CRect& rcEdge)
{
    rcEdge = rcWnd;
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rcEdge.DeflateRect(1, 1);
    BOOL bHorz = IsHorzDocked();

    switch (nHitTest)
    {
    case HTLEFT:
        if (!(m_dwSCBStyle & SCBS_EDGELEFT)) return FALSE;
        rcEdge.right = rcEdge.left + m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTTOP:
        if (!(m_dwSCBStyle & SCBS_EDGETOP)) return FALSE;
        rcEdge.bottom = rcEdge.top + m_cxEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cxEdge, 0);
        break;
    case HTRIGHT:
        if (!(m_dwSCBStyle & SCBS_EDGERIGHT)) return FALSE;
        rcEdge.left = rcEdge.right - m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTBOTTOM:
        if (!(m_dwSCBStyle & SCBS_EDGEBOTTOM)) return FALSE;
        rcEdge.top = rcEdge.bottom - m_cxEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cxEdge, 0);
        break;
    default:
        ASSERT(FALSE); // invalid hit test code
    }
    return TRUE;
}

UINT CmlControlBar::GetEdgeHTCode(int nEdge)
{
    if (nEdge == 0)
		return HTLEFT;
    if (nEdge == 1)
		return HTTOP;
    if (nEdge == 2)
		return HTRIGHT;
    if (nEdge == 3)
		return HTBOTTOM;

    ASSERT(FALSE); // invalid edge code
    return HTNOWHERE;
}

void CmlControlBar::GetRowInfo(int& nFirst, int& nLast, int& nThis)
{
    ASSERT_VALID(m_pDockBar); // verify bounds

    nThis = m_pDockBar->FindBar(this);
    ASSERT(nThis != -1);

    int i, nBars = m_pDockBar->m_arrBars.GetSize();

    // find the first and the last bar in row
    for (nFirst = -1, i = nThis - 1; i >= 0 && nFirst == -1; i--)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nFirst = i + 1;
    for (nLast = -1, i = nThis + 1; i < nBars && nLast == -1; i++)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nLast = i - 1;

    ASSERT((nLast != -1) && (nFirst != -1));
}

void CmlControlBar::GetRowSizingBars(CmlCBArray& arrSCBars)
{
    arrSCBars.RemoveAll();

    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    for (int i = nFirst; i <= nLast; i++)
    {
        CmlControlBar* pBar =
            (CmlControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;
        
		arrSCBars.Add(pBar);
    }
}

BOOL CmlControlBar::NegotiateSpace(int nLengthTotal, BOOL bHorz)
{
    ASSERT(bHorz == IsHorzDocked());

    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    int nLengthAvail = nLengthTotal;
    int nLengthActual = 0;
    int nLengthMin = 2;
    int nWidthMax = 0;
    CmlControlBar* pBar;

    for (int i = nFirst; i <= nLast; i++)
    {
        pBar = (CmlControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;
        BOOL bIsSizingBar = TRUE;
//            pBar->IsKindOf(CmlControlBar);

        int nLengthBar; // minimum length of the bar
        if (bIsSizingBar)
            nLengthBar = bHorz ? pBar->m_szMinHorz.cx - 2 :
                pBar->m_szMinVert.cy - 2;
        else
        {
            CRect rcBar;
            pBar->GetWindowRect(&rcBar);
            nLengthBar = bHorz ? rcBar.Width() - 2 : rcBar.Height() - 2;
        }

        nLengthMin += nLengthBar;
        if (nLengthMin > nLengthTotal)
        {
            // split the row after fixed bar
            if (i < nThis)
            {
                m_pDockBar->m_arrBars.InsertAt(i + 1,
                    (CControlBar*) NULL);
                return FALSE;
            }
            
            // only this sizebar remains on the row, adjust it to minsize
            if (i == nThis)
            {
                if (bHorz)
                    m_szHorz.cx = m_szMinHorz.cx;
                else
                    m_szVert.cy = m_szMinVert.cy;

                return TRUE; // the dockbar will split the row for us
            }

            // we have enough bars - go negotiate with them
            m_pDockBar->m_arrBars.InsertAt(i, (CControlBar*) NULL);
            nLast = i - 1;
            break;
        }

        if (bIsSizingBar)
        {
            nLengthActual += bHorz ? pBar->m_szHorz.cx - 2 : 
                pBar->m_szVert.cy - 2;
            nWidthMax = max(nWidthMax, bHorz ? pBar->m_szHorz.cy :
                pBar->m_szVert.cx);
        }
        else
            nLengthAvail -= nLengthBar;
    }

    CmlCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    int nNumBars = arrSCBars.GetSize();
    int nDelta = nLengthAvail - nLengthActual;

    // return faster when there is only one sizing bar per row (this one)
    if (nNumBars == 1)
    {
        ASSERT(arrSCBars[0] == this);

        if (nDelta != 0)
        {
            m_bKeepSize = FALSE;
            (bHorz ? m_szHorz.cx : m_szVert.cy) += nDelta;
        }

        return TRUE;
    }

    // make all the bars the same width
    for (i = 0; i < nNumBars; i++)
        if (bHorz)
            arrSCBars[i]->m_szHorz.cy = nWidthMax;
        else
            arrSCBars[i]->m_szVert.cx = nWidthMax;

    // distribute the difference between the bars,
    // but don't shrink them below their minsizes
    while (nDelta != 0)
    {
        int nDeltaOld = nDelta;
        for (i = 0; i < nNumBars; i++)
        {
            pBar = arrSCBars[i];
            int nLMin = bHorz ?
                pBar->m_szMinHorz.cx : pBar->m_szMinVert.cy;
            int nL = bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy;

            if ((nL == nLMin) && (nDelta < 0) || // already at min length
                pBar->m_bKeepSize) // or wants to keep its size
                continue;

            // sign of nDelta
            int nDelta2 = (nDelta == 0) ? 0 : ((nDelta < 0) ? -1 : 1);

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) += nDelta2;
            nDelta -= nDelta2;
            if (nDelta == 0) break;
        }
        // clear m_bKeepSize flags
        if ((nDeltaOld == nDelta) || (nDelta == 0))
            for (i = 0; i < nNumBars; i++)
                arrSCBars[i]->m_bKeepSize = FALSE;
    }

    return TRUE;
}

void CmlControlBar::AlignControlBars()
{
    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    BOOL bHorz = IsHorzDocked();
    BOOL bNeedRecalc = FALSE;
    int nAlign = bHorz ? -2 : 0;

    CRect rc, rcDock;
    m_pDockBar->GetWindowRect(&rcDock);

    for (int i = nFirst; i <= nLast; i++)
    {
        CmlControlBar* pBar =
            (CmlControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0)
			continue; // placeholder
        if (!pBar->IsVisible())
			continue;

        pBar->GetWindowRect(&rc);
        rc.OffsetRect(-rcDock.TopLeft());

//        if (pBar->IsKindOf(CmlControlBar))
            rc = CRect(rc.TopLeft(),
                bHorz ? pBar->m_szHorz : pBar->m_szVert);

        if ((bHorz ? rc.left : rc.top) != nAlign)
        {
            if (!bHorz)
                rc.OffsetRect(0, nAlign - rc.top - 2);
            else if (m_nDockBarID == AFX_IDW_DOCKBAR_TOP)
                rc.OffsetRect(nAlign - rc.left, -2);
            else
                rc.OffsetRect(nAlign - rc.left, 0);
            pBar->MoveWindow(rc);
            bNeedRecalc = TRUE;
        }
        nAlign += (bHorz ? rc.Width() : rc.Height()) - 2;
    }

    if (bNeedRecalc)
        m_pDockSite->DelayRecalcLayout();
}

void CmlControlBar::OnUpdateCmdUI(CFrameWnd* pTarget,
                                      BOOL bDisableIfNoHndler)
{
//    UNUSED_ALWAYS(bDisableIfNoHndler);
  //  UNUSED_ALWAYS(pTarget);
	
    UpdateDialogControls(pTarget, bDisableIfNoHndler);

    if (! HasGripper())
        return;

    BOOL bNeedPaint = FALSE;

    CPoint pt;
    ::GetCursorPos(&pt);
    BOOL bHit = (OnNcHitTest(pt) == HTCLOSE);
    BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasPushed = m_biHide.bPushed;
    m_biHide.bPushed = bHit && bLButtonDown;

    BOOL bWasRaised = m_biHide.bRaised;
    m_biHide.bRaised = bHit && !bLButtonDown;

    bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) ||
                  (m_biHide.bRaised ^ bWasRaised);

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
}

void CmlControlBar::LoadState(LPCTSTR lpszProfileName)
{
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd()); // must be called after Create()

//#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
    // compensate the caption miscalculation in CFrameWnd::SetDockState()
    CDockState state;
    state.LoadState(lpszProfileName);

    UINT nID = GetDlgCtrlID();
    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        if (!pInfo->m_bFloating)
            continue;
        
        // this is a floating dockbar - check the ID array
        for (int j = 0; j < pInfo->m_arrBarID.GetSize(); j++)
            if ((DWORD) pInfo->m_arrBarID[j] == nID)
            {
                // found this bar - offset origin and save settings
                pInfo->m_pointPos.x++;
                pInfo->m_pointPos.y +=
                    ::GetSystemMetrics(SM_CYSMCAPTION) + 1;
                pInfo->SaveState(lpszProfileName, i);
            }
    }
//#endif //_SCB_REPLACE_MINIFRAME && !_SCB_MINIFRAME_CAPTION

    CWinApp* pApp = AfxGetApp();

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-mlControlBar-%d"), lpszProfileName,
        GetDlgCtrlID());

    m_szHorz.cx = max(m_szMinHorz.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeHorzCX"), m_szHorz.cx));
    m_szHorz.cy = max(m_szMinHorz.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeHorzCY"), m_szHorz.cy));

    m_szVert.cx = max(m_szMinVert.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeVertCX"), m_szVert.cx));
    m_szVert.cy = max(m_szMinVert.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeVertCY"), m_szVert.cy));

    m_szFloat.cx = max(m_szMinFloat.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeFloatCX"), m_szFloat.cx));
    m_szFloat.cy = max(m_szMinFloat.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeFloatCY"), m_szFloat.cy));
}

void CmlControlBar::SaveState(LPCTSTR lpszProfileName)
{
    // place your SaveState or GlobalSaveState call in
    // CMainFrame's OnClose() or DestroyWindow(), not in OnDestroy()
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd());

    CWinApp* pApp = AfxGetApp();

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-mlControlBar-%d"), lpszProfileName,
        GetDlgCtrlID());

    pApp->WriteProfileInt(szSection, _T("sizeHorzCX"), m_szHorz.cx);
    pApp->WriteProfileInt(szSection, _T("sizeHorzCY"), m_szHorz.cy);

    pApp->WriteProfileInt(szSection, _T("sizeVertCX"), m_szVert.cx);
    pApp->WriteProfileInt(szSection, _T("sizeVertCY"), m_szVert.cy);

    pApp->WriteProfileInt(szSection, _T("sizeFloatCX"), m_szFloat.cx);
    pApp->WriteProfileInt(szSection, _T("sizeFloatCY"), m_szFloat.cy);
}

void CmlControlBar::GlobalLoadState(CFrameWnd* pFrame,
                                        LPCTSTR lpszProfileName)
{
    POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        CmlControlBar* pBar = 
            (CmlControlBar*) pFrame->m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
//        if (pBar->IsKindOf(CmlControlBar))
            pBar->LoadState(lpszProfileName);
    }
}

void CmlControlBar::GlobalSaveState(CFrameWnd* pFrame,
                                        LPCTSTR lpszProfileName)
{
    POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        CmlControlBar* pBar =
            (CmlControlBar*) pFrame->m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
//        if (pBar->IsKindOf(CmlControlBar))
            pBar->SaveState(lpszProfileName);
    }
}

BOOL CmlControlBar::HasGripper() const
{
    // if the miniframe has a caption, don't display the gripper
    if (IsFloating())
        return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CmlDockContext Drag Operations

static void AdjustRectangle(CRect& rect, CPoint pt)
{
    int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
                    (pt.x > rect.right) ? (pt.x - rect.right) : 0;
    int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
                    (pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
    rect.OffsetRect(nXOffset, nYOffset);
}

void CmlDockContext::StartDrag(CPoint pt)
{
	TRACE("abcdefg");
	CDockContext::StartDrag (pt);
	return ;
/*
    ASSERT_VALID(m_pBar);
    m_bDragging = TRUE;

    InitLoop();

    ASSERT((m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);

    // get true bar size (including borders)
    CRect rect;
    m_pBar->GetWindowRect(rect);
    m_ptLast = pt;
    CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
    CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
    CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

    m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
    m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

    // calculate frame dragging rectangle
    m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);

//#ifdef _MAC
 //   CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz,
//        WS_THICKFRAME, WS_EX_FORCESIZEBOX);
//#else
    CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz, WS_THICKFRAME);
//#endif
    m_rectFrameDragHorz.DeflateRect(2, 2);
    m_rectFrameDragVert = m_rectFrameDragHorz;
    
    // adjust rectangles so that point is inside
    AdjustRectangle(m_rectDragHorz, pt);
    AdjustRectangle(m_rectDragVert, pt);
    AdjustRectangle(m_rectFrameDragHorz, pt);
    AdjustRectangle(m_rectFrameDragVert, pt);

    // initialize tracking state and enter tracking loop
    m_dwOverDockStyle = CanDock();
    Move(pt);   // call it here to handle special keys
    Track();
*/
}

/////////////////////////////////////////////////////////////////////////////
// CmlMiniDockFrameWnd

//IMPLEMENT_DYNCREATE(CmlMiniDockFrameWnd, CMiniDockFrameWnd);

BEGIN_MESSAGE_MAP(CmlMiniDockFrameWnd, CMiniDockFrameWnd)
    //{{AFX_MSG_MAP(CmlMiniDockFrameWnd)
    ON_WM_NCLBUTTONDOWN()
    ON_WM_GETMINMAXINFO()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CmlMiniDockFrameWnd::Create(CWnd* pParent, DWORD dwBarStyle)
{
    // set m_bInRecalcLayout to avoid flashing during creation
    // RecalcLayout will be called once something is docked
    m_bInRecalcLayout = TRUE;

    DWORD dwStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|MFS_MOVEFRAME|
        MFS_4THICKFRAME|MFS_SYNCACTIVE|MFS_BLOCKSYSMENU|
        FWS_SNAPTOBARS;

    if (dwBarStyle & CBRS_SIZE_DYNAMIC)
        dwStyle &= ~MFS_MOVEFRAME;

    DWORD dwExStyle = 0;
//#ifdef _MAC
 //   if (dwBarStyle & CBRS_SIZE_DYNAMIC)
//        dwExStyle |= WS_EX_FORCESIZEBOX;
//    else
 //       dwStyle &= ~(MFS_MOVEFRAME|MFS_4THICKFRAME);
//#endif

    if (!CMiniFrameWnd::CreateEx(dwExStyle,
        NULL, &afxChNil, dwStyle, rectDefault, pParent))
    {
        m_bInRecalcLayout = FALSE;
        return FALSE;
    }
    dwStyle = dwBarStyle & (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT) ?
        CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
    dwStyle |= dwBarStyle & CBRS_FLOAT_MULTI;
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    //pSysMenu->DeleteMenu(SC_SIZE, MF_BYCOMMAND);
    CString strHide;
    if (strHide.LoadString(AFX_IDS_HIDE))
    {
        pSysMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
        pSysMenu->AppendMenu(MF_STRING|MF_ENABLED, SC_CLOSE, strHide);
    }

    // must initially create with parent frame as parent
    if (!m_wndDockBar.Create(pParent, WS_CHILD | WS_VISIBLE | dwStyle,
        AFX_IDW_DOCKBAR_FLOAT))
    {
        m_bInRecalcLayout = FALSE;
        return FALSE;
    }

    // set parent to CMiniDockFrameWnd
    m_wndDockBar.SetParent(this);
    m_bInRecalcLayout = FALSE;

    return TRUE;
}

void CmlMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
  //  if (nHitTest == HTCAPTION || nHitTest == HTCLOSE)
    {
        CMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
        return;
    }

 //   if (GetSizingControlBar() != NULL)
  //      CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
  //  else
  //      CMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
}

CmlControlBar* CmlMiniDockFrameWnd::GetSizingControlBar()
{
    CWnd* pWnd = GetWindow(GW_CHILD); // get the dockbar
    if (pWnd == NULL)
        return NULL;
    
    pWnd = pWnd->GetWindow(GW_CHILD); // get the controlbar
    if (pWnd == NULL)
        return NULL;

//    if (!pWnd->IsKindOf(CmlControlBar))
//        return NULL;

    return (CmlControlBar*) pWnd;
}

void CmlMiniDockFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
    CmlControlBar* pBar = GetSizingControlBar();
    if ((pBar != NULL) && (GetStyle() & MFS_4THICKFRAME) == 0
        && pBar->IsVisible())
        pBar->m_szFloat = CSize(cx + 4, cy + 4);

    CMiniDockFrameWnd::OnSize(nType, cx, cy);
}

void CmlMiniDockFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CMiniDockFrameWnd::OnGetMinMaxInfo(lpMMI);

    CmlControlBar* pBar = GetSizingControlBar();
    if (pBar != NULL)
    {
        CRect r(CPoint(0, 0), pBar->m_szMinFloat - CSize(4, 4));
//#ifndef _SCB_MINIFRAME_CAPTION
        CMiniFrameWnd::CalcBorders(&r, WS_THICKFRAME);
//#else
//        CMiniFrameWnd::CalcBorders(&r, WS_THICKFRAME|WS_CAPTION);
//#endif //_SCB_MINIFRAME_CAPTION
        lpMMI->ptMinTrackSize.x = r.Width();
        lpMMI->ptMinTrackSize.y = r.Height();
    }
}

void CmlMiniDockFrameWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
    if ((GetStyle() & MFS_4THICKFRAME) != 0)
    {
        CmlControlBar* pBar = GetSizingControlBar();
        if (pBar != NULL)
        {
            lpwndpos->flags |= SWP_NOSIZE; // don't size this time
            // prevents flicker
            pBar->m_pDockBar->ModifyStyle(0, WS_CLIPCHILDREN);
            // enable diagonal resizing
            ModifyStyle(MFS_4THICKFRAME, 0);
//#ifndef _SCB_MINIFRAME_CAPTION
            // remove caption
            ModifyStyle(WS_SYSMENU|WS_CAPTION, 0);
//#endif
            DelayRecalcLayout();
            pBar->PostMessage(WM_NCPAINT);
        }
    }

    CMiniFrameWnd::OnWindowPosChanging(lpwndpos);
}

/////////////////////////////////////////////////////////////////////////
// CmlButton

CmlButton::CmlButton()
{
    bRaised = TRUE;
    bPushed = FALSE;
}

void CmlButton::Paint(CDC* pDC)
{
    CRect rc = GetRect();

    if (bPushed)
	{  pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW),
            ::GetSysColor(COLOR_BTNHIGHLIGHT));
    }
	else
    {
		if (bRaised)
//			rc.InflateRect (-1, -1);
            pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                ::GetSysColor(COLOR_BTNSHADOW));
//			rc.InflateRect (1, 1);
 //           pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
 //               ::GetSysColor(COLOR_HIGHLIGHT));
	}

    COLORREF clrOldTextColor = pDC->GetTextColor();
    pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
    font.CreatePointFont(pointsize, _T("Marlett"));
    CFont* oldfont = pDC->SelectObject(&font);

//    pDC->TextOut(ptOrg.x + 2, ptOrg.y + 2, CString(_T("r"))); // x-like
	rc.top += 2;
	rc.left += 2;
	pDC->DrawText (CString(_T("r")), &rc, DT_CENTER|DT_SINGLELINE|DT_VCENTER);

    pDC->SelectObject(oldfont);
    pDC->SetBkMode(nPrevBkMode);
    pDC->SetTextColor(clrOldTextColor);
}

void CmlControlBar::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
    if (nHitTest == HTCLOSE)
        m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide

	CControlBar::OnNcLButtonUp(nHitTest, point);
}

void CmlControlBar::OnDestroy() 
{
	CControlBar::OnDestroy();
	
	// TODO: Add your message handler code here
//    if (m_pDockContext)
//		delete m_pDockContext;
//	m_pDockContext = NULL;
	
}

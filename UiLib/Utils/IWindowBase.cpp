#include "StdAfx.h"
#include "IWindowBase.h"

IWindowBase::IWindowBase(LPCTSTR _ZipSkin /*= NULL*/)
{
	ZipSkinPath = _ZipSkin?_ZipSkin:_T("");

#pragma region 窗口关闭按钮
	mpCloseBtn		= NULL;
	mpMaxBtn		= NULL;
	mpRestoreBtn	= NULL;
	mpMinBtn		= NULL;
#pragma endregion 窗口关闭按钮
}

IWindowBase::~IWindowBase()
{
	
}

LPCTSTR IWindowBase::GetWindowClassName() const
{
	try
	{
		return _T("WindowClassName");
	}
	catch(...)
	{
		throw "IWindowBase::GetWindowClassName";
	}
}

CControlUI* IWindowBase::CreateControl(LPCTSTR pstrClass, CPaintManagerUI* pManager)
{
	return NULL;
}

CControlUI* IWindowBase::CreateControl( LPCTSTR pstrClass )
{
	try
	{
		return NULL;
	}
	catch(...)
	{
		throw "IWindowBase::CreateControl";
	}
}

void IWindowBase::OnFinalMessage(HWND /*hWnd*/)
{
	pm.RemovePreMessageFilter(this);
	pm.RemoveNotifier(this);
	pm.ReapObjects(pm.GetRoot());
}

void IWindowBase::Init()
{

}

LRESULT IWindowBase::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT IWindowBase::OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT IWindowBase::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT IWindowBase::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = pm.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(pm.FindControl(pt));
			if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
				_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 &&
				_tcsicmp(pControl->GetClass(), _T("FadeButtonUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT IWindowBase::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x   = rcWork.left;
	lpMMI->ptMaxPosition.y   = rcWork.top;
	lpMMI->ptMaxSize.x       = rcWork.right;
	lpMMI->ptMaxSize.y       = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT IWindowBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = pm.GetRoundCorner();

	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}


LRESULT IWindowBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		return 1;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if(::IsZoomed(*this) != bZoomed )
	{
		if( !bZoomed ) {
			if( mpMaxBtn ) mpMaxBtn->SetVisible(false);
			if( mpRestoreBtn ) mpRestoreBtn->SetVisible(true);
		}
		else {
			if( mpMaxBtn ) mpMaxBtn->SetVisible(true);
			if( mpRestoreBtn ) mpRestoreBtn->SetVisible(false);
		}
	}

	return lRes;
}

CDuiString IWindowBase::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("Skins\\Default\\");
}

LRESULT IWindowBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	pm.Init(m_hWnd);
	pm.AddPreMessageFilter(this);

	CDialogBuilder builder;

	pm.SetResourcePath(GetSkinFolder().GetData());
	
	if(!ZipSkinPath.IsEmpty())
	{
		pm.SetResourceZip(ZipSkinPath.GetData());
	}

	if(!GetSkinFile().IsEmpty())
	{
		CControlUI* pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &pm);
		pm.AttachDialog(pRoot);
		pm.AddNotifier(this);

#pragma region 窗口关闭按钮
		mpCloseBtn					= static_cast<CButtonUI*>(pm.FindControl(_T("SysCloseBtn")));
		mpMaxBtn					= static_cast<CButtonUI*>(pm.FindControl(_T("SysMaxBtn")));
		mpRestoreBtn				= static_cast<CButtonUI*>(pm.FindControl(_T("SysRestoreBtn")));
		mpMinBtn					= static_cast<CButtonUI*>(pm.FindControl(_T("SysMinBtn")));
#pragma endregion 窗口关闭按钮

		Init();
		return 0;
	}
	return 1;
}

LRESULT IWindowBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(pm.m_aCustomEvents){
		TEventUI mEvent = {0};
		mEvent.Type			= uMsg;
		mEvent.lParam		= lParam;
		mEvent.wParam		= wParam;
		mEvent.ptMouse.x	= 0;
		mEvent.ptMouse.y	= 0;
		mEvent.wKeyState	= 0;
		mEvent.dwTimestamp	= GetTickCount();
		mEvent.pSender		= NULL;
		mEvent.chKey		= NULL;

		bHandled = !pm.m_aCustomEvents(&mEvent);

		return bHandled;
	}
	bHandled = FALSE;
	return 0;
}

void IWindowBase::Notify( TNotifyUI& msg )
{
	try
	{
		if( msg.sType == _T("click") )//鼠标点击消息
		{
			if( msg.pSender == mpCloseBtn )
			{
				Close(IDOK);
			}
			else if( msg.pSender == mpMinBtn )
			{ 
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);return;
			}
			else if( msg.pSender == mpMaxBtn)
			{
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);return;
			}
			else if( msg.pSender == mpRestoreBtn)
			{
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
			}
		}
	}
	catch(...)
	{
		throw "IWindowBase::Notify";
	}
}

LRESULT IWindowBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:         lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:          lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:        lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;

	case WM_NCACTIVATE:     lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:     lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:        lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:      lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:  lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;

	case WM_SIZE:           lRes = OnSize(uMsg, wParam, lParam, bHandled); break;  
	case WM_SYSCOMMAND:     lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:        lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:      lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:       lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONUP:      lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONDOWN:    lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	default:                bHandled = FALSE; break;
	}
	if (bHandled) return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled)
		return lRes;

	if (pm.MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

//************************************
// Method:    MessageHandler
// FullName:  IWindowBase::MessageHandler
// Access:    virtual public 
// Returns:   LRESULT
// Qualifier:
// Parameter: UINT uMsg
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Parameter: bool & bHandled
// Note:	  
//************************************
LRESULT IWindowBase::MessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled )
{
	try
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return ResponseDefaultKeyEvent(wParam);
			default:
				break;
			}
		}
		return FALSE;
	}
	catch(...)
	{
		throw "IWindowBase::MessageHandler";
	}
}

LRESULT IWindowBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		Close();
		return TRUE;
	}

	return FALSE;
}

//************************************
// Method:    GetPaintManager
// FullName:  IWindowBase::GetPaintManager
// Access:    public 
// Returns:   CPaintManagerUI*
// Qualifier:
// Note:	  
//************************************
CPaintManagerUI* IWindowBase::GetPaintManager()
{
	try
	{
		return &pm;
	}
	catch(...)
	{
		throw "IWindowBase::GetPaintManager";
	}
}

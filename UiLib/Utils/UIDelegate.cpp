#include "StdAfx.h"

namespace UiLib {


	//************************************
	// ��������: CDelegateBase
	// ��������: 
	// ������Ϣ: void * pObject
	// ������Ϣ: void * pFn
	// ����˵��: 
	//************************************
	CDelegateBase::CDelegateBase( void* pObject, void* pFn,LPARAM lParam /*= NULL*/,WPARAM wParam /*= NULL*/ )
	{
		m_pObject = pObject;
		m_pFn = pFn;
		m_iEventType = UIEVENT__ALL;
		m_lParam = lParam;
		m_wParam = wParam;
		m_sNotifyTypeName.Empty();
	}

	//************************************
	// ��������: CDelegateBase
	// ��������: 
	// ������Ϣ: void * pObject
	// ������Ϣ: void * pFn
	// ������Ϣ: UINT _iEventType
	// ����˵��: 
	//************************************
	CDelegateBase::CDelegateBase( void* pObject, void* pFn,UINT _iEventType /*= UIEVENT__ALL*/,LPARAM lParam /*= NULL*/,WPARAM wParam /*= NULL*/ )
	{
		m_pObject = pObject;
		m_pFn = pFn;
		m_iEventType = _iEventType;
		m_lParam = lParam;
		m_wParam = wParam;
		m_sNotifyTypeName.Empty();
	}

	//************************************
	// ��������: CDelegateBase
	// ��������: 
	// ������Ϣ: void * pObject
	// ������Ϣ: void * pFn
	// ������Ϣ: LPCTSTR _sNotifyTypeName
	// ����˵��: 
	//************************************
	CDelegateBase::CDelegateBase( void* pObject, void* pFn,LPCTSTR _sNotifyTypeName /*= NULL*/,LPARAM lParam /*= NULL*/,WPARAM wParam /*= NULL*/ )
	{
		m_pObject = pObject;
		m_pFn = pFn;
		m_iEventType = UIEVENT__ALL;
		m_lParam = lParam;
		m_wParam = wParam;
		if(NULL != _sNotifyTypeName)
			m_sNotifyTypeName = _sNotifyTypeName;
	}

	//************************************
	// ��������: CDelegateBase
	// ��������: 
	// ������Ϣ: const CDelegateBase & rhs
	// ����˵����
	//************************************
	CDelegateBase::CDelegateBase( const CDelegateBase& rhs )
	{
		m_pObject = rhs.m_pObject;
		m_pFn = rhs.m_pFn; 
		m_iEventType = rhs.m_iEventType;
		m_lParam = rhs.m_lParam;
		m_wParam = rhs.m_wParam;
		m_sNotifyTypeName = rhs.m_sNotifyTypeName.GetData();
	}

	CDelegateBase::~CDelegateBase()
	{
		if(!m_sNotifyTypeName.IsEmpty())
			m_sNotifyTypeName.Empty();
	}

	bool CDelegateBase::Equals(const CDelegateBase& rhs) const 
	{
		return m_pObject == rhs.m_pObject && m_pFn == rhs.m_pFn && m_iEventType == rhs.m_iEventType && m_sNotifyTypeName == rhs.m_sNotifyTypeName.GetData(); 
	}
	
	//************************************
	// ��������: CEventSource
	// ��������: 
	// ����˵��: 
	//************************************
	CEventSource::CEventSource()
	{
		m_aDelegates.Empty();
	}

	CEventSource::~CEventSource()
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject) delete pObject;
			pObject = NULL;
		}
	}

	CEventSource::operator bool()
	{
		return m_aDelegates.GetSize() > 0;
	}

	void CEventSource::operator+= (const CDelegateBase& d)
	{ 
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && pObject->Equals(d) ) return;
		}

		m_aDelegates.Add(d.Copy());
	}
	
	void CEventSource::operator-= (const CDelegateBase& d) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && pObject->Equals(d) ) {
				delete pObject;
				m_aDelegates.Remove(i);
				return;
			}
		}
	}
		
	bool CEventSource::operator() (void* param) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && !pObject->Invoke(param,pObject->GetLParam(),pObject->GetWParam()) ) return false;
		}
		return true;
	}

	bool CEventSource::operator() (TEventUI* pTEventUI) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && !pObject->Invoke(pTEventUI,pObject->GetLParam(),pObject->GetWParam()) ) return false;
		}
		return true;
	}

	bool CEventSource::operator() (TNotifyUI* pTNotifyUI) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBase* pObject = static_cast<CDelegateBase*>(m_aDelegates[i]);
			if( pObject && !pObject->Invoke(pTNotifyUI,pObject->GetLParam(),pObject->GetWParam()) ) return false;
		}
		return true;
	}


} // namespace UiLib
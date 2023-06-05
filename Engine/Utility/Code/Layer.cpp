#include "..\..\Header\Layer.h"
#include "Export_Utility.h"

USING(Engine)

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;

	m_mapObject.insert({ pObjTag, pInstance });

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return E_FAIL;

	CGameObject * pInstance = iter->second;

	Safe_Release<CGameObject*>(pInstance);

	iter = m_mapObject.erase(iter);

	return S_OK;
}

HRESULT CLayer::Add_GameList(CGameObject * pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;

	m_ObjectList.push_back(pInstance);

	return S_OK;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int iResult = 0;

	if (!m_ObjectList.empty())
	{
		list<CGameObject*>::iterator iter;

		for (iter = m_ObjectList.begin(); iter != m_ObjectList.end();)
		{
			iResult = (*iter)->Update_Object(fTimeDelta);
	
			if (iResult & 0x80000000)
			{
				if (this == Get_Layer(L"STAGE_BULLET_Layer"))
				{
					iter = m_ObjectList.erase(iter);
				}
				else if (this == Get_Layer(L"STAGE_PLAYERBULLET_Layer"))
				{
					iter = m_ObjectList.erase(iter);
				}
				else if (this == Get_Layer(L"STAGE_EXBULLET_Layer"))
				{
					iter = m_ObjectList.erase(iter);
				}
				else if (this == Get_Layer(L"STAGE_KRAKENBULLET_Layer"))
				{
					iter = m_ObjectList.erase(iter);
				}
				else if (this == Get_Layer(L"STAGE_LASER_Layer"))
				{
					iter = m_ObjectList.erase(iter);
				}
				else
				{
					Safe_Release(*iter);
					iter = m_ObjectList.erase(iter);
				}
			}
			else
				++(iter);
		}
	}
	else if (!m_ObjectPairList.empty())
	{
		list<pair<const _tchar*, CGameObject*>>::iterator iter;

		for (iter = m_ObjectPairList.begin(); iter != m_ObjectPairList.end();)
		{
			iResult = iter->second->Update_Object(fTimeDelta);

			if (iResult & 0x80000000)
			{
				Safe_Release(iter->second);
				m_ObjectPairList.erase(iter++);
			}
			else
				++iter;
		}
	}
	else
	{
		map<const _tchar*, CGameObject*>::iterator iter;

		for (iter = m_mapObject.begin(); iter != m_mapObject.end();)
		{
			iResult = iter->second->Update_Object(fTimeDelta);

			if (iResult & 0x80000000)
			{
				Safe_Release(iter->second);
				m_mapObject.erase(iter++);
			}
			else
				++iter;
		}
	}

	return 0;
}

void CLayer::LateUpdate_Layer(void)
{
	if (!m_ObjectList.empty())
	{
		for (auto& iter : m_ObjectList)
			iter->LateUpdate_Object();
	}
	else if (!m_ObjectPairList.empty())
	{
		for (auto& iter : m_ObjectPairList)
			iter.second->LateUpdate_Object();
	}
	else
	{
		for (auto& iter : m_mapObject)
			iter.second->LateUpdate_Object();
	}
}

HRESULT CLayer::Add_GamePair(const _tchar * pObjTag, CGameObject * pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;

	m_ObjectPairList.push_back({ pObjTag, pInstance });

	return S_OK;
}

HRESULT CLayer::Delete_GamePair(const _tchar * pObjTag)
{
	auto	iter = find_if(m_ObjectPairList.begin(), m_ObjectPairList.end(), CTag_Finder(pObjTag));

	if (iter == m_ObjectPairList.end())
		return E_FAIL;

	CGameObject * pInstance = iter->second;

	Safe_Release<CGameObject*>(pInstance);

	iter = m_ObjectPairList.erase(iter);

	return S_OK;
}

CGameObject * CLayer::Find_GamePair(const _tchar * pObjTag)
{
	auto	iter = find_if(m_ObjectPairList.begin(), m_ObjectPairList.end(), CTag_Finder(pObjTag));

	if (iter == m_ObjectPairList.end())
	{
		MSG_BOX("There is no Pair");
		return nullptr;
	}

	return iter->second;
}

CComponent * CLayer::Get_PairComponent(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_ObjectPairList.begin(), m_ObjectPairList.end(), CTag_Finder(pObjTag));

	if (iter == m_ObjectPairList.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CLayer* CLayer::Create(void)
{
	CLayer*	pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
		Safe_Release(pLayer);

	return pLayer;
}

void CLayer::Free(void)
{
	for_each(m_ObjectList.begin(), m_ObjectList.end(), CDeleteObj());
	m_ObjectList.clear();

	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();

	for_each(m_ObjectPairList.begin(), m_ObjectPairList.end(), CDeleteMap());
	m_ObjectPairList.clear();
}

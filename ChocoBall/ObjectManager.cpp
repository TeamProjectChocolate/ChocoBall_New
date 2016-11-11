#include "stdafx.h"
#include "ObjectManager.h"
#include "C2DImage.h"
#include "Player.h"
#include "RenderContext.h"

CObjectManager* CObjectManager::m_instance = nullptr;

void CObjectManager::AddObject(CGameObject* Object, LPCSTR ObjectName, PRIORTY priorty,bool common){
	if (priorty > PRIORTY::MAX_PRIORTY){
		priorty = PRIORTY::MAX_PRIORTY;
	}
	Object->SetCommon(common);
	this->Add(Object,ObjectName,priorty);
}

void CObjectManager::AddObject(CGameObject* Object,LPCSTR ObjectName,bool common){
	PRIORTY priorty = PRIORTY::MAX_PRIORTY;
	Object->SetCommon(common);
	this->Add(Object,ObjectName, priorty);
}

void CObjectManager::Add(CGameObject* GameObject,LPCSTR ObjectName, PRIORTY priority){
	OBJECT_DATA* Obj;
	Obj = new OBJECT_DATA;
	CH_ASSERT(strlen(ObjectName) < OBJECTNAME_MAX);
	strcpy(Obj->objectname, ObjectName);		// コピーせずにアドレスを保持させると、ローカル変数が渡された際にクラッシュする
	Obj->object = GameObject;
	Obj->priority = priority;
	m_GameObjects.push_back(Obj);
}

void CObjectManager::DeleteGameObject(LPCSTR ObjectName){
	int size = m_GameObjects.size();
	for (int idx = 0; idx < size; idx++){
		if (!strcmp(m_GameObjects[idx]->objectname, ObjectName)){
			m_GameObjects[idx]->object->OnDestroy();
			m_DeleteObjects.push_back(m_GameObjects[idx]->object);
			return;
		}
	}
	MessageBox(NULL, "オブジェクトが登録されていません", 0, 0);
}

void CObjectManager::DeleteGameObject(CGameObject* pObject){
	int size = m_GameObjects.size();
	pObject->OnDestroy();
	m_DeleteObjects.push_back(pObject);
}

void CObjectManager::DeleteGameObjectImmediate(CGameObject* pObject)
{
	pObject->OnDestroy();
	vector<OBJECT_DATA*>::iterator itr;
	for (itr = m_GameObjects.begin(); itr != m_GameObjects.end(); itr++){
		if (pObject == (*itr)->object){
			if ((*itr)->object->GetManagerNewFlg()){
				SAFE_DELETE(pObject);
			}
			itr = m_GameObjects.erase(itr);
			break;
		}
	}
}
void CObjectManager::CleanManager(){
	int size = m_GameObjects.size();
	for (int idx = 0; idx < size; idx++){
		if (!m_GameObjects[idx]->object->GetCommon()){
			m_GameObjects[idx]->object->OnDestroy();
			m_DeleteObjects.push_back(m_GameObjects[idx]->object);
		}
	}
}

void CObjectManager::ExcuteDeleteObjects(){
	vector<OBJECT_DATA*>::iterator itr;
	vector<CGameObject*>::iterator itr2;
	for (int priorty = PRIORTY::MAX_PRIORTY; priorty >= 0; priorty--){
		for (itr = m_GameObjects.begin(); itr != m_GameObjects.end();){
			bool inclimentFlg = true;
			if (priorty == (*itr)->priority){
				for (itr2 = m_DeleteObjects.begin(); itr2 != m_DeleteObjects.end();){
					if ((*itr2) == (*itr)->object){
						if ((*itr)->object->GetManagerNewFlg()){
							SAFE_DELETE((*itr)->object);
							SAFE_DELETE((*itr));
						}
						itr = m_GameObjects.erase(itr);
						itr2 = m_DeleteObjects.erase(itr2);
						inclimentFlg = false;
						break;
					}
					else{
						itr2++;
					}
				}
			}
			int size = m_DeleteObjects.size();
			if (size == 0){
				return;
			}
			if (inclimentFlg){
				itr++;
			}
		}
	}
}

void CObjectManager::Intialize(){
	int size = m_GameObjects.size();
	for (int priorty = 0; priorty <= PRIORTY::MAX_PRIORTY; priorty++){
		for (int idx = 0; idx < size; idx++){
			if (m_GameObjects[idx]->priority == priorty){
				if (!(m_GameObjects[idx]->object->GetOriginal())){
					m_GameObjects[idx]->object->Initialize();
				}
			}
		}
	}
}

void CObjectManager::Update(){
	int size = m_GameObjects.size();
	for (short priorty = 0; priorty <= PRIORTY::MAX_PRIORTY;priorty++){	// 優先度の高いものから更新
		for (int idx = 0; idx < size; idx++){
			if (m_GameObjects[idx]->object->GetAlive()){	// 生存しているもののみ更新
				if (m_GameObjects[idx]->priority == priorty){	// 現在の優先度と一致するものを更新
					m_GameObjects[idx]->object->Update();
				}
			}
		}
	}
}

void CObjectManager::Draw(){
	SINSTANCE(CRenderContext)->RenderingStart();

	int size = m_GameObjects.size();

	// 3Dの描画
	{
		// 基本描画
		for (short priorty = 0; priorty < PRIORTY::PARTICLE; priorty++){	// 優先度の高いものから更新
			for (int idx = 0; idx < size; idx++){

				if (m_GameObjects[idx]->object->GetAlive()){	// 生存しているもののみ描画
					if (m_GameObjects[idx]->priority == priorty){	// 現在の優先度と一致するものを描画
#ifdef NOT_DOF
#else
						m_GameObjects[idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
						m_GameObjects[idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
#endif
						m_GameObjects[idx]->object->Draw();
					}
				}
			}
		}
		// 蓄積したデータでインスタンシング描画
		vector<RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER_STATE::Instancing);
		for (auto data : datas){
			data->render->Draw();
		}
		// パーティクル描画
		for (short priorty = PRIORTY::PARTICLE; priorty < PRIORTY::OBJECT2D; priorty++){	// 優先度の高いものから更新
			for (int idx = 0; idx < size; idx++){

				if (m_GameObjects[idx]->object->GetAlive()){	// 生存しているもののみ描画
					if (m_GameObjects[idx]->priority == priorty){	// 現在の優先度と一致するものを描画
#ifdef NOT_DOF
#else
						m_GameObjects[idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
						m_GameObjects[idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
#endif
						m_GameObjects[idx]->object->Draw();
					}
				}
			}
		}
	}

	SINSTANCE(CRenderContext)->RenderingEnd();

	SINSTANCE(CRenderContext)->SetRenderingBuffer();

	// 2Dの描画
	for (short priorty = PRIORTY::OBJECT2D; priorty <= PRIORTY::MAX_PRIORTY; priorty++){	// 優先度の高いものから更新
		for (int idx = 0; idx < size; idx++){

			if (m_GameObjects[idx]->object->GetAlive()){	// 生存しているもののみ描画
				if (m_GameObjects[idx]->priority == priorty){	// 現在の優先度と一致するものを描画
					m_GameObjects[idx]->object->Draw();
				}
			}
		}
	}
}

void CObjectManager::DeleteAll(){
	for (int idx = 0, size = m_GameObjects.size(); idx < size; idx++){
		SAFE_DELETE(m_GameObjects[idx]->object);
		SAFE_DELETE(m_GameObjects[idx]);
	}
	m_GameObjects.clear();
}
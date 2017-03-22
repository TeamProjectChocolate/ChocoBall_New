#include "stdafx.h"
#include "ObjectManager.h"
#include "C2DImage.h"
#include "Player.h"
#include "RenderContext.h"

CObjectManager* CObjectManager::m_instance = nullptr;

void CObjectManager::OnCreate() {
	// 優先度の数だけ配列を生成。
	for (int idx = 0; idx < OBJECT::PRIORTY::MAX_PRIORTY; idx++) {
		m_GameObjects.push_back(vector<OBJECT_DATA*>());
	}
}

void CObjectManager::AddObject(CGameObject* Object, LPCSTR ObjectName, OBJECT::PRIORTY priorty,bool common){
	if (priorty > OBJECT::PRIORTY::MAX_PRIORTY){
		priorty = OBJECT::PRIORTY::MAX_PRIORTY;
	}
	Object->SetCommon(common);
	this->Add(Object,ObjectName,priorty);
}

void CObjectManager::AddObject(CGameObject* Object,LPCSTR ObjectName,bool common){
	OBJECT::PRIORTY priorty = OBJECT::PRIORTY::MAX_PRIORTY;
	Object->SetCommon(common);
	this->Add(Object,ObjectName, priorty);
}

void CObjectManager::Add(CGameObject* GameObject,LPCSTR ObjectName, OBJECT::PRIORTY priority){
	OBJECT_DATA* Obj;
	Obj = new OBJECT_DATA;
	CH_ASSERT(strlen(ObjectName) < OBJECTNAME_MAX);
	strcpy(Obj->objectname, ObjectName);		// コピーせずにアドレスを保持させると、ローカル変数が渡された際にクラッシュする
	Obj->object = GameObject;
	m_GameObjects[priority].push_back(Obj);
}

void CObjectManager::DeleteGameObject(LPCSTR ObjectName){
	int size = m_GameObjects.size();
	for (int idx = 0; idx < size; idx++){
		int size2 = m_GameObjects[idx].size();
		for(int idx2 = 0;idx2 < size2;idx2++){
			if (!strcmp(m_GameObjects[idx][idx2]->objectname, ObjectName)) {
				m_GameObjects[idx][idx2]->object->OnDestroy();
				m_DeleteObjects.push_back(m_GameObjects[idx][idx2]->object);
				return;
			}
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
	if (pObject == nullptr) {
		return;
	}
	pObject->OnDestroy();
	vector<OBJECT_DATA*>::iterator itr;
	int size = m_GameObjects.size();
	for(int idx = 0;idx < size;idx++){
		for (itr = m_GameObjects[idx].begin(); itr != m_GameObjects[idx].end(); itr++) {
			if (pObject == (*itr)->object) {
				if ((*itr)->object->GetManagerNewFlg()) {
					SAFE_DELETE(pObject);
				}
				itr = m_GameObjects[idx].erase(itr);
				break;
			}
		}
	}
}
void CObjectManager::CleanManager(){
	int size = m_GameObjects.size();
	for (int idx = 0; idx < size; idx++){
		int size2 = m_GameObjects[idx].size();
		for (int idx2 = 0; idx2 < size2; idx2++) {
			if (!m_GameObjects[idx][idx2]->object->GetCommon()) {
				m_GameObjects[idx][idx2]->object->OnDestroy();
				m_DeleteObjects.push_back(m_GameObjects[idx][idx2]->object);
			}
		}
	}
}

void CObjectManager::ExcuteDeleteObjects(){
	vector<OBJECT_DATA*>::iterator itr;
	vector<CGameObject*>::iterator itr2;
	for (int priorty = OBJECT::PRIORTY::MAX_PRIORTY - 1; priorty >= 0; priorty--) {
		for (itr = m_GameObjects[priorty].begin(); itr != m_GameObjects[priorty].end();) {
			bool inclimentFlg = true;
			for (itr2 = m_DeleteObjects.begin(); itr2 != m_DeleteObjects.end();) {
				if ((*itr2) == (*itr)->object) {
					if ((*itr)->object->GetManagerNewFlg()) {
						SAFE_DELETE((*itr)->object);
						SAFE_DELETE((*itr));
					}
					itr = m_GameObjects[priorty].erase(itr);
					itr2 = m_DeleteObjects.erase(itr2);
					inclimentFlg = false;
					break;
				}
				else {
					itr2++;
				}
			}
			int size = m_DeleteObjects.size();
			if (size == 0) {
				return;
			}
			if (inclimentFlg) {
				itr++;
			}
		}
	}
	m_DeleteObjects.clear();
}

void CObjectManager::Intialize(){
	for (int priorty = 0; priorty < OBJECT::PRIORTY::MAX_PRIORTY; priorty++){
		int size = m_GameObjects[priorty].size();
		for (int idx = 0; idx < size; idx++){
				if (!(m_GameObjects[priorty][idx]->object->GetOriginal())){
					m_GameObjects[priorty][idx]->object->Initialize();
				}
		}
	}
}

void CObjectManager::Update(){
	for (short priorty = 0; priorty < OBJECT::PRIORTY::MAX_PRIORTY;priorty++){	// 優先度の高いものから更新
		int size = m_GameObjects[priorty].size();
		for (int idx = 0; idx < size; idx++){
			if (m_GameObjects[priorty][idx]->object->GetAlive()){	// 生存しているもののみ更新
					m_GameObjects[priorty][idx]->object->Update();
			}
		}
	}
}

void CObjectManager::Draw(){
	SINSTANCE(CRenderContext)->RenderingStart();

	// 各種描画のための設定。
	{
		for (short priorty = 0; priorty < OBJECT::PRIORTY::EMITTER; priorty++) {	// 優先度の高いものから
			int size = m_GameObjects[priorty].size();
			for (int idx = 0; idx < size; idx++) {
				if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// 生存しているもののみ。
					m_GameObjects[priorty][idx]->object->Draw();
				}
			}
		}
	}

	// 3Dの描画
	{
		// 基本描画(不透明3Dオブジェクトまで)。
		for (short priorty = OBJECT::PRIORTY::EMITTER; priorty < OBJECT::PRIORTY::OBJECT3D_ALPHA; priorty++) {	// 優先度の高いものから更新
			int size = m_GameObjects[priorty].size();
			for (int idx = 0; idx < size; idx++){
				if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// 生存しているもののみ描画
#ifdef NOT_DOF
#else
					m_GameObjects[priorty][idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
					m_GameObjects[priorty][idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
#endif
					m_GameObjects[priorty][idx]->object->Draw();
				}
			}
		}
		// 蓄積したデータでインスタンシング描画(不透明)。
		vector<CRenderContext::RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER::TYPE::Instancing);
		for (auto data : datas){
			data->render->Draw();
		}
		// 3Dオブジェクト(半透明)描画。
		for (short priorty = OBJECT::PRIORTY::OBJECT3D_ALPHA; priorty < OBJECT::PRIORTY::PARTICLE; priorty++){	// 優先度の高いものから更新
			int size = m_GameObjects[priorty].size();
			for (int idx = 0; idx < size; idx++){

				if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// 生存しているもののみ描画
#ifdef NOT_DOF
#else
					m_GameObjects[priorty][idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
					m_GameObjects[priorty][idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
#endif
					(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, true);
					(*graphicsDevice()).SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
					m_GameObjects[priorty][idx]->object->Draw();
					(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, true);
					(*graphicsDevice()).SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				}
			}
		}
		// 蓄積したデータでインスタンシング描画(半透明)。
		vector<CRenderContext::RENDER_DATA*> datas_alpha = SINSTANCE(CRenderContext)->GetRenderArray(RENDER::TYPE::Instancing_Alpha);
		for (auto data : datas_alpha) {
			data->render->Draw();
		}
	}

	// パーティクル描画。
	for (short priorty = OBJECT::PRIORTY::PARTICLE; priorty < OBJECT::PRIORTY::OBJECT2D; priorty++) {	// 優先度の高いものから更新
		int size = m_GameObjects[priorty].size();
		for (int idx = 0; idx < size; idx++) {

			if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// 生存しているもののみ描画
#ifdef NOT_DOF
#else
				m_GameObjects[priorty][idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
				m_GameObjects[priorty][idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
#endif
				m_GameObjects[priorty][idx]->object->Draw();
			}
		}
	}


	SINSTANCE(CRenderContext)->RenderingEnd();

	SINSTANCE(CRenderContext)->SetRenderingBuffer();

	// 2Dの描画
	for (short priorty = OBJECT::PRIORTY::OBJECT2D; priorty < OBJECT::PRIORTY::MAX_PRIORTY; priorty++){	// 優先度の高いものから更新
		int size = m_GameObjects[priorty].size();
		for (int idx = 0; idx < size; idx++){
			if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// 生存しているもののみ描画
				m_GameObjects[priorty][idx]->object->Draw();
			}
		}
	}
}

void CObjectManager::DeleteAll(){
	for (int priorty = 0; priorty < OBJECT::PRIORTY::MAX_PRIORTY; priorty++) {
		for (int idx = 0, size = m_GameObjects.size(); idx < size; idx++) {
			if (m_GameObjects[priorty][idx]->object->GetManagerNewFlg()) {
				SAFE_DELETE(m_GameObjects[priorty][idx]->object);
				SAFE_DELETE(m_GameObjects[priorty][idx]);
			}
		}
		m_GameObjects[priorty].clear();
	}
	m_GameObjects.clear();
}
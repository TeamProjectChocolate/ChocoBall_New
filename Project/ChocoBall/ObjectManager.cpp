#include "stdafx.h"
#include "ObjectManager.h"
#include "C2DImage.h"
#include "Player.h"
#include "RenderContext.h"

CObjectManager* CObjectManager::m_instance = nullptr;

void CObjectManager::OnCreate() {
	// �D��x�̐������z��𐶐��B
	for (int idx = 0; idx < OBJECT::PRIORTY::MAX_PRIORTY; idx++) {
		m_GameObjects.push_back(vector<OBJECT_DATA*>());
	}
}

void CObjectManager::AddObject(CGameObject* Object, LPCSTR ObjectName, OBJECT::PRIORTY priorty,bool common){
	if (priorty > OBJECT::PRIORTY::MAX_PRIORTY){
		priorty = OBJECT::PRIORTY::MAX_PRIORTY;
	}
	else if(priorty < 0) {
		// �͈͊O�A�N�Z�X(-1�ȉ�)�B
		abort();
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
	strcpy(Obj->objectname, ObjectName);		// �R�s�[�����ɃA�h���X��ێ�������ƁA���[�J���ϐ����n���ꂽ�ۂɃN���b�V������
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
	MessageBox(NULL, "�I�u�W�F�N�g���o�^����Ă��܂���", 0, 0);
}

void CObjectManager::DeleteGameObject(CGameObject* pObject){
	pObject->OnDestroy();
	m_DeleteObjects.push_back(pObject);
}

void CObjectManager::DeleteGameObjectImmediate(CGameObject* pObject)
{
	if (pObject == nullptr) {
		return;
	}
	pObject->OnDestroy();
	int size = m_GameObjects.size();
	for(int idx = 0;idx < size;idx++){
		for (auto itr = m_GameObjects[idx].begin(); itr != m_GameObjects[idx].end(); itr++) {
			if (pObject == (*itr)->object) {
				if ((*itr)->object->GetManagerNewFlg()) {
					SAFE_DELETE((*itr)->object);
				}
				else {
					(*itr)->object = nullptr;
				}
				SAFE_DELETE((*itr));

				pObject = nullptr;
				itr = m_GameObjects[idx].erase(itr);
				return;
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

	for (int priorty = OBJECT::PRIORTY::MAX_PRIORTY - 1; priorty >= 0; priorty--) {
		for (auto itr = m_GameObjects[priorty].begin(); itr != m_GameObjects[priorty].end();) {
			// �o�^����Ă���I�u�W�F�N�g�ō폜�z��������B
			auto DeleteItr = find(m_DeleteObjects.begin(), m_DeleteObjects.end(), (*itr)->object);

			if (DeleteItr != m_DeleteObjects.end()) {
				// �폜�z��ɐς܂�Ă���B
				// �폜����v�f����������ޔ��B
				OBJECT_DATA* DeleteObject = *itr;

				// ��d�폜�A�폜�ςݗ̈�Q�Ƃ̑΍�B
				{
					// �C�e���[�^�[���ɐi�߂�B
					itr = m_GameObjects[priorty].erase(itr);
					//// itr�ɓ����v�f�������Ă����ꍇ�͂�������폜�B
					//m_GameObjects[priorty].erase(remove_if(m_GameObjects[priorty].begin(), m_GameObjects[priorty].end(), callback.CallBack));

					// m_DeleteObjects�̗v�f���폜(�����A�h���X�����������Ă����ꍇ�͂�������������)�B
					m_DeleteObjects.erase(remove(m_DeleteObjects.begin(), m_DeleteObjects.end(), DeleteObject->object),m_DeleteObjects.end());
				}

				// �폜���s�B
				if (DeleteObject->object->GetManagerNewFlg()) {
					// GameObject�̃������̈�J��
					SAFE_DELETE(DeleteObject->object);
				}
				else {
					// �}�l�[�W���Ń������m�ۂ��ꂽ���̂ł͂Ȃ����ߔz�񂩂��菜�������B
					DeleteObject->object = nullptr;
				}
				SAFE_DELETE(DeleteObject);
			}
			else {
				// �폜�z��ɂȂ��B
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
	for (short priorty = 0; priorty < OBJECT::PRIORTY::MAX_PRIORTY;priorty++){	// �D��x�̍������̂���X�V
		int size = m_GameObjects[priorty].size();
		for (int idx = 0; idx < size; idx++){
			if (m_GameObjects[priorty][idx]->object->GetAlive()){	// �������Ă�����̂̂ݍX�V
					m_GameObjects[priorty][idx]->object->Update();
			}
		}
	}
}

void CObjectManager::Draw(){
	SINSTANCE(CRenderContext)->RenderingStart();

	// �e��`��̂��߂̐ݒ�B
	{
		for (short priorty = 0; priorty < OBJECT::PRIORTY::EMITTER; priorty++) {	// �D��x�̍������̂���
			int size = m_GameObjects[priorty].size();
			for (int idx = 0; idx < size; idx++) {
				if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// �������Ă�����̂̂݁B
					m_GameObjects[priorty][idx]->object->Draw();
				}
			}
		}
	}

	// 3D�̕`��
	{
		// ��{�`��(�s����3D�I�u�W�F�N�g�܂�)�B
		for (short priorty = OBJECT::PRIORTY::EMITTER; priorty < OBJECT::PRIORTY::OBJECT3D_ALPHA; priorty++) {	// �D��x�̍������̂���X�V
			int size = m_GameObjects[priorty].size();
			for (int idx = 0; idx < size; idx++){
				if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// �������Ă�����̂̂ݕ`��
#ifdef NOT_DOF
#else
					m_GameObjects[priorty][idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
					m_GameObjects[priorty][idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
#endif
					m_GameObjects[priorty][idx]->object->Draw();
				}
			}
		}
		// �~�ς����f�[�^�ŃC���X�^���V���O�`��(�s����)�B
		const vector<CRenderContext::RENDER_DATA*>& datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER::TYPE::Instancing);
		for (auto data : datas){
			data->render->Draw();
		}
		// 3D�I�u�W�F�N�g(������)�`��B
		for (short priorty = OBJECT::PRIORTY::OBJECT3D_ALPHA; priorty < OBJECT::PRIORTY::PARTICLE; priorty++){	// �D��x�̍������̂���X�V
			int size = m_GameObjects[priorty].size();
			for (int idx = 0; idx < size; idx++){

				if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// �������Ă�����̂̂ݕ`��
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
		// �~�ς����f�[�^�ŃC���X�^���V���O�`��(������)�B
		const vector<CRenderContext::RENDER_DATA*>& datas_alpha = SINSTANCE(CRenderContext)->GetRenderArray(RENDER::TYPE::Instancing_Alpha);
		for (auto data : datas_alpha) {
			data->render->Draw();
		}
	}

	// �p�[�e�B�N���`��B
	for (short priorty = OBJECT::PRIORTY::PARTICLE; priorty < OBJECT::PRIORTY::OBJECT2D; priorty++) {	// �D��x�̍������̂���X�V
		int size = m_GameObjects[priorty].size();
		for (int idx = 0; idx < size; idx++) {

			if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// �������Ă�����̂̂ݕ`��
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

	// 2D�̕`��
	for (short priorty = OBJECT::PRIORTY::OBJECT2D; priorty < OBJECT::PRIORTY::MAX_PRIORTY; priorty++){	// �D��x�̍������̂���X�V
		int size = m_GameObjects[priorty].size();
		for (int idx = 0; idx < size; idx++){
			if (m_GameObjects[priorty][idx]->object->GetAlive()) {	// �������Ă�����̂̂ݕ`��
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
#include "stdafx.h"
#include "MapObjectManager.h"
#include "ObjectManager.h"
#include "CollisionType.h"

CMapObjectManager::CMapObjectManager() {
	m_myMotionState = nullptr;
}

CMapObjectManager::~CMapObjectManager() {

}

void CMapObjectManager::Initialize() {
	SetAlive(true);

	//���̂��������B
	{
		//���̈����ɓn���̂̓{�b�N�X��halfsize�Ȃ̂ŁA0.5�{����B
		SCollisionInfo* Table = collisionInfoTableArray[m_StageID];
		int arraySize = collisionInfoTableSizeArray[m_StageID];	//�z��̗v�f����Ԃ��B
		for (int i = 0; i < arraySize; i++) {
			SCollisionInfo& collision = Table[i];
			btBoxShape* work = new btBoxShape(btVector3(collision.scale.x*0.5f, collision.scale.y*0.5f, collision.scale.z*0.5f));
			m_groundShape.push_back(work);
			btTransform groundTransform;
			groundTransform.setIdentity();
			groundTransform.setOrigin(btVector3(-collision.pos.x, collision.pos.y, -collision.pos.z));
			groundTransform.setRotation(btQuaternion(-collision.rotation.x, collision.rotation.y, -collision.rotation.z, collision.rotation.w));
			float mass = 0.0f;

			//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
			m_myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, work, btVector3(0, 0, 0));
			btRigidBody* work2 = new btRigidBody(rbInfo);
			m_rigidBody.push_back(work2);
			work2->activate();
			m_rigidBody[i]->setUserIndex(CollisionType_Map);
			//���[���h�ɒǉ��B
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(work2);
		}
	}

	// �}�b�v�`�b�v��z�u�B
	{
		MapObject* MapObjects = MapObjectsPointerArray[m_StageID];
		if (MapObjectsMaxArray[m_StageID] == 1) {
			if (!strcmp(MapObjects[0].FileName, "NotData")) {
				// ��񂪏o�͂���ĂȂ��Ƃ��B
				return;
			}
		}
		for (int idx = 0; idx < MapObjectsMaxArray[m_StageID];idx++) {
			CField* WorkObj = new CField;
			string name = "image/";
			name = name + static_cast<string>(MapObjects[idx].FileName);
			WorkObj->SetStageID(m_StageID);
			WorkObj->SetFileName(name.c_str());
			WorkObj->Initialize();
			// Unity��DirectX�ł͍��W�n�����]���Ă��邽�߁A�o�̓f�[�^�������G���Ă���i�[����B
			WorkObj->SetPos(MapObjects[idx].Position);
			WorkObj->SetQuaternion(MapObjects[idx].Rotation);
			D3DXVECTOR3 scale = MapObjects[idx].Scale;
			WorkObj->SetScale(MapObjects[idx].Scale);
			if (static_cast<REFRACTIVES>(MapObjects[idx].RType) == REFRACTIVES::NONE) {
				WorkObj->SetIsFresnel(false);
			}
			else {
				WorkObj->SetIsFresnel(true);
				WorkObj->SetRefractive(static_cast<REFRACTIVES>(MapObjects[idx].RType));
			}
			WorkObj->SetAlpha(MapObjects[idx].Alpha);
			m_MapObjects.push_back(WorkObj);
		}
	}
}

void CMapObjectManager::Update() {
	for (auto obj : m_MapObjects) {
		obj->Update();
	}
}

void CMapObjectManager::Draw() {
	if (m_czbuffersphere == NULL) {
		m_czbuffersphere = SINSTANCE(CObjectManager)->FindGameObject<CZBufferSphere>(_T("ZBufferSphere"));
	}
	for (auto obj : m_MapObjects) {
		obj->SetZBufferSphere(m_czbuffersphere);
		obj->Draw();
	}
}

void CMapObjectManager::DrawShadow(CCamera* camera) {
	for (auto obj : m_MapObjects) {
		obj->DrawShadow(camera);
	}
}

void CMapObjectManager::Draw_EM(CCamera* camera) {
	for (auto obj : m_MapObjects) {
		obj->Draw_EM(camera);
	}
}

void CMapObjectManager::Is_DrawShadow_Use_Horizon() {
	for (auto obj : m_MapObjects) {
		obj->Is_DrawShadow_Use_Horizon();
	}
}

void CMapObjectManager::SetPintoWorld(const D3DXMATRIX& mat) {
	for (auto obj : m_MapObjects) {
		obj->SetPintoWorld(mat);
	}
}
void CMapObjectManager::SetPintoPos(const D3DXVECTOR3& pos) {
	for (auto obj : m_MapObjects) {
		obj->SetPintoPos(pos);
	}
}

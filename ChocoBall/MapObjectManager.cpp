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

	//剛体を初期化。
	{
		//この引数に渡すのはボックスのhalfsizeなので、0.5倍する。
		SCollisionInfo* Table = collisionInfoTableArray[m_StageID];
		int arraySize = collisionInfoTableSizeArray[m_StageID];	//配列の要素数を返す。
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
			//ワールドに追加。
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(work2);
		}
	}

	// マップチップを配置。
	{
		MapObject* MapObjects = MapObjectsPointerArray[m_StageID];
		if (MapObjectsMaxArray[m_StageID] == 1) {
			if (!strcmp(MapObjects[0].FileName, "NotData")) {
				// 情報が出力されてないとき。
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
			// UnityとDirectXでは座標系が反転しているため、出力データを少し触ってから格納する。
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

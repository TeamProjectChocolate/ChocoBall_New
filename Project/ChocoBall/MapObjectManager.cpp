#include "stdafx.h"
#include "MapObjectManager.h"
#include "ObjectManager.h"
#include "CollisionType.h"

CMapObjectManager::CMapObjectManager() {
}

CMapObjectManager::~CMapObjectManager() {
	for (auto Obj : m_MapObjects) {
		SINSTANCE(CObjectManager)->DeleteGameObject(Obj);
		SAFE_DELETE(Obj);
	}
	m_MapObjects.clear();
	for (auto rigidBody : m_rigidBodys) {
		SAFE_DELETE(rigidBody);
	}
	m_rigidBodys.clear();
}

void CMapObjectManager::Initialize() {
	SetAlive(true);

	////剛体を初期化。
	//{
	//	//この引数に渡すのはボックスのhalfsizeなので、0.5倍する。
	//	SCollisionInfo* Table = collisionInfoTableArray[m_StageID];
	//	int arraySize = collisionInfoTableSizeArray[m_StageID];	//配列の要素数を返す。
	//	for (int i = 0; i < arraySize; i++) {
	//		SCollisionInfo& collision = Table[i];
	//		btBoxShape* work = new btBoxShape(btVector3(collision.scale.x*0.5f, collision.scale.y*0.5f, collision.scale.z*0.5f));
	//		m_groundShape.push_back(work);
	//		btTransform groundTransform;
	//		groundTransform.setIdentity();
	//		groundTransform.setOrigin(btVector3(-collision.pos.x, collision.pos.y, -collision.pos.z));
	//		groundTransform.setRotation(btQuaternion(-collision.rotation.x, collision.rotation.y, -collision.rotation.z, collision.rotation.w));
	//		float mass = 0.0f;

	//		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	//		m_myMotionState = new btDefaultMotionState(groundTransform);
	//		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, work, btVector3(0, 0, 0));
	//		btRigidBody* work2 = new btRigidBody(rbInfo);
	//		m_rigidBody.push_back(work2);
	//		work2->activate();
	//		m_rigidBody[i]->setUserIndex(CollisionType_Map);
	//		//ワールドに追加。
	//		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody_Dynamic(work2);
	//	}
	//}

	//// マップチップを配置。
	//{
	//	MapObject* MapObjects = MapObjectsPointerArray[m_StageID];
	//	if (MapObjectsMaxArray[m_StageID] == 1) {
	//		if (!strcmp(MapObjects[0].FileName, "NotData")) {
	//			// 情報が出力されてないとき。
	//			return;
	//		}
	//	}
	//	for (int idx = 0; idx < MapObjectsMaxArray[m_StageID];idx++) {
	//		CField* WorkObj = new CField;
	//		string name = "image/";
	//		name = name + static_cast<string>(MapObjects[idx].FileName);
	//		WorkObj->SetStageID(m_StageID);
	//		WorkObj->SetFileName(name.c_str());

	//		PRIORTY priorty;
	//		if (MapObjects[idx].Alpha >= 1.0f) {
	//			priorty = PRIORTY::OBJECT3D;
	//		}
	//		else {
	//			priorty = PRIORTY::OBJECT3D_ALPHA;
	//		}
	//		SINSTANCE(CObjectManager)->AddObject(WorkObj, _T("MapObject"), priorty, false);
	//		m_MapObjects.push_back(WorkObj);
	//	}
	//}
}

void CMapObjectManager::Build() {

	//剛体を初期化。
	{
		//この引数に渡すのはボックスのhalfsizeなので、0.5倍する。
		SCollisionInfo* Table = collisionInfoTableArray[m_StageID];
		int arraySize = collisionInfoTableSizeArray[m_StageID];	//配列の要素数を返す。
		for (int i = 0; i < arraySize; i++) {
			SCollisionInfo& collision = Table[i];

			SH_ENGINE::TRANSFORM tr;	// 剛体に渡すTransform情報。
			tr.Identity();
			// 位置情報設定。
			tr.position = collision.pos;
			// Unityの座標系からDirectXの座標系に変換。
			tr.position.x *= -1.0f;
			tr.position.z *= -1.0f;
			// 回転情報設定。
			tr.angle = collision.rotation;
			// Unityの座標系からDirectXの座標系に変換。
			tr.angle.x *= -1.0f;
			tr.angle.z *= -1.0f;
			// ※スケールは不要なため省く。

			CRigidbody* work = new CRigidbody;
			work->InitCollision(
				nullptr,
				tr,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				new btBoxShape(btVector3(collision.scale.x*0.5f, collision.scale.y*0.5f, collision.scale.z*0.5f)),
				CollisionType::Map,
				0.0f,
				true,
				true);
			work->BitMask_AllOff();
			work->BitMask_On(CollisionType::Player);
			work->BitMask_On(CollisionType::Chocoball);
			work->BitMask_On(CollisionType::Camera);
			work->BitMask_On(CollisionType::Enemy);

			m_rigidBodys.push_back(work);
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
		for (int idx = 0; idx < MapObjectsMaxArray[m_StageID]; idx++) {
			CField* WorkObj = new CField;
			string name = "image/";
			name = name + static_cast<string>(MapObjects[idx].FileName);
			WorkObj->SetStageID(m_StageID);
			WorkObj->SetFileName(name.c_str());

			OBJECT::PRIORTY priorty;
			if (MapObjects[idx].Alpha >= 1.0f) {
				priorty = OBJECT::PRIORTY::OBJECT3D;
			}
			else {
				priorty = OBJECT::PRIORTY::OBJECT3D_ALPHA;
			}
			WorkObj->Initialize();

			// UnityとDirectXでは座標系が反転しているため、出力データを少し触ってから格納する。
			if (m_StageID == STAGE_ID::BOSS) {
				D3DXVECTOR3 workPos = MapObjects[idx].Position;
				workPos.x *= -1;
				workPos.z *= -1;
				WorkObj->SetPos(workPos);
			}
			else {
				WorkObj->SetPos(MapObjects[idx].Position);
			}
			WorkObj->SetQuaternion(MapObjects[idx].Rotation);
			D3DXVECTOR3 scale = MapObjects[idx].Scale;
			WorkObj->SetScale(MapObjects[idx].Scale);
			if (static_cast<FRESNEL::REFRACTIVES>(MapObjects[idx].RType) == FRESNEL::REFRACTIVES::NONE) {
				WorkObj->SetIsFresnel(false);
			}
			else {
				WorkObj->SetIsFresnel(true);
				WorkObj->SetRefractive(static_cast<FRESNEL::REFRACTIVES>(MapObjects[idx].RType));
			}
			WorkObj->SetAlpha(MapObjects[idx].Alpha);

			SINSTANCE(CObjectManager)->AddObject(WorkObj, _T("MapObject"), priorty, false);
			m_MapObjects.push_back(WorkObj);
		}
	}

	//MapObject* MapObjects = MapObjectsPointerArray[m_StageID];
	//if (MapObjectsMaxArray[m_StageID] == 1) {
	//	if (!strcmp(MapObjects[0].FileName, "NotData")) {
	//		// 情報が出力されてないとき。
	//		return;
	//	}
	//}

	//for (int idx = 0; idx < MapObjectsMaxArray[m_StageID]; idx++) {
	//	// UnityとDirectXでは座標系が反転しているため、出力データを少し触ってから格納する。
	//	if (m_StageID == STAGE_ID::BOSS) {
	//		D3DXVECTOR3 workPos = MapObjects[idx].Position;
	//		workPos.x *= -1;
	//		workPos.z *= -1;
	//		m_MapObjects[idx]->SetPos(workPos);
	//	}
	//	else {
	//		m_MapObjects[idx]->SetPos(MapObjects[idx].Position);
	//	}
	//	m_MapObjects[idx]->SetQuaternion(MapObjects[idx].Rotation);
	//	D3DXVECTOR3 scale = MapObjects[idx].Scale;
	//	m_MapObjects[idx]->SetScale(MapObjects[idx].Scale);
	//	if (static_cast<REFRACTIVES>(MapObjects[idx].RType) == REFRACTIVES::NONE) {
	//		m_MapObjects[idx]->SetIsFresnel(false);
	//	}
	//	else {
	//		m_MapObjects[idx]->SetIsFresnel(true);
	//		m_MapObjects[idx]->SetRefractive(static_cast<REFRACTIVES>(MapObjects[idx].RType));
	//	}
	//	m_MapObjects[idx]->SetAlpha(MapObjects[idx].Alpha);
	//}
}

void CMapObjectManager::Update() {
	//for (auto obj : m_MapObjects) {
	//	obj->Update();
	//}
}

void CMapObjectManager::Draw() {
	if (m_czbuffersphere == NULL) {
		m_czbuffersphere = SINSTANCE(CObjectManager)->FindGameObject<CZBufferSphere>(_T("ZBufferSphere"));
	}
	for (auto obj : m_MapObjects) {
		obj->SetZBufferSphere(m_czbuffersphere);
	}
}

void CMapObjectManager::DrawShadow(CCamera* camera) {
	//for (auto obj : m_MapObjects) {
	//	obj->DrawShadow(camera);
	//}
}

void CMapObjectManager::Draw_EM(CCamera* camera) {
	//for (auto obj : m_MapObjects) {
	//	obj->Draw_EM(camera);
	//}
}

void CMapObjectManager::Is_DrawShadow_Use_Horizon() {
	//for (auto obj : m_MapObjects) {
	//	obj->Is_DrawShadow_Use_Horizon();
	//}
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

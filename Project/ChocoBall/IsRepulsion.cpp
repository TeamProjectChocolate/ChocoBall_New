#include "stdafx.h"
#include "IsRepulsion.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"

//// コールバックY(物理ワールド)
//// ※ワールドの何かに当たったかの判定に用いる。
//struct ContactResult_Y : public btCollisionWorld::ContactResultCallback
//{
//	ContactResult_Y()
//	{
//		isHit = false;
//	}
//
//	// 何らかのコリジョンと衝突したか。
//	bool isHit;
//	// 衝突した点。
//	D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, -FLT_MAX, 0.0f);
//	// 衝突した点の法線(Y成分)。
//	D3DXVECTOR3 hitNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	// 押し戻す向き。
//	D3DXVECTOR3 Direction;
//	// 食い込んでいる大きさ。
//	float Length = FLT_MAX;
//
//	D3DXVECTOR3 startPos;
//	// 当たったコリジョンのタイプを格納。
//	CollisionType hitCollisionType;
//
//	vector<bool>  m_MaskCollisionTypes;	// このクラスのインスタンスを持つオブジェクトが、そのCollisionTypeのあたりを無視するかのフラグを格納(trueなら無視)。
//	CGameObject* UserPointer = nullptr;		// isIntersectクラスのインスタンスを保持しているオブジェクトを格納せよ。
//	bool isFirstCallback = false;		// 一回の当たり判定で最初に呼ばれたコールバックか。
//
//	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
//	{
//		// 当たった相手の属性を引っ張ってくる。
//		int idx = colObj1Wrap->getCollisionObject()->getUserIndex();
//		if (m_MaskCollisionTypes[idx]) {
//			return 0.0f;
//		}
//
//		// 衝突点の法線を引っ張ってくる。
//		D3DXVECTOR3 hitPointNormal(cp.m_normalWorldOnB.x(), cp.m_normalWorldOnB.y(), cp.m_normalWorldOnB.z());
//		D3DXVec3Normalize(&hitPointNormal, &hitPointNormal);
//
//		// 上方向と法線のなす角度を算出。
//		float angle = D3DXVec3Dot(&hitPointNormal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
//		angle = fabsf(acosf(angle));
//		// 地面の傾斜が54度より小さい場合地面とみなす。
//		if (angle >= fPI * 0.3f) {
//			// 衝突していたものが地面ではない。
//			return 0.0f;
//		}
//		isHit = true;
//		//XZ平面での法線。
//		hitNormal.x = hitPointNormal.x;
//		hitNormal.y = 0.0f;
//		hitNormal.z = hitPointNormal.z;
//		D3DXVec3Normalize(&hitNormal, &hitNormal);
//
//		// 衝突点A。
//		D3DXVECTOR3 posA = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnA());
//		// 衝突点B。
//		D3DXVECTOR3 hitPosTmp = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnB());
//		// 衝突点の距離を求める。
//		Direction = hitPosTmp - posA;
//		// 押し戻す量。
//		float fDistTmp = D3DXVec3Length(&Direction);
//		if (Length > fDistTmp) {
//			// この衝突点のほうが近いので、最も近い衝突点を更新する。
//			hitPos = hitPosTmp;
//			hitNormal = static_cast<D3DXVECTOR3>(cp.m_normalWorldOnB);
//			Length = fDistTmp;
//		}
//		// 押し返す向き。
//		D3DXVec3Normalize(&Direction, &Direction);
//
//		hitCollisionType = static_cast<CollisionType>(colObj1Wrap->getCollisionObject()->getUserIndex());
//
//		if (isFirstCallback) {
//			// 一度の当たり判定で一度しかこの処理を行わない。
//			if (UserPointer) {
//				UserPointer->OnCollisionStay(const_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()));
//			}
//			if (colObj1Wrap->getCollisionObject()->getUserPointer()) {
//				static_cast<CGameObject*>(colObj1Wrap->getCollisionObject()->getUserPointer())->OnCollisionStay(UserPointer->GetCollisionObject());
//			}
//		}
//		return 0;
//	}
//};

// コールバック(物理ワールド)
// ※ワールドの何かに当たったかの判定に用いる。
struct ContactResult : public btCollisionWorld::ContactResultCallback
{
	// 衝突した点の法線()。
	D3DXVECTOR3 hitNormal;
	// 何かのコリジョンに当たったか。
	bool isHit = false;
	// 当たったコリジョンのタイプを格納。
	CollisionType hitCollisionType;
	// 衝突点。
	D3DXVECTOR3 hitPos;
	// 押し戻す向き。
	D3DXVECTOR3 Direction;
	// 食い込んでいる大きさ。
	float Length = 0.0f;
	ContactResult()
	{
		isHit = false;
	}

	vector<bool>  m_MaskCollisionTypes;	// このクラスのインスタンスを持つオブジェクトが、そのCollisionTypeのあたりを無視するかのフラグを格納(trueなら無視)。
	CGameObject* UserPointer = nullptr;		// isIntersectクラスのインスタンスを保持しているオブジェクトを格納せよ。
	bool isFirstCallback = false;		// 一回の当たり判定で最初に呼ばれたコールバックか。

	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		// 当たった相手の属性を引っ張ってくる。
		int idx = colObj1Wrap->getCollisionObject()->getUserIndex();
		if (m_MaskCollisionTypes[idx]) {
			return 0.0f;
		}

		// 衝突点の法線を引っ張ってくる。
		D3DXVECTOR3 hitPointNormal(cp.m_normalWorldOnB.x(), cp.m_normalWorldOnB.y(), cp.m_normalWorldOnB.z());
		D3DXVec3Normalize(&hitPointNormal, &hitPointNormal);

		//float d = D3DXVec3Dot(&hitPointNormal, &CVec3Up);
		//if (acosf(d) < fPI * 0.2) {
		//	//ホントは地面かどうかとかの属性を見るのがベストなんだけど、今回は角度で。
		//	return 0.0f;
		//}
		isHit = true;
		//XZ平面での法線。
		hitNormal.x = hitPointNormal.x;
		hitNormal.y = hitPointNormal.y;
		hitNormal.z = hitPointNormal.z;
		D3DXVec3Normalize(&hitNormal, &hitNormal);

		// 衝突点A。
		D3DXVECTOR3 posA = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnA());
		// 衝突点B。
		hitPos = static_cast<D3DXVECTOR3>(cp.getPositionWorldOnB());
		// 押し返す向き。
		Direction = hitPos - posA;
		Direction.y = 0.0f;
		// 押し戻す量。
		Length = D3DXVec3Length(&Direction);
		D3DXVec3Normalize(&Direction, &Direction);

		hitCollisionType = static_cast<CollisionType>(colObj1Wrap->getCollisionObject()->getUserIndex());

		if (isFirstCallback) {
			// 一度の当たり判定で一度しかこの処理を行わない。
			if (UserPointer) {
				UserPointer->OnCollisionStay(const_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()));
			}
			if (colObj1Wrap->getCollisionObject()->getUserPointer()) {
				static_cast<CGameObject*>(colObj1Wrap->getCollisionObject()->getUserPointer())->OnCollisionStay(UserPointer->GetCollisionObject());
			}
		}
		return 0.0f;
	}
};



CIsRepulsion::CIsRepulsion()
{
}


CIsRepulsion::~CIsRepulsion()
{
}

void CIsRepulsion::Initialize(btCollisionObject* collision) {
	m_pCollision = collision;
}

void CIsRepulsion::IsRepulsion(const D3DXVECTOR3& pos, D3DXVECTOR3& moveSpeed){
	//ContactResult callback;
	//callback.m_MaskCollisionTypes =
	//	callback.isFirstCallback = false;
	//callback.UserPointer = static_cast<CGameObject*>(m_pCollision->getUserPointer());
	//SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ContactTest_Dynamic(m_pCollision,callback);
	//if (callback.isHit) {

	//}
}

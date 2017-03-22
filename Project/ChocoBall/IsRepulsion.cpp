#include "stdafx.h"
#include "IsRepulsion.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"


CIsRepulsion::CIsRepulsion()
{
}


CIsRepulsion::~CIsRepulsion()
{
}

void CIsRepulsion::Initialize(CCollisionInterface* collision) {
	m_pCollision = collision;
}

void CIsRepulsion::Repulsion(D3DXVECTOR3* moveSpeed){
	ContactResult callback;
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ContactTest_Dynamic(m_pCollision->GetCollision(),callback);
	if (callback.isHit) {
		// 当たっている。
		if (callback.hitCollisionType != Collision::Type::AttackWall) {
			// 攻撃してくるチョコ壁じゃなければ終了。
			return;
		}
		// 衝突点。
		D3DXVECTOR3 hitPoint = callback.hitPoint;
		// 衝突点からこのクラスが持つコリジョンの中心へのベクトル。
		D3DXVECTOR3 Vec = m_pCollision->GetPos() - hitPoint;
		// 衝突点から中心点への距離。
		float Dist;
		Dist = D3DXVec3Length(&Vec);
		// コリジョンの大きさから距離を引いて最終的に押し戻す量を求める。
		Dist = m_pCollision->GetScale().x - Dist;

		// 食い込んだ逆方向に算出した量押し戻す。
		*moveSpeed += (Vec * Dist);
	}
}

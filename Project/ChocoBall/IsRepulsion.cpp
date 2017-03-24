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
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ContactTest_Dynamic(m_pCollision->GetCollisionObject(),callback);

	// 剛体が自発的に動いていない。
	// ほんの少しだけコリジョンサイズを大きくして疑似的に全方位にSweepTestをかける。
	D3DXVECTOR3 OriginScale = m_pCollision->GetScale();
	m_pCollision->SetScale(OriginScale * 2.0f);

	if (callback.isHit) {
		// コリジョンサイズを元に戻す。
		m_pCollision->SetScale(OriginScale);

		if (callback.hitCollisionType_A == Collision::Type::Map || callback.hitCollisionType_B == Collision::Type::Map) {
			return;
		}
		// 当たっている。
		if ((callback.hitCollisionType_A != Collision::Type::AttackWall) && (callback.hitCollisionType_B != Collision::Type::AttackWall)) {
			// 攻撃してくるチョコ壁じゃなければ終了。
			return;
		}

		OutputDebugString("動く壁に衝突。\n");

		// 衝突点。
		D3DXVECTOR3 hitPoint;

		// チョコ壁の情報を持ってくる。
		if (callback.hitCollisionType_A == Collision::Type::AttackWall) {
			hitPoint = callback.hitPoint_A;
		}
		else {
			hitPoint = callback.hitPoint_B;
		}

		// Y成分を打ち消す。
		D3DXVECTOR3 pos = m_pCollision->GetPos();
		pos.y = hitPoint.y;

		// 衝突点からこのクラスが持つコリジョンの中心へのベクトル。
		D3DXVECTOR3 Vec = pos - hitPoint;
		// 衝突点から中心点への距離。
		float Dist;
		Dist = D3DXVec3Length(&Vec);
		// コリジョンの大きさから距離を引いて最終的に押し戻す量を求める。
		Dist = m_pCollision->GetScale().x - Dist;

		// 食い込んだ逆方向に算出した量押し戻す。
		D3DXVec3Normalize(&Vec, &Vec);
		*moveSpeed += (Vec * Dist);
	}
	else {
		// コリジョンサイズを戻す。
		m_pCollision->SetScale(OriginScale);
	}

}

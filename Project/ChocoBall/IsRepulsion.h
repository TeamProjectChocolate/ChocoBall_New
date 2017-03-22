#pragma once
#include "BulletPhysics.h"
#include "CollisionType.h"

// 周囲の剛体による斥力をオブジェクトに反映するクラス。
class CIsRepulsion
{
public:
	CIsRepulsion();
	~CIsRepulsion();
	void Initialize(CCollisionInterface* collision);
	// 斥力を持つオブジェクトに斥力の合計影響値を返却する関数。
	// 引数：	オブジェクトの移動速度(関数内部で斥力を加算)。
	void Repulsion(D3DXVECTOR3* moveSpeed);
private:
	CCollisionInterface* m_pCollision = nullptr;
};


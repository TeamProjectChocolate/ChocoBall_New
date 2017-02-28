#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "StageTable.h"
#include "CollisionType.h"
#include "BulletPhysics.h"

typedef struct COURCE_BLOCK {
	D3DXVECTOR3 startPosition;
	D3DXVECTOR3 endPosition;
	int blockNo;
	COURCE_BLOCK* nextBlock;
};

namespace Boss_Cource {
	enum BOSS_COURCE_TYPE { Wait = 0, Move, Attack };
}

typedef struct BOSS_COURCE_BLOCK : public COURCE_BLOCK {
	D3DXVECTOR3 ActionTargetDir;		// ボスがステートに従ってアクションする際の向き。
	D3DXVECTOR3 ActionPos;		// ボスがステートに従ってアクションする際の位置。
	Boss_Cource::BOSS_COURCE_TYPE BlockType;	// コースの属性(この値に従ってボスのステートが変化する)。
	bool IsEnd;		// 最後のコースかどうか。
};


// コース定義。
class CCourceDef{
public:
	CCourceDef(){ m_courceNum = 0; };
	~CCourceDef(){};
	void Initialize();
	COURCE_BLOCK FindCource(D3DXVECTOR3);
	COURCE_BLOCK FindCource(int);
	D3DXVECTOR3 EndCource(){
		return m_courceEnd;
	}
	int GetCourceMax(){
		return m_courceNum;
	}
	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
private:
	COURCE_BLOCK m_cource;
	int m_courceNum;
	D3DXVECTOR3 m_courceEnd;
	STAGE_ID m_StageID;
};

struct SBOSS_BLOCK_COLLISION {
	btTransform Transform;
	btVector3 size;	// バウンディングボックスのサイズ。
	btVector3 OriginOffset;	// モデルの原点と剛体の原点のオフセット。
	unique_ptr<btBoxShape> pBoxCollision;	// 剛体の形状(今回は箱)。
	unique_ptr<btDefaultMotionState> myMotionState;
	unique_ptr<btCollisionObject> pCollision;
	bool IsHit;	// 剛体がすでに衝突しているか。
public:
	SBOSS_BLOCK_COLLISION() {
		IsHit = false;
	}
};

struct ContactResult : public btCollisionWorld::ContactResultCallback {
	// 何かのコリジョンに当たったか。
	bool isHit = false;
	// 当たったコリジョンのタイプを格納。
	CollisionType hitCollisionType;
	ContactResult()
	{
		isHit = false;
	}
	virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		if(colObj1Wrap->getCollisionObject()->getUserIndex())
		if (colObj1Wrap->getCollisionObject()->getUserIndex() == CollisionType_Player
			|| colObj1Wrap->getCollisionObject()->getUserIndex() == CollisionType_Chocoball
			|| colObj1Wrap->getCollisionObject()->getUserIndex() == CollisionType_Enemy)
		{
			//無視。
			return 0.0f;
		}
		else if (colObj1Wrap->getCollisionObject()->getUserIndex() == CollisionType_Camera) {
			//無視。
			return 0.0f;
		}
		else if (colObj1Wrap->getCollisionObject()->getUserIndex() == CollisionType_Bullet || colObj1Wrap->getCollisionObject()->getUserIndex() == CollisionType_Boss) {
			return 0.0f;
		}

		isHit = true;
		hitCollisionType = static_cast<CollisionType>(colObj1Wrap->getCollisionObject()->getUserIndex());
		return 0.0f;
	}
};


// ボス用のコース定義。
class CBossCourceDef {
public:
	CBossCourceDef() { m_courceNum = 0; }
	~CBossCourceDef();
	void Initialize();
	bool FindCource(btCollisionObject*,vector<BOSS_COURCE_BLOCK*>*);
	int GetCourceMax() {
		return m_courceNum;
	}
private:
	vector<BOSS_COURCE_BLOCK*> m_cource;
	vector<SBOSS_BLOCK_COLLISION*> m_Collisions;
	int m_courceNum;
};
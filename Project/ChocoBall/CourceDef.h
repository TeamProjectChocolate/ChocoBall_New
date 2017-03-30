#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "StageTable.h"
#include "CollisionType.h"
#include "BulletPhysics.h"
#include "CollisionObject.h"

namespace Cource{
	// コース構造体。
	typedef struct COURCE_BLOCK {
		D3DXVECTOR3 startPosition;
		D3DXVECTOR3 endPosition;
		int blockNo;
	};

	// ボス用コースの属性。
	namespace Boss_Cource {
		enum BOSS_COURCE_TYPE { Wait = 0, Move, Attack , Sleep};
	}

	// ボス用のコース構造体。
	typedef struct BOSS_COURCE_BLOCK : public COURCE_BLOCK {
		D3DXVECTOR3 ActionTargetDir;		// ボスがステートに従ってアクションする際の向き。
		D3DXVECTOR3 ActionPos;		// ボスがステートに従ってアクションする際の位置。
		Boss_Cource::BOSS_COURCE_TYPE BlockType;	// コースの属性(この値に従ってボスのステートが変化する)。
		bool IsEnd;		// 最後のコースかどうか。
	};

	// ボス用のコースのコリジョン構造体。
	struct SBOSS_BLOCK_COLLISION {
		CCollisionObject* CollisionObject = nullptr;
		bool IsHit;	// 剛体がすでに衝突しているか。
	public:
		SBOSS_BLOCK_COLLISION() {
			IsHit = false;
		}
	};
}

// コース定義。
class CCourceDef{
public:
	CCourceDef(){};
	~CCourceDef();
	void Initialize();
	Cource::COURCE_BLOCK FindCource(D3DXVECTOR3);
	Cource::COURCE_BLOCK FindCource(int);
	D3DXVECTOR3 EndCource(){
		return m_courceEnd;
	}
	int GetCourceMax(){
		return m_cource.size();
	}
	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
private:
	vector<Cource::COURCE_BLOCK*> m_cource;	// コース定義配列。
	D3DXVECTOR3 m_courceEnd;	// コースの終端。
	STAGE_ID m_StageID;
};

// ボス用のコース定義。
class CBossCourceDef {
public:
	CBossCourceDef() { m_courceNum = 0; }
	~CBossCourceDef();
	void Initialize();
	bool FindCource(const btCollisionObject*,vector<Cource::BOSS_COURCE_BLOCK*>*);
	int GetCourceMax() {
		return m_courceNum;
	}
private:
	vector<Cource::BOSS_COURCE_BLOCK*> m_cource;
	vector<Cource::SBOSS_BLOCK_COLLISION*> m_Collisions;
	int m_courceNum;
};
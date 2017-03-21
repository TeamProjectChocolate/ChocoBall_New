#include "stdafx.h"
#include "CourceDef.h"
#include "StageTable.h"
#include "islntersect.h"
#include "ObjectManager.h"


CCourceDef::~CCourceDef() {
	for (auto cource : m_cource) {
		SAFE_DELETE(cource);
	}
	m_cource.clear();
};

void CCourceDef::Initialize(){
	D3DXVECTOR3* CourceDef = CourceDefineArray[m_StageID];
	int arraySize =CourceDefineNumArray[m_StageID];
	for(int num = 0; num + 1 < arraySize; num++){
		Cource::COURCE_BLOCK* pCource = new Cource::COURCE_BLOCK;
		pCource->startPosition = CourceDef[num];
		pCource->endPosition = CourceDef[num + 1];
		pCource->startPosition.x *= -1.0f;
		pCource->startPosition.z *= -1.0f;
		pCource->endPosition.x *= -1.0f;
		pCource->endPosition.z *= -1.0f;
		pCource->blockNo = num;
		m_cource.push_back(pCource);
		if (num + 2 >= arraySize){
			m_courceEnd = pCource->endPosition;
		}
	}
}

Cource::COURCE_BLOCK CCourceDef::FindCource(D3DXVECTOR3 pos){
    float Min = FLT_MAX;
	Cource::COURCE_BLOCK* ret = nullptr;
	for (auto cource : m_cource){
		D3DXVECTOR3 endvector = cource->endPosition - cource->startPosition;
		D3DXVECTOR3 Playervector = pos - cource->startPosition;
		D3DXVECTOR3 Dir;
		float Distance = D3DXVec3Length(&endvector);
		D3DXVec3Normalize(&Dir, &endvector);
		float PlayerDist = D3DXVec3Dot(&Dir, &Playervector);
		if (PlayerDist <= Distance + 5.0f && PlayerDist > -5.0f){
			D3DXVECTOR3 VecToVecDist/*ベクトルとベクトルの距離*/ = (Dir * PlayerDist) - Playervector;
			if (D3DXVec3Length(&VecToVecDist) < Min){
				ret = cource;
				Min = D3DXVec3Length(&VecToVecDist);
			}
		}
	}
	if (ret == nullptr){
		Cource::COURCE_BLOCK ret2;
		ret2.blockNo = -1;
		return ret2;
	}
	return *ret;
}

Cource::COURCE_BLOCK CCourceDef::FindCource(int num){
	Cource::COURCE_BLOCK block;
	if (num < 0){
		block.blockNo = -1;
		return block;
	}
	else if (num >= m_cource.size()) {
		return block;
	}
	
	return *m_cource[num];
}


// ボス用のコース定義。

CBossCourceDef::~CBossCourceDef() {
	for (auto cource : m_cource) {
		SAFE_DELETE(cource);
	}
	m_cource.clear();
	for (auto collision : m_Collisions) {
		SAFE_DELETE(collision->CollisionObject);
		SAFE_DELETE(collision);
	}
	m_Collisions.clear();
}


void CBossCourceDef::Initialize() {
	m_courceNum = 0;
	BOSS_Cource_Block_ByUnity* CourceDef = BossCourceDefine_Boss;
	int arraySize = BossCourceDefineNumArray[0];
	for (int num = 0; num < arraySize; num++) {
		// Unityから受け取った値をDirectXの座標系に変換して格納。
		Cource::BOSS_COURCE_BLOCK* work = new Cource::BOSS_COURCE_BLOCK;
		// コースの向きベクトルを求めるための値(向きベクトルの開始点)。
		work->startPosition = CourceDef[num].startPosition;
		work->startPosition.x *= -1.0f;
		work->startPosition.z *= -1.0f;
		// コースの向きベクトルを求めるための値(向きベクトルの終了点)。
		work->endPosition = CourceDef[num].endPosition;
		work->endPosition.x *= -1.0f;
		work->endPosition.z *= -1.0f;
		// ボスがアクションを起こす位置。
		work->ActionPos = CourceDef[num].ActionPos;
		work->ActionPos.x *= -1.0f;
		work->ActionPos.z *= -1.0f;
		// ボスがアクションを起こす向き。
		work->ActionTargetDir = CourceDef[num].ActionTargetDir;
		work->ActionTargetDir.x *= -1.0f;
		work->ActionTargetDir.z *= -1.0f;
		// ボスのステートを決定する値。
		work->BlockType = static_cast<Cource::Boss_Cource::BOSS_COURCE_TYPE>(CourceDef[num].BlockType);
		// 最後のコース定義か。
		work->IsEnd = CourceDef[num].IsEnd;
		// コースナンバー。
		work->blockNo = num;
		// コース定義配列に追加。
		m_cource.push_back(work);

		// コリジョンを生成。
		{
			Cource::SBOSS_BLOCK_COLLISION* pBossBlockCollision = new Cource::SBOSS_BLOCK_COLLISION;
			pBossBlockCollision->CollisionObject = new CCollisionObject;

			SH_ENGINE::TRANSFORM tr;	// コリジョンに渡すTransform情報。
			tr.Identity();
			// 位置情報設定。
			tr.position = CourceDef[num].ActionPos;
			// Unityの座標系からDirectXの座標系に変換。
			tr.position.x *= -1.0f;
			tr.position.z *= -1.0f;
			// 回転情報設定。
			tr.angle = CourceDef[num].Rotation;
			// Unityの座標系からDirectXの座標系に変換。
			tr.angle.x *= -1.0f;
			tr.angle.z *= -1.0f;
			// ※スケールは不要なため省く。

			// ボックスコリジョンを定義(中心から見たサイズなので半分)。
			btVector3 size = btVector3(CourceDef[num].Range.x / 2, CourceDef[num].Range.y / 2, CourceDef[num].Range.z / 2);
			// 上記で決定した値でボックスコリジョン作成。
			pBossBlockCollision->CollisionObject->InitCollision(nullptr, tr, D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(size),CollisionType::Boss_Cource,0.0f,true,true);
			// コース判定用のボスのコリジョンとのみ衝突判定を行う。
			pBossBlockCollision->CollisionObject->BitMask_AllOn();
			pBossBlockCollision->CollisionObject->BitMask_Off(CollisionType::Boss_Gost);
			// フラグ初期化。
			pBossBlockCollision->IsHit = false;
			// コースコリジョン配列に追加。
			m_Collisions.push_back(pBossBlockCollision);
		}
		m_courceNum++;
	}
}

bool CBossCourceDef::FindCource(btCollisionObject* CollisionObject, vector<Cource::BOSS_COURCE_BLOCK*>* CourceBlocks) {
	CourceBlocks->clear();
	int size = m_Collisions.size();
	bool IsNowCourceChange = false;	// 接触しているコースが前回から変化したか。
	for (int idx = 0; idx < size; idx++) {
		ContactResult CallBack;
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->GetCollisionWorld()->contactPairTest(CollisionObject, m_Collisions[idx]->CollisionObject->GetCollision(), CallBack);
		if (CallBack.isHit) {
			if (!(m_Collisions[idx]->IsHit)) {
				// 前のフレームの時点でこの剛体が衝突していなかった。
				m_Collisions[idx]->IsHit = true;
				IsNowCourceChange = true;
			}
			CourceBlocks->push_back(m_cource[idx]);
		}
		else {
			if (m_Collisions[idx]->IsHit) {
				// 前のフレームの時点でこの剛体が衝突していた。
				m_Collisions[idx]->IsHit = false;
				IsNowCourceChange = true;
			}
		}
	}
	return IsNowCourceChange;
}
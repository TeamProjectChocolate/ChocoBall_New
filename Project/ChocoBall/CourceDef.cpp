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
			D3DXVECTOR3 VecToVecDist/*�x�N�g���ƃx�N�g���̋���*/ = (Dir * PlayerDist) - Playervector;
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


// �{�X�p�̃R�[�X��`�B

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
		// Unity����󂯎�����l��DirectX�̍��W�n�ɕϊ����Ċi�[�B
		Cource::BOSS_COURCE_BLOCK* work = new Cource::BOSS_COURCE_BLOCK;
		// �R�[�X�̌����x�N�g�������߂邽�߂̒l(�����x�N�g���̊J�n�_)�B
		work->startPosition = CourceDef[num].startPosition;
		work->startPosition.x *= -1.0f;
		work->startPosition.z *= -1.0f;
		// �R�[�X�̌����x�N�g�������߂邽�߂̒l(�����x�N�g���̏I���_)�B
		work->endPosition = CourceDef[num].endPosition;
		work->endPosition.x *= -1.0f;
		work->endPosition.z *= -1.0f;
		// �{�X���A�N�V�������N�����ʒu�B
		work->ActionPos = CourceDef[num].ActionPos;
		work->ActionPos.x *= -1.0f;
		work->ActionPos.z *= -1.0f;
		// �{�X���A�N�V�������N���������B
		work->ActionTargetDir = CourceDef[num].ActionTargetDir;
		work->ActionTargetDir.x *= -1.0f;
		work->ActionTargetDir.z *= -1.0f;
		// �{�X�̃X�e�[�g�����肷��l�B
		work->BlockType = static_cast<Cource::Boss_Cource::BOSS_COURCE_TYPE>(CourceDef[num].BlockType);
		// �Ō�̃R�[�X��`���B
		work->IsEnd = CourceDef[num].IsEnd;
		// �R�[�X�i���o�[�B
		work->blockNo = num;
		// �R�[�X��`�z��ɒǉ��B
		m_cource.push_back(work);

		// �R���W�����𐶐��B
		{
			Cource::SBOSS_BLOCK_COLLISION* pBossBlockCollision = new Cource::SBOSS_BLOCK_COLLISION;
			pBossBlockCollision->CollisionObject = new CCollisionObject;

			SH_ENGINE::TRANSFORM tr;	// �R���W�����ɓn��Transform���B
			tr.Identity();
			// �ʒu���ݒ�B
			tr.position = CourceDef[num].ActionPos;
			// Unity�̍��W�n����DirectX�̍��W�n�ɕϊ��B
			tr.position.x *= -1.0f;
			tr.position.z *= -1.0f;
			// ��]���ݒ�B
			tr.angle = CourceDef[num].Rotation;
			// Unity�̍��W�n����DirectX�̍��W�n�ɕϊ��B
			tr.angle.x *= -1.0f;
			tr.angle.z *= -1.0f;
			// ���X�P�[���͕s�v�Ȃ��ߏȂ��B

			// �{�b�N�X�R���W�������`(���S���猩���T�C�Y�Ȃ̂Ŕ���)�B
			btVector3 size = btVector3(CourceDef[num].Range.x / 2, CourceDef[num].Range.y / 2, CourceDef[num].Range.z / 2);
			// ��L�Ō��肵���l�Ń{�b�N�X�R���W�����쐬�B
			pBossBlockCollision->CollisionObject->InitCollision(nullptr, tr, D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(size),CollisionType::Boss_Cource,0.0f,true,true);
			// �R�[�X����p�̃{�X�̃R���W�����Ƃ̂ݏՓ˔�����s���B
			pBossBlockCollision->CollisionObject->BitMask_AllOn();
			pBossBlockCollision->CollisionObject->BitMask_Off(CollisionType::Boss_Gost);
			// �t���O�������B
			pBossBlockCollision->IsHit = false;
			// �R�[�X�R���W�����z��ɒǉ��B
			m_Collisions.push_back(pBossBlockCollision);
		}
		m_courceNum++;
	}
}

bool CBossCourceDef::FindCource(btCollisionObject* CollisionObject, vector<Cource::BOSS_COURCE_BLOCK*>* CourceBlocks) {
	CourceBlocks->clear();
	int size = m_Collisions.size();
	bool IsNowCourceChange = false;	// �ڐG���Ă���R�[�X���O�񂩂�ω��������B
	for (int idx = 0; idx < size; idx++) {
		ContactResult CallBack;
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->GetCollisionWorld()->contactPairTest(CollisionObject, m_Collisions[idx]->CollisionObject->GetCollision(), CallBack);
		if (CallBack.isHit) {
			if (!(m_Collisions[idx]->IsHit)) {
				// �O�̃t���[���̎��_�ł��̍��̂��Փ˂��Ă��Ȃ������B
				m_Collisions[idx]->IsHit = true;
				IsNowCourceChange = true;
			}
			CourceBlocks->push_back(m_cource[idx]);
		}
		else {
			if (m_Collisions[idx]->IsHit) {
				// �O�̃t���[���̎��_�ł��̍��̂��Փ˂��Ă����B
				m_Collisions[idx]->IsHit = false;
				IsNowCourceChange = true;
			}
		}
	}
	return IsNowCourceChange;
}
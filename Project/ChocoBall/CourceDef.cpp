#include "stdafx.h"
#include "CourceDef.h"
#include "StageTable.h"
#include "islntersect.h"
#include "ObjectManager.h"


void CCourceDef::Initialize(){
	m_courceNum = 0;
	D3DXVECTOR3* CourceDef = CourceDefineArray[m_StageID];
	int arraySize =CourceDefineNumArray[m_StageID];
	COURCE_BLOCK* itr = &m_cource;
	for(int num = 0; num + 1 < arraySize; num++){
		itr->startPosition = CourceDef[num];
		itr->endPosition = CourceDef[num + 1];
		itr->startPosition.x *= -1.0f;
		itr->startPosition.z *= -1.0f;
		itr->endPosition.x *= -1.0f;
		itr->endPosition.z *= -1.0f;
		itr->blockNo = num;
		COURCE_BLOCK* node = new COURCE_BLOCK;
		itr->nextBlock = node;
		m_courceNum++;
		if (num + 2 >= arraySize){
			m_courceEnd = itr->endPosition;
			itr->nextBlock = nullptr;
		}
		else{
			itr = node;
		}
	}
}

COURCE_BLOCK CCourceDef::FindCource(D3DXVECTOR3 pos){
    float Min = FLT_MAX;
	COURCE_BLOCK* ret = nullptr;
	COURCE_BLOCK* itr = &m_cource;
	for (int num = 0; num < m_courceNum; num++){
		D3DXVECTOR3 endvector = itr->endPosition - itr->startPosition;
		D3DXVECTOR3 Playervector = pos - itr->startPosition;
		D3DXVECTOR3 Dir;
		float Distance = D3DXVec3Length(&endvector);
		D3DXVec3Normalize(&Dir, &endvector);
		float PlayerDist = D3DXVec3Dot(&Dir, &Playervector);
		if (PlayerDist <= Distance + 5.0f && PlayerDist > -5.0f){
			D3DXVECTOR3 VecToVecDist/*�x�N�g���ƃx�N�g���̋���*/ = (Dir * PlayerDist) - Playervector;
			if (D3DXVec3Length(&VecToVecDist) < Min){
				ret = itr;
				Min = D3DXVec3Length(&VecToVecDist);
			}
		}
		itr = itr->nextBlock;
	}
	if (ret == nullptr){
		COURCE_BLOCK ret2;
		ret2.blockNo = -1;
		//abort();
		return ret2;
	}
	return *ret;
}

COURCE_BLOCK CCourceDef::FindCource(int num){
	if (num < 0){
		COURCE_BLOCK block;
		block.blockNo = -1;
		return block;
	}
	COURCE_BLOCK* itr = &m_cource;
	for (int idx = 0; idx < m_courceNum; idx++){
		if (itr->blockNo == num){
			return *itr;
		}
		itr = itr->nextBlock;
	}
	return *itr;
}


// �{�X�p�̃R�[�X��`�B

CBossCourceDef::~CBossCourceDef() {
	for (auto cource : m_cource) {
		SAFE_DELETE(cource);
	}
	m_cource.clear();
	for (auto rigidBody : m_Collisions) {
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveCollisionObject(rigidBody->pCollision.get());
		SAFE_DELETE(rigidBody);
	}
	m_Collisions.clear();
}


void CBossCourceDef::Initialize() {
	m_courceNum = 0;
	BOSS_Cource_Block_ByUnity* CourceDef = BossCourceDefine_Boss;
	int arraySize = BossCourceDefineNumArray[0];
	for (int num = 0; num < arraySize; num++) {
		// Unity����󂯎�����l��DirectX�̍��W�n�ɕϊ����Ċi�[�B
		BOSS_COURCE_BLOCK* work = new BOSS_COURCE_BLOCK;
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
		work->BlockType = static_cast<Boss_Cource::BOSS_COURCE_TYPE>(CourceDef[num].BlockType);
		// �Ō�̃R�[�X��`���B
		work->IsEnd = CourceDef[num].IsEnd;
		// �R�[�X�i���o�[�B
		work->blockNo = num;
		// �R�[�X��`�z��ɒǉ��B
		m_cource.push_back(work);

		// ���̂𐶐��B
		{
			SBOSS_BLOCK_COLLISION* pBossBlockRigidBody = new SBOSS_BLOCK_COLLISION;
			// �{�b�N�X�R���W�������`(���S���猩���T�C�Y�Ȃ̂Ŕ���)�B
			pBossBlockRigidBody->size = btVector3(CourceDef[num].Range.x / 2, CourceDef[num].Range.y / 2, CourceDef[num].Range.z / 2);
			// ��L�Ō��肵���l�Ń{�b�N�X�R���W�����쐬�B
			pBossBlockRigidBody->pBoxCollision.reset(new btBoxShape(pBossBlockRigidBody->size));
			// ���̂�Transform����ݒ�B
			btTransform rbTransform;
			rbTransform.setIdentity();
			rbTransform.setOrigin(btVector3(-CourceDef[num].RangePos.x, CourceDef[num].RangePos.y, -CourceDef[num].RangePos.z));
			rbTransform.setRotation(btQuaternion(-CourceDef[num].Rotation.x, CourceDef[num].Rotation.y, -CourceDef[num].Rotation.z, CourceDef[num].Rotation.w));
			// �R���W���������B
			pBossBlockRigidBody->pCollision.reset(new btCollisionObject);
			pBossBlockRigidBody->pCollision->setWorldTransform(rbTransform);
			pBossBlockRigidBody->pCollision->setCollisionShape(pBossBlockRigidBody->pBoxCollision.get());
			// ���[���h�ɒǉ��B
			SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddCollisionObject(pBossBlockRigidBody->pCollision.get());
			pBossBlockRigidBody->IsHit = false;
			// ���̔z��ɒǉ��B
			m_Collisions.push_back(pBossBlockRigidBody);
		}
		m_courceNum++;
	}
}

bool CBossCourceDef::FindCource(btCollisionObject* CollisionObject, vector<BOSS_COURCE_BLOCK*>* CourceBlocks) {
	CourceBlocks->clear();
	int size = m_Collisions.size();
	bool IsNowCourceChange = false;	// �ڐG���Ă���R�[�X���O�񂩂�ω��������B
	for (int idx = 0; idx < size; idx++) {
		ContactResult CallBack;
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->GetCollisionWorld()->contactPairTest(CollisionObject, m_Collisions[idx]->pCollision.get(), CallBack);
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


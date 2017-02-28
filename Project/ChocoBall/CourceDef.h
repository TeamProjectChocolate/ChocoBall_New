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
	D3DXVECTOR3 ActionTargetDir;		// �{�X���X�e�[�g�ɏ]���ăA�N�V��������ۂ̌����B
	D3DXVECTOR3 ActionPos;		// �{�X���X�e�[�g�ɏ]���ăA�N�V��������ۂ̈ʒu�B
	Boss_Cource::BOSS_COURCE_TYPE BlockType;	// �R�[�X�̑���(���̒l�ɏ]���ă{�X�̃X�e�[�g���ω�����)�B
	bool IsEnd;		// �Ō�̃R�[�X���ǂ����B
};


// �R�[�X��`�B
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
	btVector3 size;	// �o�E���f�B���O�{�b�N�X�̃T�C�Y�B
	btVector3 OriginOffset;	// ���f���̌��_�ƍ��̂̌��_�̃I�t�Z�b�g�B
	unique_ptr<btBoxShape> pBoxCollision;	// ���̂̌`��(����͔�)�B
	unique_ptr<btDefaultMotionState> myMotionState;
	unique_ptr<btCollisionObject> pCollision;
	bool IsHit;	// ���̂����łɏՓ˂��Ă��邩�B
public:
	SBOSS_BLOCK_COLLISION() {
		IsHit = false;
	}
};

struct ContactResult : public btCollisionWorld::ContactResultCallback {
	// �����̃R���W�����ɓ����������B
	bool isHit = false;
	// ���������R���W�����̃^�C�v���i�[�B
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
			//�����B
			return 0.0f;
		}
		else if (colObj1Wrap->getCollisionObject()->getUserIndex() == CollisionType_Camera) {
			//�����B
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


// �{�X�p�̃R�[�X��`�B
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
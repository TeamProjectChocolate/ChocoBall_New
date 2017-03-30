#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "StageTable.h"
#include "CollisionType.h"
#include "BulletPhysics.h"
#include "CollisionObject.h"

namespace Cource{
	// �R�[�X�\���́B
	typedef struct COURCE_BLOCK {
		D3DXVECTOR3 startPosition;
		D3DXVECTOR3 endPosition;
		int blockNo;
	};

	// �{�X�p�R�[�X�̑����B
	namespace Boss_Cource {
		enum BOSS_COURCE_TYPE { Wait = 0, Move, Attack , Sleep};
	}

	// �{�X�p�̃R�[�X�\���́B
	typedef struct BOSS_COURCE_BLOCK : public COURCE_BLOCK {
		D3DXVECTOR3 ActionTargetDir;		// �{�X���X�e�[�g�ɏ]���ăA�N�V��������ۂ̌����B
		D3DXVECTOR3 ActionPos;		// �{�X���X�e�[�g�ɏ]���ăA�N�V��������ۂ̈ʒu�B
		Boss_Cource::BOSS_COURCE_TYPE BlockType;	// �R�[�X�̑���(���̒l�ɏ]���ă{�X�̃X�e�[�g���ω�����)�B
		bool IsEnd;		// �Ō�̃R�[�X���ǂ����B
	};

	// �{�X�p�̃R�[�X�̃R���W�����\���́B
	struct SBOSS_BLOCK_COLLISION {
		CCollisionObject* CollisionObject = nullptr;
		bool IsHit;	// ���̂����łɏՓ˂��Ă��邩�B
	public:
		SBOSS_BLOCK_COLLISION() {
			IsHit = false;
		}
	};
}

// �R�[�X��`�B
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
	vector<Cource::COURCE_BLOCK*> m_cource;	// �R�[�X��`�z��B
	D3DXVECTOR3 m_courceEnd;	// �R�[�X�̏I�[�B
	STAGE_ID m_StageID;
};

// �{�X�p�̃R�[�X��`�B
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
#pragma once
#include "stdafx.h"
#include "EnemyBase.h"
#include "CourceDef.h"
#include "DivisionWall.h"
#include "Barrier.h"
#include "Audio.h"

class CState;

class CHadBar;

class CChocoBall;
class CCBManager;

#define BossDmage 10

class CEnemy_Boss :
	public CGameObject
{
public:
	enum BOSS_ANIMATION { Wait = 0, FastsSwim, Swim, Up, Down, Dive };
	enum BOSS_STATE { Sleep = 0,BWait, BMove, Attack, Escape, ShotAttack, RushAttack, Shot, Trans, Rotate, NonAction};
	enum SHOT_STATE { Ready = 0, Action, Post };
public:
	CEnemy_Boss();
	~CEnemy_Boss();
	void InitState();
	void SetInitPosition(const D3DXVECTOR3&);
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void OnDestroy()override;
	// �`���R�{�[���ɓ��������ۂɌĂ΂��R�[���o�b�N�B
	// �����͓��������`���R�{�[���̃}�l�[�W���[�B
	void ChocoHit(CCBManager*);
	void ConfigLight()override;

	void ChangeState(BOSS_STATE);

	void SetUpTechnique()override {
		m_pRender->SetUpTechnique("Skin_Tex");
	}
	void EM_SetUpTechnique()override {
		m_pEMSamplingRender->SetUpTechnique("Skin_Tex_Lim");
	}
	void SetAnimationState(BOSS_ANIMATION anim) {
		m_AnimState = anim;
	}
	void SetPlayingState(PLAYING_STATE ps) {
		m_PlayingState = ps;
	}
	CAnimation* GetAnimation() {
		return m_pModel->GetAnimation();
	}
	// �w�肳�ꂽ�X�e�[�g�̓Y�������Q�Ƃ��ă|�C���^��ԋp�B
	CState* GetState(BOSS_STATE stateNum) {
		if (stateNum >= m_States.size()) {
			// �w�肳�ꂽ�Y�������X�e�[�g�z��ɓo�^����Ă��鐔���傫���B
			abort();
		}
		return m_States[stateNum].get();
	}

	const vector<BOSS_COURCE_BLOCK*>& GetNowCource() {
		return m_NowCource;
	}
	bool GetIsNowCourceChange() {
		return m_IsNowCourceChange;
	}
	void SetIsUpdateCource(bool flg) {
		m_IsUpdateCource = flg;
	}
	void SetDivisionWall(vector<CDivisionWall*>* walls) {
		m_Walls = walls;
	}

	const D3DXVECTOR3& GetSize() {
		return m_size;
	}

	CBarrier* GetBarrier() {
		return m_pBarrier;
	}

	void SetAudio(CAudio* pAudio) {
		m_pAudio = pAudio;
	}
	CAudio* GetAudio() {
		return m_pAudio;
	}

	void DivisionWallOpen();
private:
	short m_HP;	// �q�b�g�|�C���g�B
	CHadBar* m_pHPBar;	// HP�o�[�B

	BOSS_STATE m_State;
	PLAYING_STATE m_PlayingState;
	BOSS_ANIMATION m_AnimState;

	D3DXVECTOR3 m_size;

	btVector3 m_OriginOffset;	// ���f���̌��_�ƍ��̂̌��_�̃I�t�Z�b�g�B
	unique_ptr<btCapsuleShapeZ> m_pCapsuleCollision;	// ���̂̌`��(����̓J�v�Z��)�B
	unique_ptr<btDefaultMotionState> m_myMotionState;
	unique_ptr<btRigidBody> m_pRigidBody;
	// �R�[�X��`����p�̃R���W�����B
	btVector3 m_CollisionSize;
	unique_ptr<btBoxShape> m_pBoxShape_Cource;	// �R���W�����̌`��(����͔�)�B
	unique_ptr<btCollisionObject> m_pCollision;	// �R���W�����B

	// �R�[�X��`�B
	CBossCourceDef m_CourceDef;
	vector<BOSS_COURCE_BLOCK*> m_NowCource;
	bool m_IsNowCourceChange = false;	// �ڐG���Ă���R�[�X���O�񂩂�ω��������B
	bool m_IsUpdateCource = true;	// �R�[�X�Ƃ̓����蔻����s�����B

	int m_DamageCounter = 0;
	CState* m_pCurrentState = nullptr;
	vector<unique_ptr<CState>> m_States;	// �{�X�̃X�e�[�g���ׂĂ��i�[�B

	vector<CDivisionWall*>* m_Walls;
	int m_WallsIdx = 0;	// Walls�̓Y�����B

	// �{�X�����o���A(�{�X���U�����̂݉�������A�`���R�{�[����������)�B
	CBarrier* m_pBarrier = nullptr;

	CAudio* m_pAudio = nullptr;
};

#define Max_AnimNum 6

// �N�W���G�l�~�[�B
static double BossAnimationTime[] = {
	198.0 / 30.0,//�ҋ@�B
	79.0 / 30.0,//�����j���B
	//84.0 / 30.0,//�����j���B
	180.0 / 30.0,//�x���j���B
	//214.0 / 30.0,//�x���j���B
	698.0 / 30.0,//�㏸�B
	105.0 / 30.0,//���~�B
	382.0 / 30.0,//���s�B
};

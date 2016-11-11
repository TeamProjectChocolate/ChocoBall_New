#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "DirectInput.h"
#include "ActreLight.h"
#include "islntersect.h"
#include "Turn.h"
#include "GameManager.h"
#include "CourceDef.h"
#include "LockOn.h"
#include "PlayerBullet.h"
#include "CBManager.h"
#include "Vibration.h"
#include "CourceCamera.h"
#include "Audio.h"

class CLockOn;
class CCBManager;
class CParticleEmitter;
class MoveFloor;

#define PLAYER_JUMP_POWER /*20.0f*/13.5f	//�v���C���[�̃W�����v�̗́B

class CPlayer : public CGameObject
{
public:
	CPlayer(){
		m_pEmitter = nullptr;
		m_pCamera = nullptr;
		parent = nullptr;
	};
	~CPlayer();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SetUpTechnique()override{
		m_pRender->SetUpTechnique("Skin_Tex_Lim");
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("Skin_Tex_Lim");
	}

	// ���C�g�ݒ�B
	void ConfigLight()override;

	void EnemyBulletHit(D3DXVECTOR3 moveDir);

	void Setradius(float radius)
	{
		m_radius = radius;
	}
	bool GetShotflag()
	{
		return m_ShotFlg;
	}
	GAMEEND_ID GetGameState()
	{
		return m_GameState;
	}
	bool GetChocoBall()
	{
		return ChocoBall;
	}
	void SetCBM(CCBManager* CHOCO)
	{
		m_CBManager.push_back(CHOCO);
	}
	int GetBusterEnemyNum()
	{
		return BusterEnemyNum;
	}

	//�e�����߂���
	void SetParent(MoveFloor* parent);

	MoveFloor* GetParent(){
		return parent;
	}
	

	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
	CCourceDef* GetCourceDef(){
		return &m_Courcedef;
	}

	bool GetGameover()
	{
		return GamaOverFlag;
	}
	void RequestGameOver()
	{
		m_GameState = GAMEEND_ID::OVER;
	}
	bool IsVibration() ;
	
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
	//bool GetIsJump() {
	//	return m_IsJump;
	//}
private:
	enum ANIMATION_STATE{ WAIT = 0, WALK,JUMP_START,JUMP_NOW,JUMP_END, DAMAGE};
	enum JUMP_STATE{J_NULL,J_EINS,J_ZWEI,J_DREI};
private:
	// �p�[�e�B�N���֘A�B
	CParticleEmitter* m_pEmitter;
	CParticleEmitter* m_pEmitter2;
	float m_JumpParticleTime;						//�W�����v�����n���̃p�[�e�B�N���������������鎞��
	float m_JumpParticleTimer;						//�W�����v�����n���̃p�[�e�B�N�����𔭐������鎞�Ԃ̃J�E���^�[
	//float m_GunParticleTime;						//�e���ˎ��̃p�[�e�B�N���������������鎞��
	//float m_GunParticleTimer;						//�e���ˎ��̃p�[�e�B�N�����𔭐������鎞�Ԃ̃J�E���^�[
	bool m_PreviousJumpFlag;			//�p�[�e�B�N�����W�����v�̒��n���ɔ��������邽�߂̃t���O
	// ���C�g�֘A�B
	D3DXVECTOR3		m_lightDir[NUM_DIFFUSE_LIGHT];
	D3DXVECTOR4		m_lightColor[NUM_DIFFUSE_LIGHT];	
	// �v���C���[����֘A�B
	MOVE_STATE m_State;
	ANIMATION_STATE m_AnimState;
	bool			m_ShotFlg;			//�e�����˂���Ă���̂��̃t���O
	// �v���C���[�A�N�V��������֘A�B
	float m_NowJumpPower;	// �W�����v�́B
	float			_X;
	float			m_radius;
	D3DXVECTOR3		m_initPosition;		//�������W�B
	D3DXVECTOR3		m_moveSpeed;		//�ړ����x�B
	float			m_currentAngleY;	//���݂̕���
	float			m_targetAngleY;		//������������
	float			m_turnSpeed;		//��]���x
	bool			isTurn;				//��]�t���O
	CTurn			m_Turn;				//CTurn�̃C���X�^���X
	CCourceDef		m_Courcedef;
	int m_NowCourceNo;
	D3DXVECTOR3 RV0;
	D3DXMATRIX Rot;
	D3DXVECTOR4 RV1;
	CVibration m_vibration;
	int BulletShotInterval;			//�e�𔭎˂���Ԋu�̎���
	JUMP_STATE m_JumpState;
	float m_JumpEndInterval;
	float m_JumpEndCounter;
	// �����蔻��֘A�B
	EnemyBase* m_NearEnemy = nullptr;
	bool            LockOnflag;			//���b�N�I���t���O
	EnemyBase* m_LockOnEnemy = nullptr;
	CLockOn         m_LockOn;			//LockOn�̃C���X�^���X
	CIsIntersect	m_IsIntersect;		//CIsIntersect�̃C���X�^���X
	float m_VibrationInterval;			// �U�����Ă��玟�̐U������܂ł̗P�\�B
	float m_VibrationCounter;			// �J�E���^�[�B
	// �Q�[���I�[�o�[����֘A�B
	GAMEEND_ID		m_GameState = GAMEEND_ID::CONTINUE;
	bool            GamaOverFlag;		//�Q�[���I�[�o�[
	float			deadTimer;			//�Q�[���I�[�o�[�܂ł̑ҋ@����
	// �M�~�b�N�֘A�B
	bool			m_HitFlag;
	MoveFloor*		parent;
	D3DXVECTOR3		localPosition;
	bool            ChocoBall;			//�`���R�{�[���𗬂��t���O
	int             BusterEnemyNum;		//�|�����G�̐�
	list<CCBManager*> m_CBManager;
	list<CCBManager*> m_DeleteChocoBall;
	vector<CPlayerBullet*> m_bullets;
	vector<CPlayerBullet*> m_Deletebullets;	// �폜���X�g
	// ���̑��B
	bool m_ActiveKeyState;	// �L�[������󂯕t���邩�B
	CInterface*	m_pInput;
	D3DXVECTOR3		m_size;	//�v���C���[������o�E���f�B���O�{�b�N�X�̃T�C�Y�B
	STAGE_ID m_StageID;
	CCourceCamera* m_pCamera;
	CAudio* m_pAudio;//SE�g�p
private:
	// �Q�[���X�e�[�^�X�Ǘ��B
	void StateManaged();
	// �v���C���[����֘A�B
	void MoveStateManaged();
	void Move();
	void UpDownMove();
	void LockOn();
	void BehaviorCorrection();
	void BulletShot();
	void ChocoHit();
	void RollingPlayer();
	// �����蔻��֘A�B
	void Collisions();
	// �L�[����B
	void KeyState();
	// �폜�֘A�B
	void DeleteChocoBall(CCBManager*);
	void ExcuteDeleteChocoBall();
	void DeleteBullet(CPlayerBullet*);
	void ExcuteDeleteBullets();
};

extern CPlayer* g_player;

namespace tkEngine{
	const D3DXVECTOR3 vec3Zero = { 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 vec3Right = { 1.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 vec3Left = { -1.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 vec3Up = { 0.0f, 1.0f, 0.0f };
	const D3DXVECTOR3 vec3Down = { 0.0f, -1.0f, 0.0f };
	const D3DXVECTOR3 vec3Front = { 0.0f, 0.0f, 1.0f };
	const D3DXVECTOR3 vec3Back = { 0.0f, 0.0f, -1.0f };

	const D3DXVECTOR3 vec3AxisX = { 1.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 vec3AxisY = { 0.0f, 1.0f, 0.0f };
	const D3DXVECTOR3 vec3AxisZ = { 0.0f, 0.0f, 1.0f };
}

static double AnimationTime[] = {
	-1.0,
	20.0/30.0,
	4.0/30.0,
	18.0/30.0,
	17.0/30.0,
	-1.0
};
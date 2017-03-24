#pragma once
#include "GameObject.h"
#include "CourceCamera.h"
#include "Particle.h"
#include "ParticleParameterTable.h"
#include "ObjectManager.h"

class CPlayer;

#define MAX_NAME 256

// �p�[�e�B�N���̔����@
class CParticleEmitter:public CGameObject
{
public:
	// �p�[�e�B�N�������퐶���֐��B
	// ����: �G�~�b�^�[�̖��O�B
	//		 ��������p�[�e�B�N���̎�ށB
	//		 �����ʒu�B
	//		 �J�����̃|�C���^�B
	//		 ���݂̃X�e�[�W�ԍ��B
	//		 �G�~�b�g�t���O(�I���Ȃ�p�[�e�B�N������)�B
	//		 �I�u�W�F�N�g�}�l�[�W���[�̃W�F�l���[�g�֐����g�p���ăG�~�b�^�[�𐶐����邩�B
	static CParticleEmitter* EmitterCreate(LPCSTR EmitterName, PARTICLE_TYPE type,D3DXVECTOR3 pos,CCamera* pcamera,STAGE_ID Id,bool Emitflg,bool IsManagerGeneration){
		CParticleEmitter* pEmitter = nullptr;
		if (IsManagerGeneration) {
			pEmitter = SINSTANCE(CObjectManager)->GenerationObject<CParticleEmitter>(EmitterName, OBJECT::PRIORTY::EMITTER, false);
		}
		else {
			pEmitter = new CParticleEmitter;
		}
		pEmitter->SetCamera(pcamera);
		pEmitter->SetEmitParameter(ParticleParamPT_Array[type]);
		pEmitter->SetEmitterName(EmitterName);
		pEmitter->SetEmitPos(pos);
		pEmitter->SetEmitFlg(Emitflg);
		pEmitter->SetStageID(Id);
		pEmitter->Initialize();
		return pEmitter;
	}

	CParticleEmitter();
	~CParticleEmitter();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Draw_EM(CCamera*)override;
	void ApplyForce(const D3DXVECTOR3&);
	void SetCamera(CCamera* camera){
		m_camera = camera;
	}
	void SetEmitParameter(SParticleEmitParameter* param){
		m_param = *param;
	}
	const SParticleEmitParameter& GetEmitParameter()const{
		return m_param;
	}
	void SetEmitPos(const D3DXVECTOR3& pos){
		m_emitPosition = pos;
	}
	const D3DXVECTOR3& GetEmitPos() const{
		return m_emitPosition;
	}
	void SetEmitterName(LPCSTR name){
		CH_ASSERT(strlen(name) < MAX_NAME);
		strcpy(m_EmitterName, name);
	}
	void SetEmitFlg(bool flg){
		m_EmitFlg = flg;
	}
	bool GetEmitFlg()const{
		return m_EmitFlg;
	}
	void SetDirection(const D3DXVECTOR3& dir){
		m_emitDirection = dir;
	}
	const D3DXVECTOR3& GetDirection()override{
		return m_emitDirection;
	}
	const D3DXVECTOR3& GetTailPos()const{
		return m_pTailParticle->GetPos();
	}
	bool GetResidual()const{
		return m_Residual;
	}
	void SetStageID(STAGE_ID id){
		m_Stage_ID = id;
	}
	int GetCourceNo()const{
		return m_CurrentCourceNo;
	}
	int GetPlayerCourceNo()const{
		return m_NowPlayerCourceNo;
	}
	void SetCourceLange(int lange){
		m_CourceLange = lange;
	}
	int GetCourceLange()const{
		return m_CourceLange; 
	}
	void SetIsUseCource(bool flg){
		m_IsUseCource = flg;
	}
	inline bool GetIsUseDeathTime()const {
		return m_IsUseDeathTime;
	}
public:
	// �G�~�b�^�[�Ɏ��̐鍐��^����֐��B
	// �w�肵�����Ԍ�ɃG�~�b�^�[���폜����B
	// ����:	�폜�܂ł̗P�\���ԁB
	// �߂�l:	�Ăяo�����̃C���X�^���X�Ɋi�[���邱�Ƃŉ���ς݂̃A�h���X���Q�Ƃ��Ȃ��B
	// �����̊֐����ĂԂƃG�~�b�^�[�͎����I�ɍ폜���s���܂��B
	inline CParticleEmitter* SentenceOfDeath(float DeathTime) {
		m_DeathTime = DeathTime;
		m_DeathCounter = 0.0f;
		m_IsUseDeathTime = true;
		return nullptr;
	}
private:
	void EmitParticle();
	void DeathCount();
private:
	float m_timer;
	CRandom m_random;
	CCamera* m_camera;
	SParticleEmitParameter m_param;
	D3DXVECTOR3 m_emitPosition;
	D3DXVECTOR3 m_emitDirection;	// �p�[�e�B�N���𔭐����������(�I�u�W�F�N�g�̕�����\�����̂Ƃ͕ʕ��Ƃ��Ē�`)�B
	list<CParticle*> m_ParticleList;
	char m_EmitterName[MAX_NAME];
	char m_ParticleName[MAX_NAME];
	int m_count;
	bool m_EmitFlg;	// �p�[�e�B�N���𐶐����邩�̃t���O
	int m_CurrentCourceNo;
	int m_NowPlayerCourceNo;
	CCourceDef m_CourceDef;
	CPlayer* m_pPlayer;
	CParticle* m_pTailParticle = nullptr;		// �Ō�ɐ��������p�[�e�B�N���̃|�C���^
	bool m_Residual;			// �Ō�ɐ��������p�[�e�B�N�����c�����Ă��邩
	int m_CourceLange;			// �v���C���[�ƃp�[�e�B�N���̔����ʒu���ǂꂭ�炢����Ă���Δ��������Ȃ���
	float m_DeathTime;	// �G�~�b�^�[�̎���(SentenceOfDeath�֐����Ă΂ꂽ���ォ��J�E���g)�B
	float m_DeathCounter;
	bool m_IsUseDeathTime = false;
	bool m_IsUseCource = true;	// �R�[�X��`���g�p���邩�B 
private:
	STAGE_ID m_Stage_ID;
};


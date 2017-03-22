#pragma once
#include "GameObject.h"
#include "CourceCamera.h"
#include "Particle.h"
#include "ParticleParameterTable.h"
#include "ObjectManager.h"
#include "Player.h"

#define MAX_NAME 256

// パーティクルの発生機
class CParticleEmitter:public CGameObject
{
public:
	// パーティクル発生器生成関数。
	// 引数: エミッターの名前。
	//		 生成するパーティクルの種類。
	//		 発生位置。
	//		 カメラのポインタ。
	//		 現在のステージ番号。
	//		 エミットフラグ(オンならパーティクル生成)。
	//		 オブジェクトマネージャーのジェネレート関数を使用してエミッターを生成するか。
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
		m_param = param;
	}
	SParticleEmitParameter* GetEmitParameter(){
		return m_param;
	}
	void SetEmitPos(D3DXVECTOR3 pos){
		m_emitPosition = pos;
	}
	D3DXVECTOR3 GetEmitPos(){
		return m_emitPosition;
	}
	void SetEmitterName(LPCSTR name){
		CH_ASSERT(strlen(name) < MAX_NAME);
		strcpy(m_EmitterName, name);
	}
	void SetEmitFlg(bool flg){
		m_EmitFlg = flg;
	}
	bool GetEmitFlg(){
		return m_EmitFlg;
	}
	void SetDirection(const D3DXVECTOR3& dir){
		m_emitDirection = dir;
	}
	const D3DXVECTOR3& GetDirection()override{
		return m_emitDirection;
	}
	D3DXVECTOR3 GetTailPos(){
		return *m_TailPosition;
	}
	bool GetResidual(){
		return m_Residual;
	}
	void SetStageID(STAGE_ID id){
		m_Stage_ID = id;
	}
	int GetCourceNo(){
		return m_CurrentCourceNo;
	}
	int GetPlayerCourceNo(){
		return m_NowPlayerCourceNo;
	}
	void SetCourceLange(int lange){
		m_CourceLange = lange;
	}
	int GetCourceLange(){
		return m_CourceLange; 
	}
	void SetIsUseCource(bool flg){
		m_IsUseCource = flg;
	}
	inline bool GetIsUseDeathTime() {
		return m_IsUseCource;
	}
public:
	// エミッターに死の宣告を与える関数。
	// 指定した時間後にエミッターを削除する。
	inline void SentenceOfDeath(float DeathTime) {
		m_DeathTime = DeathTime;
		m_DeathCounter = 0.0f;
		m_IsUseDeathTime = true;
	}
private:
	void EmitParticle();
	void DeathCount();
private:
	float m_timer;
	CRandom m_random;
	CCamera* m_camera;
	SParticleEmitParameter* m_param;
	D3DXVECTOR3 m_emitPosition;
	D3DXVECTOR3 m_emitDirection;	// パーティクルを発生させる向き(オブジェクトの方向を表すものとは別物として定義)。
	list<CParticle*> m_ParticleList;
	char m_EmitterName[MAX_NAME];
	char m_ParticleName[MAX_NAME];
	int m_count;
	bool m_EmitFlg;	// パーティクルを生成するかのフラグ
	int m_CurrentCourceNo;
	int m_NowPlayerCourceNo;
	CCourceDef m_CourceDef;
	CPlayer* m_pPlayer;
	D3DXVECTOR3* m_TailPosition;	// 最後に生成したプリミティブのポジションを追跡するためのポインタ
	CParticle* m_pTailParticle;		// 最後に生成したパーティクルのポインタ
	bool m_Residual;			// 最後に生成したパーティクルが残留しているか
	int m_CourceLange;			// プレイヤーとパーティクルの発生位置がどれくらい離れていれば発生させないか
	float m_DeathTime;	// エミッターの寿命(SentenceOfDeath関数が呼ばれた直後からカウント)。
	float m_DeathCounter;
	bool m_IsUseDeathTime = false;
	bool m_IsUseCource = true;	// コース定義を使用するか。 
private:
	STAGE_ID m_Stage_ID;
};


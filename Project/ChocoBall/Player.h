#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "DirectInput.h"
#include "ActreLight.h"
#include "islntersect.h"
#include "IsRepulsion.h"
#include "Turn.h"
#include "GameManager.h"
#include "CourceDef.h"
#include "LockOn.h"
#include "PlayerBullet.h"
#include "Vibration.h"
#include "CourceCamera.h"
#include "Audio.h"

class CLockOn;
class CCBManager;
class CParticleEmitter;
class MoveFloor;

#define PLAYER_JUMP_POWER 13.5f	//プレイヤーのジャンプの力。

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
		m_pRender->SetUpTechnique("Skin_Tex");
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("Skin_Tex_Lim");
	}
	void SetUpShadowTechnique()override {
		m_pShadowRender->SetUpTechnique("BoneShadowMapping");
	}

	void OnTriggerStay(const btCollisionObject* pCollision)override;
	void OnCollisionStay(const btCollisionObject* pCollision)override;
	
	// チョコボールとの当たりのコールバック関数。
	void ChocoHit();

	// エネミーと当たった時のコールバック。
	void EnemyHit();

	// ライト設定。
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
	GAMEEND::ID GetGameState()
	{
		return m_GameState;
	}
	bool GetChocoBall()
	{
		return ChocoBall;
	}
	int GetBusterEnemyNum()
	{
		return BusterEnemyNum;
	}

	//親をきめるやつ
	void SetParent(MoveFloor* parent);

	MoveFloor* GetParent(){
		return parent;
	}
	

	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
	inline STAGE_ID GetStageID() {
		return m_StageID;
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
		m_GameState = GAMEEND::ID::OVER;
	}
	bool IsVibration() ;
	
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
	int GetNowCourceNo() {
		return m_NowCourceNo;
	}
	const D3DXVECTOR3& GetSize() {
		return m_size;
	}
private:
	enum ANIMATION_STATE{ WAIT = 0, WALK,JUMP_START,JUMP_NOW,JUMP_END, DAMAGE};
	enum JUMP_STATE{J_NULL,J_EINS,J_ZWEI,J_DREI};
private:
	// パーティクル関連。
	CParticleEmitter* m_pEmitter;
	CParticleEmitter* m_pEmitter2;
	float m_JumpParticleTime;						//ジャンプ＆着地時のパーティクルが発生し続ける時間
	float m_JumpParticleTimer;						//ジャンプ＆着地時のパーティクルをを発生させる時間のカウンター
	//float m_GunParticleTime;						//銃発射時のパーティクルが発生し続ける時間
	//float m_GunParticleTimer;						//銃発射時のパーティクルをを発生させる時間のカウンター
	bool m_PreviousJumpFlag;			//パーティクルをジャンプの着地時に発生させるためのフラグ
	// ライト関連。
	D3DXVECTOR3		m_lightDir[NUM_DIFFUSE_LIGHT];
	D3DXVECTOR4		m_lightColor[NUM_DIFFUSE_LIGHT];	
	// プレイヤー制御関連。
	MOVE::STATE m_State;
	ANIMATION_STATE m_AnimState;
	float m_AnimInterpolation;	// アニメーションの補間時間。
	bool			m_ShotFlg;			//弾が発射されているのかのフラグ
	// プレイヤーアクション制御関連。
	float m_NowJumpPower;	// ジャンプ力。
	float			_X;
	float			m_radius;
	D3DXVECTOR3		m_initPosition;		//初期座標。
	D3DXVECTOR3		m_moveSpeed;		//移動速度。
	float			m_currentAngleY;	//現在の方向
	float			m_targetAngleY;		//向きたい方向
	float			m_turnSpeed;		//回転速度
	bool			isTurn;				//回転フラグ
	CTurn			m_Turn;				//CTurnのインスタンス
	CCourceDef		m_Courcedef;
	int m_NowCourceNo;
	D3DXVECTOR3 RV0;
	D3DXMATRIX Rot;
	D3DXVECTOR4 RV1;
	CVibration m_vibration;
	int BulletShotInterval;			//弾を発射する間隔の時間
	JUMP_STATE m_JumpState;
	float m_JumpEndInterval;
	float m_JumpEndCounter;
	float m_JumpStartInterval;
	float m_JumpStartCounter;
	float m_RunningCounter;
	float m_RunningRange;
	bool m_IsActive_X;
	bool m_IsActive_Y;
	// 当たり判定関連。
	EnemyBase* m_NearEnemy = nullptr;
	bool            LockOnflag;			//ロックオンフラグ
	EnemyBase* m_LockOnEnemy = nullptr;
	CLockOn         m_LockOn;			//LockOnのインスタンス
	CIsRepulsion    m_IsRepulsion;		// 他のオブジェクトがプレイヤーに与える影響を処理。
	CIsIntersect	m_IsIntersect;		// プレイヤーが移動した結果食い込んでいる場合の処理。
	float m_VibrationInterval;			// 振動してから次の振動判定までの猶予。
	float m_VibrationCounter;			// カウンター。
	// ゲームオーバー制御関連。
	GAMEEND::ID		m_GameState = GAMEEND::ID::CONTINUE;
	bool            GamaOverFlag;		//ゲームオーバー
	float			deadTimer;			//ゲームオーバーまでの待機時間
	// ギミック関連。
	bool			m_HitFlag;
	MoveFloor*		parent;
	D3DXVECTOR3		localPosition;
	bool            ChocoBall;			//チョコボールを流すフラグ
	int             BusterEnemyNum;		//倒した敵の数
	// その他。
	bool m_ActiveKeyState;	// キー判定を受け付けるか。
	CInterface*	m_pInput;
	D3DXVECTOR3		m_size;	//プレイヤーを内包するバウンディングボックスのサイズ。
	STAGE_ID m_StageID;
	CCourceCamera* m_pCamera;
	CAudio* m_pAudio;//SE使用
private:
	// ゲームステータス管理。
	void StateManaged();
	// プレイヤー制御関連。
	void MoveStateManaged();
	void Move();
	void UpDownMove();
	void LockOn();
	void BehaviorCorrection();
	void BulletShot();
	void RollingPlayer();
	// 当たり判定関連。
	void Collisions();
	// キー判定。
	void KeyState();
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
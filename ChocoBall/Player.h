#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "DirectInput.h"
#include "Light.h"
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

enum PLAYER_STATE{Wait = 0, Walk, Jump };
class CPlayer : public CGameObject
{
public:
	CPlayer(){
		m_pEmitter = nullptr;
		m_pCamera = nullptr;
		parent = nullptr;
		m_testFlg = false;
	};
	~CPlayer();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SetUpTechnique()override{
		m_pRender->SetUpTechnique("NotNormalMapAnimationFresnelTec");
	}
	void ConfigLight();
	void ReflectionLight(D3DXVECTOR4);
	void SetUpLight();
	void Move();
	void LockOn();
	void BehaviorCorrection();
	void StateManaged();
	void BulletShot();
	void ChocoHit();
	void RollingPlayer();

	void DeleteChocoBall(CCBManager*);
	void ExcuteDeleteChocoBall();

	void DeleteBullet(CPlayerBullet*);
	void ExcuteDeleteBullets();
	void EnemyBulletHit( D3DXVECTOR3 moveDir );
	void Setradius(float radius)
	{
		m_radius = radius;
	}
	bool GetShotflag()
	{
		return Shotflag;
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

	//親をきめるやつ
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
private:
	CInterface*	m_pInput;
	CParticleEmitter* m_pEmitter;
	CParticleEmitter* m_pEmitter2;
	CLight			m_light;
	D3DXVECTOR3		m_lightDir[NUM_DIFFUSE_LIGHT];
	D3DXVECTOR4		m_lightColor[NUM_DIFFUSE_LIGHT];	
	float			_X;
	float			m_radius;
	D3DXVECTOR3		m_initPosition;		//初期座標。
	D3DXVECTOR3		m_moveSpeed;		//移動速度。
	float			m_currentAngleY;	//現在の方向
	float			m_targetAngleY;		//向きたい方向
	float			m_turnSpeed;		//回転速度
	bool			isTurn;				//回転フラグ
	bool            LockOnflag;			//ロックオンフラグ
	int				m_lockonEnemyIndex;	//ロックオンしている敵のインデックス。
	CLockOn         m_LockOn;			//LockOnのインスタンス
	CIsIntersect	m_IsIntersect;		//CIsIntersectのインスタンス

	void UpdateLight();

	CTurn			m_Turn;				//CTurnのインスタンス
	bool			Shotflag;			//弾が発射されているのかのフラグ
	bool            Jumpflag;			//ジャンプフラグ
	GAMEEND_ID		m_GameState = GAMEEND_ID::CONTINUE;
	bool            GamaOverFlag;		//ゲームオーバー
	bool            ChocoBall;			//チョコボールを流すフラグ

	float			deadTimer;			//ゲームオーバーまでの待機時間
	int             BusterEnemyNum;		//倒した敵の数

	float m_JumpParticleTime;						//ジャンプ＆着地時のパーティクルが発生し続ける時間
	float m_JumpParticleTimer;						//ジャンプ＆着地時のパーティクルをを発生させる時間のカウンター

	//float m_GunParticleTime;						//銃発射時のパーティクルが発生し続ける時間
	//float m_GunParticleTimer;						//銃発射時のパーティクルをを発生させる時間のカウンター

	bool m_PreviousJumpFlag;			//パーティクルをジャンプの着地時に発生させるためのフラグ

	int BulletShotInterval;			//弾を発射する間隔の時間

	D3DXVECTOR3		m_size;	//プレイヤーを内包するバウンディングボックスのサイズ。

	list<CCBManager*> m_CBManager;
	list<CCBManager*> m_DeleteChocoBall;

	CCourceDef		m_Courcedef;
	D3DXVECTOR3 RV0;
	D3DXMATRIX Rot;
	D3DXVECTOR4 RV1;

	// 弾の配列
	vector<CPlayerBullet*> m_bullets;
	vector<CPlayerBullet*> m_Deletebullets;	// 削除リスト


	//入口
	
	bool			m_HitFlag;
	MoveFloor*		parent;
	D3DXVECTOR3		localPosition;


	//
	STAGE_ID m_StageID;
	bool m_MoveFlg;
	CVibration m_vibration;
	CCourceCamera* m_pCamera;

	CAudio* m_pAudio;//SE使用
	int m_NowCourceNo;

	static bool m_testFlg;
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
	20.0f/30.0f
};
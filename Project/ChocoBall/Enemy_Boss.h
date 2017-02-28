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
	// チョコボールに当たった際に呼ばれるコールバック。
	// 引数は当たったチョコボールのマネージャー。
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
	// 指定されたステートの添え字を参照してポインタを返却。
	CState* GetState(BOSS_STATE stateNum) {
		if (stateNum >= m_States.size()) {
			// 指定された添え字がステート配列に登録されている数より大きい。
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
	short m_HP;	// ヒットポイント。
	CHadBar* m_pHPBar;	// HPバー。

	BOSS_STATE m_State;
	PLAYING_STATE m_PlayingState;
	BOSS_ANIMATION m_AnimState;

	D3DXVECTOR3 m_size;

	btVector3 m_OriginOffset;	// モデルの原点と剛体の原点のオフセット。
	unique_ptr<btCapsuleShapeZ> m_pCapsuleCollision;	// 剛体の形状(今回はカプセル)。
	unique_ptr<btDefaultMotionState> m_myMotionState;
	unique_ptr<btRigidBody> m_pRigidBody;
	// コース定義判定用のコリジョン。
	btVector3 m_CollisionSize;
	unique_ptr<btBoxShape> m_pBoxShape_Cource;	// コリジョンの形状(今回は箱)。
	unique_ptr<btCollisionObject> m_pCollision;	// コリジョン。

	// コース定義。
	CBossCourceDef m_CourceDef;
	vector<BOSS_COURCE_BLOCK*> m_NowCource;
	bool m_IsNowCourceChange = false;	// 接触しているコースが前回から変化したか。
	bool m_IsUpdateCource = true;	// コースとの当たり判定を行うか。

	int m_DamageCounter = 0;
	CState* m_pCurrentState = nullptr;
	vector<unique_ptr<CState>> m_States;	// ボスのステートすべてを格納。

	vector<CDivisionWall*>* m_Walls;
	int m_WallsIdx = 0;	// Wallsの添え字。

	// ボスを守るバリア(ボスが攻撃時のみ解除され、チョコボールが当たる)。
	CBarrier* m_pBarrier = nullptr;

	CAudio* m_pAudio = nullptr;
};

#define Max_AnimNum 6

// クジラエネミー。
static double BossAnimationTime[] = {
	198.0 / 30.0,//待機。
	79.0 / 30.0,//早く泳ぐ。
	//84.0 / 30.0,//早く泳ぐ。
	180.0 / 30.0,//遅く泳ぐ。
	//214.0 / 30.0,//遅く泳ぐ。
	698.0 / 30.0,//上昇。
	105.0 / 30.0,//下降。
	382.0 / 30.0,//潜行。
};

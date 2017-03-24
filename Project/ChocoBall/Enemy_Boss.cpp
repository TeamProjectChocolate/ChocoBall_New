#include "stdafx.h"
#include "Enemy_Boss.h"
#include "ActreLight.h"
#include "ObjectManager.h"
#include "HadBar.h"
#include "InputManager.h"
#include "ShadowRender.h"
#include "ChocoBall.h"
#include "EnemyBullet.h"
#include "AttackState.h"
#include "EscapeState.h"
#include "MoveState.h"
#include "WaitState.h"
#include "ShotState.h"
#include "TransState.h"
#include "RotateState.h"
#include "CourceDef.h"
#include "SleepState.h"
#include "CBManager.h"
#include "ShotAttackState.h"
#include "RushAttackState.h"

CEnemy_Boss::CEnemy_Boss()
{
}


CEnemy_Boss::~CEnemy_Boss()
{
	this->OnDestroy();
}

void CEnemy_Boss::InitState() {
	// ボスのステートをすべて登録していく。
	m_States.push_back(unique_ptr<CState>(new CSleepState(this)));
	m_States.push_back(unique_ptr<CState>(new CWaitState(this)));
	m_States.push_back(unique_ptr<CState>(new CMoveState(this)));
	m_States.push_back(unique_ptr<CState>(new CAttackState(this)));
	m_States.push_back(unique_ptr<CState>(new CEscapeState(this)));
	m_States.push_back(unique_ptr<CState>(new CShotAttackState(this)));
	m_States.push_back(unique_ptr<CState>(new CRushAttackState(this)));
	m_States.push_back(unique_ptr<CState>(new CShotState(this)));
	m_States.push_back(unique_ptr<CState>(new CTransState(this)));
	m_States.push_back(unique_ptr<CState>(new CRotateState(this)));

	// 何もしないステート。
	m_States.push_back(unique_ptr<CState>(new CState(this)));
}

void CEnemy_Boss::SetInitPosition(const D3DXVECTOR3& pos)
{
	m_transform.position = pos;
	// 剛体にポジションを設定。
	m_CollisionObject->SetPos(pos);
	// コース判定用のコリジョンにポジションを設定。
	m_CourceCollision.SetPos(pos);
	// 現在のコース取得。
	m_IsNowCourceChange = m_CourceDef.FindCource(m_CourceCollision.GetCollisionObject(), &m_NowCource);
	// バリア初期化。
	m_pBarrier = SINSTANCE(CObjectManager)->GenerationObject<CBarrier>(_T("Barrier"), OBJECT::PRIORTY::OBJECT3D_ALPHA, false);
	m_pBarrier->Initialize();
	m_pBarrier->Build(m_transform.position, 33.0f);
	// 最初のステートに移行。
	//this->ChangeState(BOSS_STATE::Sleep);
	this->ChangeState(BOSS_STATE::RushAttack);

}

void CEnemy_Boss::Initialize() {
	UseModel<C3DImage>();
	SINSTANCE(CShadowRender)->Entry(this);

	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_pModel->SetFileName("image/humpback_whale_model23.X");
	CGameObject::Initialize();
	SetRotation(D3DXVECTOR3(0, 1, 0), 0.1f);

	m_size = D3DXVECTOR3(5.0f,5.0f,20.0f);

	m_PlayingState = ANIMATION::PLAYING_STATE::REPEAT;
	m_AnimState = BOSS_ANIMATION::Wait;
	m_State = BOSS_STATE::BWait;
	m_pModel->SetUseBorn(true);
	for (int idx = 0; idx < m_pModel->GetAnimation()->GetNumAnimationSet(); idx++) {
		m_pModel->GetAnimation()->SetAnimationEndtime(idx, BossAnimationTime[idx]);
	}
	//m_pModel->GetAnimation()->PlayAnimation(m_AnimState, 0.0f);
	ConfigLight();
	SetAlive(true);
	m_HP = 3000;
	m_pModel->SetAlpha(1.0f);

	// HPバー生成。
	m_pHPBar = SINSTANCE(CObjectManager)->GenerationObject<CHadBar>(_T("BossHPBar"),OBJECT::PRIORTY::OBJECT2D,false);
	vector<BarColor> ColorArray;
	ColorArray.push_back(BarColor::Green);
	ColorArray.push_back(BarColor::Yellow);
	ColorArray.push_back(BarColor::Red);
	m_pHPBar->Initialize(ColorArray,m_HP,m_HP);

	// 剛体を生成。
	{
		btScalar Radius = m_CapsuleRadius * 0.5f;
		btScalar Height = (m_CapsuleHeight * 0.5f) - Radius;
		//ここで剛体の形状を決定。
		btCapsuleShapeZ* pShape = new btCapsuleShapeZ(Radius, Height);
		ActivateCollision(D3DXVECTOR3(0.0f, -0.25f, 0.0f),pShape,Collision::Type::Boss,Collision::FilterGroup::Enemy,false,0.0f,true,true);
		m_CollisionObject->BitMask_AllOff();
		m_CollisionObject->BitMask_On(Collision::FilterGroup::Player);
		m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);
	}

	// コース判定用のコリジョンを生成。
	{
		m_CourceCollision.InitCollision(this, m_transform, Vector3::Zero, new btBoxShape(btVector3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 2.0f)), Collision::Type::Boss_Gost, Collision::FilterGroup::Boss_Ghost , 0.0f, true, true);
		m_CourceCollision.BitMask_AllOff();
		m_CourceCollision.BitMask_On(Collision::FilterGroup::Boss_Cource);
	}

	// コース定義初期化。
	m_CourceDef.Initialize();
	m_IsNowCourceChange = false;

	// ボスで使用するステートをすべて保持。
	this->InitState();
}

void CEnemy_Boss::Update() {

	if (m_IsUpdateCource) {
		// 当たり判定の結果で現在いるコースを更新。
		m_IsNowCourceChange = m_CourceDef.FindCource(m_CourceCollision.GetCollisionObject(), &m_NowCource);
	}

	// 現在のステートを更新。
	m_pCurrentState->Update();

	// アニメーション再生関数を呼び出す
	m_pModel->SetCurrentAnimNo(m_AnimState);
	if (m_PlayingState == ANIMATION::PLAYING_STATE::REPEAT) {
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.5f,true);
	}
	else if (m_PlayingState == ANIMATION::PLAYING_STATE::ONCE) {
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.5f,false);
	}

	CGameObject::Update();

	// コース判定用のコリジョン更新。
	m_CourceCollision.SetPos(m_transform.position);

	m_pBarrier->SetPos(m_transform.position);
}

void CEnemy_Boss::ChangeState(BOSS_STATE NextState) {
	if (NextState >= m_States.size()) {
		// 渡された値が登録外の値。
		abort();
	}
	if (m_pCurrentState) {
		m_pCurrentState->Exit(NextState);
	}

	m_pCurrentState = m_States[NextState].get();

	m_pCurrentState->Entry();
	m_State = NextState;
}

void CEnemy_Boss::Draw() {
	if (GetAlive())
	{
		SetUpTechnique();
		CGameObject::Draw();
	}
}

void CEnemy_Boss::ChocoHit(CCBManager* HitChocoManager) {
	if (m_pCurrentState->IsPossibleDamage()) {
		m_HP -= BossDmage;
		m_DamageCounter += BossDmage;
		if (m_HP <= 0) {
			// 暫定処理。
			m_HP = 0;
			SetAlive(false);
			this->DivisionWallOpen();
		}
		else if (m_DamageCounter >= BossDmage * 10) {
			HitChocoManager->SetIsBossDamage(false);
			m_DamageCounter = 0;
			this->ChangeState(BOSS_STATE::Escape);
			static_cast<CEscapeState*>(m_pCurrentState)->SetHitCBManager(HitChocoManager);
		}
		m_pHPBar->SetValue(m_HP);
	}
}

void CEnemy_Boss::DivisionWallOpen() {
	if (m_WallsIdx < m_Walls->size()) {
		vector<CDivisionWall*> walls = *m_Walls;
		walls[m_WallsIdx]->SetIsMove(true);
		m_WallsIdx++;
	}
}

void CEnemy_Boss::OnDestroy()
{
	SINSTANCE(CShadowRender)->DeleteObjectImidieit(this);
	m_CollisionObject.reset();
	//m_CourceCollision.RemoveWorld();
}

void CEnemy_Boss::ConfigLight() {
	if (!m_pLight) {
		m_pLight = new CActreLight;
	}
	CGameObject::ConfigLight();

	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(0, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(1.0f, 0.0f, 0.0f));

	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(1, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(2, D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//static_cast<CActreLight*>(m_pLight)->SetOrigDiffuseLightDirection(3, D3DXVECTOR3(0.0f, 0.0f, -1.0f));

	m_pLight->SetDiffuseLightColor(0, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(1, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(2, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetDiffuseLightColor(3, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pLight->SetLimLightDirection(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_pLight->SetLimLightColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	m_pLight->SetAmbientLight(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
}
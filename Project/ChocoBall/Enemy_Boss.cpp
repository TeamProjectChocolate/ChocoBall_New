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
#include "DeathState.h"

CEnemy_Boss::CEnemy_Boss()
{
}


CEnemy_Boss::~CEnemy_Boss()
{
	this->OnDestroy();
}

void CEnemy_Boss::InitState() {
	// �{�X�̃X�e�[�g�����ׂēo�^���Ă����B
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
	m_States.push_back(unique_ptr<CState>(new CDeathState(this)));

	// �������Ȃ��X�e�[�g�B
	m_States.push_back(unique_ptr<CState>(new CState(this)));
}

void CEnemy_Boss::SetInitPosition(const D3DXVECTOR3& pos)
{
	m_transform.position = pos;
	// ���̂Ƀ|�W�V������ݒ�B
	m_CollisionObject->SetPos(pos);
	// �R�[�X����p�̃R���W�����Ƀ|�W�V������ݒ�B
	m_CourceCollision.SetPos(pos);
	// ���݂̃R�[�X�擾�B
	m_IsNowCourceChange = m_CourceDef.FindCource(m_CourceCollision.GetCollisionObject(), &m_NowCource);
	// �o���A�������B
	m_pBarrier = SINSTANCE(CObjectManager)->GenerationObject<CBarrier>(_T("Barrier"), OBJECT::PRIORTY::OBJECT3D_ALPHA, false);
	m_pBarrier->Initialize();
	m_pBarrier->Build(m_transform.position, 33.0f);
	// �ŏ��̃X�e�[�g�Ɉڍs�B
	this->ChangeState(BOSS_STATE::Sleep);
	//this->ChangeState(BOSS_STATE::Attack);
	//this->ChangeState(BOSS_STATE::BMove);

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
	m_HP = 7200.0f;
	//m_HP = 1800.0f;
	//m_HP = 10.0f;
	m_pModel->SetAlpha(1.0f);

	m_Player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));

	// HP�o�[�����B
	m_pHPBar = SINSTANCE(CObjectManager)->GenerationObject<CHadBar>(_T("BossHPBar"),OBJECT::PRIORTY::OBJECT2D,false);

	vector<BarColor> ColorArray;
	ColorArray.push_back(BarColor::Blue);
	ColorArray.push_back(BarColor::Green);
	ColorArray.push_back(BarColor::Yellow);
	ColorArray.push_back(BarColor::Red);
	m_pHPBar->Initialize(this,ColorArray,m_HP,m_HP);

	// ���̂𐶐��B
	{
		btScalar Radius = m_CapsuleRadius * 0.5f;
		btScalar Height = (m_CapsuleHeight * 0.5f) - Radius;
		//�����ō��̂̌`�������B
		btCapsuleShapeZ* pShape = new btCapsuleShapeZ(Radius, Height);
		ActivateCollision(D3DXVECTOR3(0.0f, -0.25f, 0.0f),pShape,Collision::Type::Boss,Collision::FilterGroup::Enemy,false,0.0f,true,true);
		m_CollisionObject->BitMask_AllOff();
		m_CollisionObject->BitMask_On(Collision::FilterGroup::Player);
		m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);
	}

	// �R�[�X����p�̃R���W�����𐶐��B
	{
		m_CourceCollision.InitCollision(this, m_transform, Vector3::Zero, new btBoxShape(btVector3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 2.0f)), Collision::Type::Boss_Gost, Collision::FilterGroup::Boss_Ghost , 0.0f, true, true);
		m_CourceCollision.BitMask_AllOff();
		m_CourceCollision.BitMask_On(Collision::FilterGroup::Boss_Cource);
	}

	// �R�[�X��`�������B
	m_CourceDef.Initialize();
	m_IsNowCourceChange = false;

	// �{�X�Ŏg�p����X�e�[�g�����ׂĕێ��B
	this->InitState();
}

void CEnemy_Boss::Update() {

	if (m_IsUpdateCource) {
		// �����蔻��̌��ʂŌ��݂���R�[�X���X�V�B
		m_IsNowCourceChange = m_CourceDef.FindCource(m_CourceCollision.GetCollisionObject(), &m_NowCource);
	}

	// ���݂̃X�e�[�g���X�V�B
	m_pCurrentState->Update();

	// �A�j���[�V�����Đ��֐����Ăяo��
	m_pModel->SetCurrentAnimNo(m_AnimState);
	if (m_PlayingState == ANIMATION::PLAYING_STATE::REPEAT) {
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.5f,true);
	}
	else if (m_PlayingState == ANIMATION::PLAYING_STATE::ONCE) {
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.5f,false);
	}

	CGameObject::Update();

	// �R�[�X����p�̃R���W�����X�V�B
	m_CourceCollision.SetPos(m_transform.position);

	m_pBarrier->SetPos(m_transform.position);
}

void CEnemy_Boss::ChangeState(BOSS_STATE NextState) {
	if (NextState >= m_States.size()) {
		// �n���ꂽ�l���o�^�O�̒l�B
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
		// ���̃X�e�[�g�̓{�X�Ƀ_���[�W��^������B

		float BossDamage;	// �ꗱ�̃`���R�{�[�����{�X�ɗ^����_���[�W�B
		float MaxDamage;	// ��̃`���R�{�[���}�l�[�W���[���{�X�ɗ^������ő�_���[�W�B

		switch (static_cast<HPBarNo>(m_pHPBar->GetNowBarNum())) {
		case HPBarNo::One:
			// ���Q�[�W�B
			// �P���v�Z3��B
			BossDamage = 20.0f;
			MaxDamage = 600.0f;
			break;
		case HPBarNo::Two:
			// ���Q�[�W�B
			// �P���v�Z4��B
			BossDamage = 15.0f;
			MaxDamage = 450.0f;
			break;
		case HPBarNo::Three:
			// ��O�Q�[�W�B
			// �P���v�Z3��B
			BossDamage = 20.0f;
			MaxDamage = 600.0f;
			break;
		case HPBarNo::Last:
			// �ŏI�Q�[�W�B
			// �P���v�Z5��B
			BossDamage = 12.0f;
			MaxDamage = 360.0f;
			break;
		default:
			BossDamage = 0.0f;
			MaxDamage = 0.0f;
			break;
		}

		m_HP -= BossDamage;
		m_DamageCounter += BossDamage;

		if (m_HP <= 0) {
			if (m_State != BOSS_STATE::Death) {
				if (m_Player->GetGameState() != GAMEEND::ID::OVER) {
					// �{�X���S�����B
					// �b�菈���B
					m_HP = 0;
					//SetAlive(false);
					this->DivisionWallOpen();
					ChangeState(BOSS_STATE::Death);
				}
			}
		}
		else if (m_DamageCounter >= MaxDamage) {
			HitChocoManager->SetIsBossDamage(false);
			m_DamageCounter = 0.0f;
			this->ChangeState(BOSS_STATE::Escape);
			static_cast<CEscapeState*>(m_pCurrentState)->SetHitCBManager(HitChocoManager);
		}
		m_pHPBar->SetValue(m_HP);
	}
}

void CEnemy_Boss::BreakEventCallBack() {
	SetIsBreak(true);
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
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

	// �������Ȃ��X�e�[�g�B
	m_States.push_back(unique_ptr<CState>(new CState(this)));
}

void CEnemy_Boss::SetInitPosition(const D3DXVECTOR3& pos)
{
	m_transform.position = pos;
	// ���̂Ƀ|�W�V������ݒ�B
	m_pRigidBody->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	m_pCollision->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	// ���݂̃R�[�X�擾�B
	m_IsNowCourceChange = m_CourceDef.FindCource(m_pCollision.get(), &m_NowCource);
	// �o���A�������B
	m_pBarrier = SINSTANCE(CObjectManager)->GenerationObject<CBarrier>(_T("Barrier"), PRIORTY::OBJECT3D_ALPHA, false);
	m_pBarrier->Initialize();
	m_pBarrier->Build(m_transform.position, 33.0f);
	// �ŏ��̃X�e�[�g�Ɉڍs�B
	this->ChangeState(BOSS_STATE::Sleep);
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

	m_PlayingState = PLAYING_STATE::REPEAT;
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

	// HP�o�[�����B
	m_pHPBar = SINSTANCE(CObjectManager)->GenerationObject<CHadBar>(_T("BossHPBar"),PRIORTY::OBJECT2D,false);
	vector<BarColor> ColorArray;
	ColorArray.push_back(BarColor::Green);
	ColorArray.push_back(BarColor::Yellow);
	ColorArray.push_back(BarColor::Red);
	m_pHPBar->Initialize(ColorArray,m_HP,m_HP);

	// ���̂𐶐��B
	{
		btScalar Radius = 3.5f * 0.5f;
		btScalar Height = (30.0f * 0.5f) - Radius;
		m_pCapsuleCollision.reset(new btCapsuleShapeZ(Radius,Height));//�����ō��̂̌`�������
		m_OriginOffset = btVector3(0.0f, -0.25f, 0.0f);
		btTransform rbTransform;
		rbTransform.setIdentity();
		rbTransform.setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z) + m_OriginOffset);
		rbTransform.setRotation(btQuaternion(m_transform.angle.x, m_transform.angle.y, m_transform.angle.z, m_transform.angle.w));

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		m_myMotionState.reset(new btDefaultMotionState(rbTransform));
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, m_myMotionState.get(), m_pCapsuleCollision.get(), btVector3(0, 0, 0));
		m_pRigidBody.reset(new btRigidBody(rbInfo));
		m_pRigidBody->setUserIndex(CollisionType::CollisionType_Boss);
		m_pRigidBody->activate();
		//���[���h�ɒǉ��B
		//SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddCollisionObject_Dynamic();

		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody_Dynamic(m_pRigidBody.get());
	}

	// �R�[�X����p�̃R���W�����𐶐��B
	{
		m_CollisionSize = btVector3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 2.0f);
		m_pBoxShape_Cource.reset(new btBoxShape(m_CollisionSize));//�����ŃR���W�����̌`�������
		btTransform rbTransform;
		rbTransform.setIdentity();
		rbTransform.setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z));
		m_pCollision.reset(new btCollisionObject);
		m_pCollision->setWorldTransform(rbTransform);
		m_pCollision->setCollisionShape(m_pBoxShape_Cource.get());
		//���[���h�ɒǉ��B
		SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddCollisionObject(m_pCollision.get());
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
		m_IsNowCourceChange = m_CourceDef.FindCource(m_pCollision.get(), &m_NowCource);
	}

	// ���݂̃X�e�[�g���X�V�B
	m_pCurrentState->Update();

	// �A�j���[�V�����Đ��֐����Ăяo��
	m_pModel->SetCurrentAnimNo(m_AnimState);
	if (m_PlayingState == PLAYING_STATE::REPEAT) {
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.5f,true);
	}
	else if (m_PlayingState == PLAYING_STATE::ONCE) {
		m_pModel->GetAnimation()->Play(m_pModel->GetCurrentAnimNo(), 0.5f,false);
	}

	CGameObject::Update();

	// ���̏��X�V�B
	m_pRigidBody->getWorldTransform().setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z) + m_OriginOffset);
	m_pRigidBody->getWorldTransform().setRotation(btQuaternion(m_transform.angle.x, m_transform.angle.y, m_transform.angle.z, m_transform.angle.w));
	// �R�[�X����p�̍��̍X�V�B
	m_pCollision->getWorldTransform().setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z));

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
		m_HP -= BossDmage;
		m_DamageCounter += BossDmage;
		if (m_HP <= 0) {
			// �b�菈���B
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
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveRigidBody_Dynamic(m_pRigidBody.get());
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->RemoveCollisionObject(m_pCollision.get());
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
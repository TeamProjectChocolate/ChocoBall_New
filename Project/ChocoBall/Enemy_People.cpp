#include "stdafx.h"
#include "Enemy_People.h"
#include "ActreLight.h"
#include "BulletPhysics.h"
#include "Rigidbody.h"

CEnemy_People::CEnemy_People()
{
}


CEnemy_People::~CEnemy_People()
{
}

void CEnemy_People::Initialize() {
	EnemyBase::Initialize();
	m_pModel->SetFileName("image/enem.x");
	CGameObject::Initialize();
	m_pModel->m_Refractive = 0.34f;
	m_AnimState = ENEMY_ANIMATION::Wait;
	m_State = MOVE::STATE::Wait;
	m_pModel->SetUseBorn(true);
	for (int idx = 0; idx < m_pModel->GetAnimation()->GetNumAnimationSet(); idx++) {
		m_pModel->GetAnimation()->SetAnimationEndtime(idx, EnemyAnimationTime[Enemy_ModelType::People][idx]);
	}
	m_pModel->GetAnimation()->Play(-1, 0.0f,true);
	ConfigLight();

	btSphereShape* shape = new btSphereShape(1.0f);
	float mass = 1.0f;
	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), shape, Collision::Type::Enemy,Collision::FilterGroup::Enemy, false, mass, false, false);
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Enemy);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Map);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Gimmick);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Player);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);

	//第一引数は質量、第二引数は回転のしやすさ。
	static_cast<CRigidbody*>(m_CollisionObject.get())->SetMassProps(mass, D3DXVECTOR3(0.01f, 0.01f, 0.01f));
}

void CEnemy_People::Update() {
	switch (m_State) {
	case MOVE::STATE::Wait:
		m_AnimState = ENEMY_ANIMATION::Wait;
		m_CurrentAngleY = m_Turn.Update(m_IsTurn, m_TargetAngleY);
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_CurrentAngleY);
		m_PlayingState = ANIMATION::PLAYING_STATE::REPEAT;
		break;
	case MOVE::STATE::Walk:
		m_AnimState = ENEMY_ANIMATION::Walk;
		Move();
		m_CurrentAngleY = m_Turn.Update(m_IsTurn, m_TargetAngleY);
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_CurrentAngleY);
		m_PlayingState = ANIMATION::PLAYING_STATE::REPEAT;
		break;
	case MOVE::STATE::RockOn:
		m_AnimState = ENEMY_ANIMATION::Stance;
		Move();
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), /*m_TargetAngleY*/m_CurrentAngleY);
		m_PlayingState = ANIMATION::PLAYING_STATE::ONCE;
		break;
	case MOVE::STATE::Fly:
		//m_AnimState = ENEMY_ANIMATION::Down;
		//死亡までの待機時間の設定
		//m_deadTimer += 1.0 / 60.0f;
		//if (m_deadTimer >= EnemyAnimationTime[Enemy_ModelType::People][ENEMY_ANIMATION::Down]) {
		//	SetAlive(false);
		//}
		//m_PlayingState = PLAYING_STATE::ONCE;
		m_AnimState = ENEMY_ANIMATION::Wait;
		RollingEnemy();
		break;
	}
	EnemyBase::Update();
}

void CEnemy_People::HitReaction(D3DXVECTOR3 Dir) {
	m_State = MOVE::STATE::Fly;
	m_pModel->GetAnimation()->SetAnimSpeed(2.0f);//アニメーション再生速度を設定

	// 剛体設定。
	{
		m_CollisionObject->AddWorld();
		Dir *= 750.0f;
		float Power = 1000.0f;
		static_cast<CRigidbody*>(m_CollisionObject.get())->ApplyForce(Dir + (Vector3::Up * Power));//チョコボールに当たって吹っ飛ぶ力を設定
		static_cast<CRigidbody*>(m_CollisionObject.get())->SetAngularVelocity(D3DXVECTOR3(5.0f, 5.0f, 5.0f));
		// キネマティック解除。
		static_cast<CRigidbody*>(m_CollisionObject.get())->OnDynamic();
	}
}

void CEnemy_People::RollingEnemy()
{
	//死亡までの待機時間の設定
	m_deadTimer += 1.0 / 60.0f;
	if (m_deadTimer >= 2.0f) {
		SetAlive(false);
	}
}

void CEnemy_People::ConfigLight() {
	EnemyBase::ConfigLight();

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
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
	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), shape, CollisionType::Enemy, false, mass, false, false);
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(CollisionType::Boss);
	m_CollisionObject->BitMask_On(CollisionType::Map);
	m_CollisionObject->BitMask_On(CollisionType::Wall);
	m_CollisionObject->BitMask_On(CollisionType::Floor);
	m_CollisionObject->BitMask_On(CollisionType::Player);
	m_CollisionObject->BitMask_On(CollisionType::Chocoball);

	//�������͎��ʁA�������͉�]�̂��₷���B
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
		//���S�܂ł̑ҋ@���Ԃ̐ݒ�
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
	m_pModel->GetAnimation()->SetAnimSpeed(2.0f);//�A�j���[�V�����Đ����x��ݒ�

	// ���̐ݒ�B
	{
		m_CollisionObject->AddWorld();
		Dir *= 750.0f;
		float Power = 1000.0f;
		static_cast<CRigidbody*>(m_CollisionObject.get())->ApplyForce(Dir + (Vector3::Up * Power));//�`���R�{�[���ɓ������Đ�����ԗ͂�ݒ�
		static_cast<CRigidbody*>(m_CollisionObject.get())->SetAngularVelocity(D3DXVECTOR3(5.0f, 5.0f, 5.0f));
	}
}

void CEnemy_People::RollingEnemy()
{
	btRigidBody* rb = m_IsIntersect.GetRigidBody();//�G�l�~�[�̍��̂��擾

												   //�����G���W���Ōv�Z�����ړ����G�l�~�[�ɔ��f
	btVector3 pos = rb->getWorldTransform().getOrigin();
	m_transform.position.x = pos.x();
	m_transform.position.y = pos.y();
	m_transform.position.z = pos.z();

	//�����G���W���Ōv�Z������]���G�l�~�[�ɔ��f
	btQuaternion rot = rb->getWorldTransform().getRotation();
	m_transform.angle.x = rot.x();
	m_transform.angle.y = rot.y();
	m_transform.angle.z = rot.z();
	m_transform.angle.w = rot.w();

	//���S�܂ł̑ҋ@���Ԃ̐ݒ�
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
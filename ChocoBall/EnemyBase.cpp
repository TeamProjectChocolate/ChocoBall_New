#include "stdafx.h"
#include "EnemyBase.h"
#include "islntersect.h"
#include "EnemyManager.h"

void EnemyBase::Build(){
	m_pEnemyManager = SINSTANCE(CObjectManager)->FindGameObject<CEnemyManager>(_T("EnemyManager"));
}

void EnemyBase::Initialize(){
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/enem.x");
	CGameObject::Initialize();
	m_pModel->m_Refractive = 0.34f;
	m_AnimState = ENEMY_ANIMATION::Wait;
	m_State = MOVE_STATE::Wait;
	m_pModel->SetUseBorn(true);
	for (int idx = 0; idx < m_pModel->GetAnimation()->GetNumAnimationSet(); idx++){
		m_pModel->GetAnimation()->SetAnimationEndtime(idx, EnemyAnimationTime[idx]);
	}
	m_pModel->GetAnimation()->PlayAnimation(-1, 0.0f);
	SINSTANCE(CShadowRender)->Entry(this);
}

void EnemyBase::Update(){
	switch (m_State){
	case MOVE_STATE::Wait:
		m_AnimState = ENEMY_ANIMATION::Wait;
		m_CurrentAngleY = m_Turn.Update(m_IsTurn, m_TargetAngleY);
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_CurrentAngleY);
		m_PlayingState = PLAYING_STATE::REPEAT;
		break;
	case MOVE_STATE::Walk:
		m_AnimState = ENEMY_ANIMATION::Walk;
		Move();
		m_transform.position += m_Direction * m_moveSpeed;
		m_CurrentAngleY = m_Turn.Update(m_IsTurn, m_TargetAngleY);
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_CurrentAngleY);
		m_PlayingState = PLAYING_STATE::REPEAT;
		break;
	case MOVE_STATE::RockOn:
		m_AnimState = ENEMY_ANIMATION::Stance;
		Move();
		SetRotation(D3DXVECTOR3(0.0f, 1.0f, 0.0f), /*m_TargetAngleY*/m_CurrentAngleY);
		m_PlayingState = PLAYING_STATE::ONCE;
		break;
	case MOVE_STATE::Fly:
		m_AnimState = ENEMY_ANIMATION::Down;
		//���S�܂ł̑ҋ@���Ԃ̐ݒ�
		m_deadTimer += 1.0 / 60.0f;
		if (m_deadTimer >= EnemyAnimationTime[ENEMY_ANIMATION::Down]){
			SetAlive(false);
		}
		m_PlayingState = PLAYING_STATE::ONCE;
		//m_AnimState = ENEMY_ANIMATION::Wait;
		//RollingEnemy();
		break;
	}

	// �A�j���[�V�����Đ��֐����Ăяo��
	m_pModel->SetCurrentAnimNo(m_AnimState);
	if (m_PlayingState == PLAYING_STATE::REPEAT){
		m_pModel->GetAnimation()->PlayAnimation(m_pModel->GetCurrentAnimNo(), 0.1f);

	}
	else if (m_PlayingState == PLAYING_STATE::ONCE){
		m_pModel->GetAnimation()->PlayAnimation_Once(m_pModel->GetCurrentAnimNo(), 0.1f);
	}
	CGameObject::Update();
}

void EnemyBase::OnDestroy()
{
	SINSTANCE(CShadowRender)->DeleteObjectImidieit(this);
}

void EnemyBase::PlayerBulletHit(D3DXVECTOR3 moveDir)
{
	m_IsIntersect.CollisitionInitialize(&m_transform.position, 1.0f, CollisionType_Enemy);
	m_State = MOVE_STATE::Fly;
	//btRigidBody* rb = m_IsIntersect.GetRigidBody();//�G�l�~�[�̍��̂��擾
	////m_IsIntersect.GetSphereShape()->setLocalScaling(btVector3(0.3f, 0.3f, 0.3f));//�G�l�~�[�̋����������ݒ肵�A�`���R�{�[���ɖ������悤�ɂ��Ă���B
	//rb->setMassProps(1.0f, btVector3(0.01f, 0.01f, 0.01f));//�������͎��ʁA�������͉�]�̂��₷��
	//moveDir *= 750.0f;
	//rb->applyForce(btVector3(moveDir.x, moveDir.y + 1000.0f, moveDir.z), btVector3(1.0f, 1.0f, 1.0f));//�`���R�{�[���ɓ������Đ�����ԗ͂�ݒ�
	//rb->setAngularVelocity(btVector3(5.0f, 5.0f, 5.0f));
	//m_pModel->GetAnimation()->SetAnimSpeed(2.0f);//�A�j���[�V�����Đ����x��ݒ�
}

void EnemyBase::RollingEnemy()
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
	if (m_deadTimer >= 2.0f){
		SetAlive(false);
	}
}

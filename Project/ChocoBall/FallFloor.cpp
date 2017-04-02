#include "stdafx.h"
#include "FallFloor.h"
#include "ShadowRender.h"

bool FallingFloor::m_IsPlayCue = false;

void FallingFloor::Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scale)
{
	UseModel<C3DImage>();
	SINSTANCE(CShadowRender)->Entry(this);
	m_pModel->SetFileName("image/down_block.x");
	CGameObject::Initialize();
	m_transform.position = pos;
	StartPos = m_transform.position;
	m_transform.scale = scale;
	m_transform.angle = rot;
	m_MaxSpeed = 0.1f;
	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.9f * scale.x *0.5f, 0.5f *  scale.y * 0.5f, 1.9f * scale.z * 0.5f)), Collision::Type::Floor,Collision::FilterGroup::Gimmick, false, 0.0f, true,true);
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Player);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Enemy);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Gimmick);

	m_player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));

	//m_RigidBody->setActivationState(DISABLE_DEACTIVATION);

	SetAlive(true);
}


void FallingFloor::Update()
{
	D3DXVECTOR3 PlayerPos = m_player->GetPos();

	if (m_IsHitPlayer)
	{
		m_moveSpeed += 0.02f;
		if (m_MaxSpeed <= m_moveSpeed){
			m_moveSpeed = m_MaxSpeed;
		}
		if (m_MaxMove == -1 || m_transform.position.y > StartPos.y - m_MaxMove){
			if (!m_IsPlayCue){
 				m_pAudio->PlayCue("Lift", false,this);//リフトAudio
				m_IsPlayCue = true;
				m_IamFlgKeeper = true;
			}
			m_transform.position.y -= m_moveSpeed;
			PlayerPos.y -= m_moveSpeed;
			m_player->SetPos(PlayerPos);
		}
		else{
			if (m_IamFlgKeeper){
				m_pAudio->StopCue("Lift",false,this);//リフトAudio
				m_IsPlayCue = false;
				m_IamFlgKeeper = false;
			}
		}
	}
	else 
	{
		if (m_IamFlgKeeper){
			m_pAudio->StopCue("Lift",false,this);//リフトAudio
			m_IsPlayCue = false;
			m_IamFlgKeeper = false;
		}
		if (m_transform.position.y < StartPos.y) {
			m_transform.position.y += 0.05f;
			m_moveSpeed = 0.0f;
		}
	}

	m_IsHitPlayer = false;

	CGameObject::Update();
}


void FallingFloor::Draw()
{
	CGameObject::Draw();
}

void FallingFloor::OnCollisionStay(CCollisionInterface* pCollision) {
	if (pCollision->GetCollisionType() == Collision::Type::Player) {
		// 当たったものがプレイヤーだった。
		if (m_CollisionObject->GetIsCollisionY()) {
			// Y方向の判定で衝突した。
			m_IsHitPlayer = true;
		}
	}
}

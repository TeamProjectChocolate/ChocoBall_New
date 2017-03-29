#include "stdafx.h"
#include "UpFloor.h"
#include "ShadowRender.h"

bool CUpFloor::m_IsPlayCue = false;

void CUpFloor::Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scale)
{
	UseModel<C3DImage>();
	SINSTANCE(CShadowRender)->Entry(this);

	m_pModel->SetFileName("image/down_block.x");
	CGameObject::Initialize();

	m_transform.position = pos;
	StartPos = m_transform.position;
	m_transform.scale = scale;
	m_transform.angle = rot;

	if (scale.z != 1.0f){
		ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.79f * scale.x *0.5f, 0.5f *  scale.y * 0.5f, 1.79f * scale.z * 0.5f)), Collision::Type::Floor,Collision::FilterGroup::Gimmick ,false, 0.0f, true,true);
	}
	else{
		ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.5f * scale.x *0.5f, 0.3f *  scale.y * 0.5f, 1.0f * scale.z * 0.5f)), Collision::Type::Floor,Collision::FilterGroup::Gimmick, false, 0.0f, true,true);
	}
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Player);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Enemy);

	m_player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));

	SetAlive(true);
	m_IamFlgKeeper = false;
}


void CUpFloor::Update()
{

	D3DXVECTOR3 PlayerPos = m_player->GetPos();

	if (m_IsHitPlayer)
	{
		if (m_MaxMove == -1 || m_transform.position.y < StartPos.y + m_MaxMove){
			if (!m_IsPlayCue){
				m_pAudio->PlayCue("Lift", false,this);//リフトAudio
				m_IsPlayCue = true;
				m_IamFlgKeeper = true;
			}
			m_transform.position.y += 0.1f;
			PlayerPos.y += 0.1f;
			m_player->SetPos(PlayerPos);
		}
		else{
			if (m_IamFlgKeeper){
				m_pAudio->StopCue("Lift",false,this);//リフトAudio
				m_IsPlayCue = false;
				m_IamFlgKeeper = false;
			}
		}

		m_IsHitPlayer = false;
	}
	else if (m_transform.position.y > StartPos.y)
	{
		if (m_IamFlgKeeper){
			m_pAudio->StopCue("Lift",false,this);
			m_IsPlayCue = false;
			m_IamFlgKeeper = false;
		}
		m_transform.position.y -= 0.05f;
	}

	CGameObject::Update();
}


void CUpFloor::Draw()
{
	CGameObject::Draw();
}


void CUpFloor::OnCollisionStay(CCollisionInterface* pCollision) {
	if (pCollision->GetCollisionType() == Collision::Type::Player) {
		// 当たったものがプレイヤーだった。
		if (m_CollisionObject->GetIsCollisionY()) {
			// Y方向の判定で衝突した。
			m_IsHitPlayer = true;
		}
	}
}

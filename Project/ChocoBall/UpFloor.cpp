#include "stdafx.h"
#include "UpFloor.h"


bool CUpFloor::m_IsPlayCue = false;

void CUpFloor::Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scale)
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/down_block.x");
	CGameObject::Initialize();

	m_transform.position = pos;
	StartPos = m_transform.position;
	m_transform.scale = scale;
	m_transform.angle = rot;

	if (scale.z != 1.0f){
		ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.5f * scale.x *0.5f, 0.3f *  scale.y * 0.5f, 1.75f * scale.z * 0.5f)), CollisionType::Floor, false, 0.0f, true,true);
	}
	else{
		ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.5f * scale.x *0.5f, 0.3f *  scale.y * 0.5f, 1.0f * scale.z * 0.5f)), CollisionType::Floor, false, 0.0f, true,true);
	}
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(CollisionType::Chocoball);
	m_CollisionObject->BitMask_On(CollisionType::Player);
	m_CollisionObject->BitMask_On(CollisionType::Enemy);

	m_player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));

	SetAlive(true);
	m_IamFlgKeeper = false;
}


void CUpFloor::Update()
{

	D3DXVECTOR3 PlayerPos = m_player->GetPos();

	if (IsHitPlayer(m_transform.position, 1.0f))
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


bool CUpFloor::IsHitPlayer(D3DXVECTOR3 pos, float radius)
{
	m_WorldMatrix = m_pModel->m_World;
	D3DXMatrixInverse(&m_InvWorld, nullptr, &m_WorldMatrix);


	PlayerPos = m_player->GetPos();

	D3DXVec3Transform(&dimension, &PlayerPos, &m_InvWorld);

	if (fabsf(dimension.x) < 1.5f && fabsf(dimension.z) < 1.5f && dimension.y <= 1.7f && dimension.y >= 0.6f)
	{
		return true;
	}

	return false;
}
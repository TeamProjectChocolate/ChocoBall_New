#include "stdafx.h"
#include "MoveFloor.h"


bool MoveFloor::m_IsPlayCue = false;

MoveFloor::MoveFloor()
{
	m_player = NULL;

}

MoveFloor::~MoveFloor()
{
	m_pAudio->StopCue("Lift", false, this);
}

void MoveFloor::Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scale)
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/down_block.x");
	CGameObject::Initialize();
	//D3DXMatrixInverse()
	m_transform.position = pos;//D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	StartPos = m_transform.position;
	SetRotation(D3DXVECTOR3(0, 0, 0), 0.1f);
	m_transform.scale = scale;// D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_transform.angle = rot;
	m_state = move_flont;
	m_MoveSpeed = D3DXVECTOR3(0.0f,0.0f,0.05f);

	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.5f * scale.x *0.5f, 0.3f *  scale.y * 0.5f, 1.5f * scale.z * 0.5f)), CollisionType::Floor, false, 0.0f, true,true);
	m_CollisionObject->BitMask_AllOn();
	m_CollisionObject->BitMask_Off(CollisionType::Chocoball);
	m_CollisionObject->BitMask_Off(CollisionType::Player);

	m_player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));

	//m_RigidBody->setActivationState(DISABLE_DEACTIVATION);

	SetAlive(true);

	//SetAlpha(1.0f);
	//m_IsIntersect.CollisitionInitialize(&m_transform.position, m_radius);
}


void MoveFloor::Update()
{
	float length = D3DXVec3Length(&(m_transform.position - StartPos));
	D3DXVECTOR3 PlayerPos = m_player->GetPos();
	if (MaxMove == 0.0f){
		m_state = move_none;
	}
	else if (length > MaxMove )
	{
		m_state = move_back;
	}
	else if (length <= 0.01f && m_state == move_back)
	{
		m_state = move_flont;
	}

	switch (m_state){
	case move_none:
		if (m_IamFlgKeeper){
			m_pAudio->StopCue("Lift",true,this);//リフトAudio
			m_IsPlayCue = false;
			m_IamFlgKeeper = false;
		}
		break;
	case move_flont:
		m_transform.position += m_dir * m_MoveSpeed.z;
		break;
	case move_back:
		m_transform.position -= m_dir * m_MoveSpeed.z;
		break;
	}
	

	CGameObject::Update();

	if (IsHitPlayer(m_transform.position, 1.0f))
	{
		m_player->SetParent(this);
		if (m_state != move_none){
			if (!m_IsPlayCue){
				m_pAudio->PlayCue("Lift", false ,this);//リフトAudio
				m_IsPlayCue = true;
				m_IamFlgKeeper = true;
			}
		}
	}
	else if (!IsHitPlayer(m_transform.position, 1.0f))
	{
		if (m_player->GetParent() == this){
			m_player->SetParent(nullptr);
		}
		if (m_IamFlgKeeper){
			m_pAudio->StopCue("Lift",true,this);
			m_IsPlayCue = false;
			m_IamFlgKeeper = false;
		}
	}


}

void MoveFloor::Draw()
{
	//SetUpTechnique();
	CGameObject::Draw();
}

bool MoveFloor::IsHitPlayer(D3DXVECTOR3 pos, float radius)
{
	m_WorldMatrix = m_pModel->m_World;
	D3DXMatrixInverse(&m_InvWorld, nullptr, &m_WorldMatrix);


	PlayerPos = m_player->GetPos();

	D3DXVec3Transform(&dimension, &PlayerPos, &m_InvWorld);

	if (fabsf(dimension.x) < 1.5f * m_transform.scale.x && fabsf(dimension.z) < 1.5f * m_transform.scale.z && dimension.y <= 1.7f && dimension.y >= 0.6f)
	{
		return true;
	}

	return false;
}

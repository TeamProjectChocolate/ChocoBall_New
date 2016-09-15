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
		this->Build(D3DXVECTOR3(1.5f * scale.x, 0.3f * scale.y, 1.75f * scale.z), m_transform.position);
	}
	else{
		this->Build(D3DXVECTOR3(1.5f * scale.x, 0.3f * scale.y, 1.0f * scale.z), m_transform.position);
	}

	m_player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));

	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	SetAlive(true);
	m_IamFlgKeeper = false;
}


void CUpFloor::Update()
{

	btTransform& trans = m_rigidBody->getWorldTransform();
	trans.setOrigin(btVector3(m_transform.position.x, m_transform.position.y, m_transform.position.z));

	D3DXVECTOR3 PlayerPos = m_player->GetPos();

	CGameObject::Update();
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
}


void CUpFloor::Draw()
{
	CGameObject::Draw();
}


void CUpFloor::Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos){
	//この引数に渡すのはボックスhalfsizeなので、0.5倍する。
	m_collisionShape = new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	groundTransform.setRotation(btQuaternion(m_transform.angle.x, m_transform.angle.y, m_transform.angle.z, m_transform.angle.w));
	float mass = 0.0f;

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_myMotionState, m_collisionShape, btVector3(0, 0, 0));
	m_rigidBody = new btRigidBody(rbInfo);
	//m_rigidBody->setUserIndex(1);
	//ワールドに追加。
	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->AddRigidBody(m_rigidBody);
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
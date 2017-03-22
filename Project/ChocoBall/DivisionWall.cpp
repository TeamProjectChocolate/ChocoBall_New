#include "stdafx.h"
#include "DivisionWall.h"
#include "CollisionType.h"
#include "ObjectManager.h"

CDivisionWall::CDivisionWall()
{
}

CDivisionWall::~CDivisionWall()
{
}

void CDivisionWall::Build(const D3DXVECTOR3& pos, const D3DXQUATERNION& Rotation) {
	m_transform.position = pos;
	m_transform.angle = Rotation;
	m_transform.scale = D3DXVECTOR3(5.0f, 6.0f, 1.0f);

	// 剛体生成。
	{
		D3DXVECTOR3 size = D3DXVECTOR3(5.0f,6.0f,1.0f);
		//この引数に渡すのはボックスhalfsizeなので、0.5倍する。
		ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f)), CollisionType::Wall, false, 0.0f, true,true);
		m_CollisionObject->BitMask_AllOn();
		m_CollisionObject->BitMask_Off(CollisionType::Wall);
	}
}

void CDivisionWall::Initialize() {
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/BR.x");
	CGameObject::Initialize();
	m_pModel->m_alpha = 0.7f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::GLASS];
	m_moveSpeed = 0.5f;

	SetAlive(true);
}

void CDivisionWall::Update() {
	if (m_IsMove) {
		D3DXMATRIX Rota = m_pModel->GetRotation();
		D3DXVECTOR3 Local_X;
		Local_X.x = Rota.m[0][0];
		Local_X.y = Rota.m[0][1];
		Local_X.z = Rota.m[0][2];
		m_transform.position += Local_X * m_moveSpeed;
		m_Length += m_moveSpeed;
		if (m_Length > m_transform.scale.x) {
			// オブジェクトの横のサイズ分スライドしたら。
			m_IsMove = false;
		}
	}
	CGameObject::Update();
}

void CDivisionWall::Draw() {
	CGameObject::Draw();
}
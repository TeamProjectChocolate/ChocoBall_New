#include "stdafx.h"
#include "Block.h"
#include "ShadowRender.h"
#include "BulletPhysics.h"
#include "RenderContext.h"
#include "Infomation.h"
#include "Player.h"

void CBlock::OnDestroy(){
	m_CollisionObject->RemoveWorld();
	//子供に死亡したことを通知。
	if (m_child){
		m_child->OnDestroyParent();
	}
	m_isDead = true;
}

//親が死んだときに呼ばれる処理。
void CBlock::OnDestroyParent()
{
	m_eState = enState_Fall;
	m_fallPosY = m_parent->GetPos().y;
	SetParent(m_parent->m_parent);
	//自分の子供を落下させる。
	CBlock* child = m_child;
	while (child != NULL){
		child->m_fallPosY = child->m_parent->GetPos().y;
		child->m_eState = enState_Fall;
		child = child->m_child;
	}
}


void CBlock::Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot)
{
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/BR.x");
	CGameObject::Initialize();
#ifdef NOT_INSTANCING
	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::CHOCOLATE];
#endif
	m_transform.position = pos;
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_transform.angle = rot;

	float mass = 0.0f;
	//この引数に渡すのはボックスhalfsizeなので、0.5倍する。
	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.0f*0.5f, 1.0f*0.5f, 1.0f*0.5f)/*btVector3(1.0f, 1.0f, 1.0f)*/), Collision::Type::Wall,Collision::FilterGroup::Gimmick, false, mass, true,true);
	m_CollisionObject->BitMask_AllOff();
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Player);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Chocoball);
	m_CollisionObject->BitMask_On(Collision::FilterGroup::Enemy);

	SetAlive(true);

#ifdef NOT_INSTANCING
	SINSTANCE(CShadowRender)->Entry(this);
#endif
}


void CBlock::Update()
{
	switch (m_eState){
	case enState_Normal:
		break;
	case enState_Broken:
		break;
	case enState_Fall:
		m_transform.position.y -= 0.1f;
		//m_RigidBody->activate();
		if (m_transform.position.y <= m_fallPosY){
			m_transform.position.y = m_fallPosY;
			m_eState = enState_Normal;
		}
		break;
	}

	if (m_IsThrow) {
		// 指定された方向に進む。
		m_transform.position += m_Velocity;
		CollisionPlayer();
	}
	
	CGameObject::Update();
}

void CBlock::CollisionPlayer() {
	SweepResult_XZ callback(this,true);

	D3DXVECTOR3 StartPos = m_CollisionObject->GetPos();
	D3DXVECTOR3 EndPos = StartPos + m_Velocity;
	D3DXQUATERNION Rotation = m_CollisionObject->GetRotation();

	btTransform Start, End;
	Start = btTransform(btQuaternion(Rotation.x, Rotation.y, Rotation.z, Rotation.w), btVector3(StartPos.x, StartPos.y, StartPos.z));
	End = btTransform(btQuaternion(Rotation.x, Rotation.y, Rotation.z, Rotation.w), btVector3(EndPos.x, EndPos.y, EndPos.z));

	SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"))->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()),Start,End,callback);
	
	if (callback.isHit) {
		if (callback.hitCollisionType != Collision::Type::Player) {
			// プレイヤー以外無視。
			return;
		}
		// 衝突。
		OutputDebugString("当たったよ。\n");

		// 自分からプレイヤーへの向きベクトル算出。
		D3DXVECTOR3 ThisToPlayer;
		//ThisToPlayer = callback.hitPos - m_CollisionObject->GetPos();
		ThisToPlayer = m_Player->GetCollision()->GetPos() - callback.hitPos;
		D3DXVec3Normalize(&ThisToPlayer, &ThisToPlayer);
		D3DXVECTOR3 addDir;
		D3DXVec3Normalize(&addDir, &m_Velocity);

		float rad = fabsf((acosf(D3DXVec3Dot(&addDir, &ThisToPlayer))));

		if (rad >= D3DXToRadian(30.0f)) {
			return;
		}

		//D3DXVECTOR3 addPosXZ = addPos;
		//addPosXZ.y = 0.0f;

		//D3DXVECTOR3 t;
		//t.x = -addPos.x;
		//t.y = 0.0f;
		//t.z = -addPos.z;
		//D3DXVec3Normalize(&t, &t);
		////D3DXVec3Normalize(&t, &addPos);
		////半径分押し戻す。
		//float radius = m_CollisionObject->GetCollisionShape()->getLocalScaling().getX();
		//t *= radius;
		//addPos += t;
		////続いて壁に沿って滑らせる。
		////滑らせる方向を計算。
		//D3DXVec3Cross(&t, &callback.hitNormalXZ, &Vector3::Up);
		//D3DXVec3Normalize(&t, &t);
		////D3DXVec3Normalize(&t, &addPos);
		//t *= D3DXVec3Dot(&t, &addPosXZ);
		//addPos += t;	//滑らせるベクトルを加算。

		// isIntersectクラスでデルタタイムにより除算されるため、60掛けてから渡す。
		// ブロック進行方向算出。
		//D3DXVECTOR3 Dir;
		//D3DXVec3Normalize(&Dir,&m_Velocity);
		//// このブロックの力に対して真っ向から抵抗している力を算出。
		//float Power = D3DXVec3Dot(&-Dir, &m_Player->GetMoveSpeed());

		m_Player->SetRepulsion(/*addPos*/m_Velocity * 60.0f);
	}
}

void CBlock::Draw()
{
	m_pRender->SetModelData(m_pModel);
	m_pRender->Draw();
}

void CBlock::BeginDraw()
{
	SetUpTechnique();
	UINT numPass;
	m_pRender->GetEffect()->Begin(&numPass/*テクニック内に定義されているパスの数が返却される*/, 0);
	m_pRender->GetEffect()->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	// 現在のプロジェクション行列とビュー行列をシェーダーに転送
	SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pRender->GetEffect());
	SINSTANCE(CRenderContext)->GetCurrentLight()->SetLight(m_pRender->GetEffect());
	// 視点をシェーダーに転送
	m_pRender->GetEffect()->SetVector("EyePosition", reinterpret_cast<D3DXVECTOR4*>(&SINSTANCE(CRenderContext)->GetCurrentCamera()->GetPos()));

	SINSTANCE(CShadowRender)->SetShadowCamera(m_pRender->GetEffect());
}
void CBlock::EndDraw()
{
	m_pRender->GetEffect()->EndPass();
	m_pRender->GetEffect()->End();
}

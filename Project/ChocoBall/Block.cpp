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
	ActivateCollision(D3DXVECTOR3(0.0f, 0.0f, 0.0f), new btBoxShape(btVector3(1.0f*0.5f, 1.0f*0.5f, 1.0f*0.5f)), Collision::Type::Wall,Collision::FilterGroup::Gimmick, false, mass, true,true);
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
	case enState_Crumble:
		int randPtern = 8;
		int rnd_x = (rand() % randPtern) - (randPtern / 2);
		int rnd_y = (rand() % randPtern) - (randPtern / 2);
		int rnd_z = (rand() % randPtern) - (randPtern / 2);

		// 崩れ方をいい感じにばらけさせる(力の向きと大きさ)。
		float Power = 100.0f;
		float Volume = 0.1f;
		D3DXVECTOR3 Vel = m_Velocity;
		Vel.x += (Volume * static_cast<float>(rnd_x));
		Vel.y += (Volume * static_cast<float>(rnd_y));
		Vel.z += (Volume * static_cast<float>(rnd_z));
		static_cast<CRigidbody*>(m_CollisionObject.get())->ApplyForce(Vel * Power);

		// 崩れ方をいい感じにばらけさせる(回転のしやすさ)。
		float mass = 1.0f;
		D3DXVECTOR3 Inertia = Vector3::One;
		Inertia.x += (Volume * static_cast<float>(rnd_x));
		Inertia.y += (Volume * static_cast<float>(rnd_y));
		Inertia.z += (Volume * static_cast<float>(rnd_z));
		static_cast<CRigidbody*>(m_CollisionObject.get())->SetMassProps(mass, Inertia);

		// 死亡判定。
		this->DeathCount();
		break;
	}

	if (m_IsThrow) {
		// 指定された方向に進む。
		m_transform.position += m_Velocity;
		// 衝突判定。
		CollisionPlayer();

		if (m_ThrowCounter >= m_ThrowTime) {
			// 指定された時間を過ぎた。

			int randPtern = 8;
			int rnd = (rand() % randPtern) - (randPtern / 2);
			this->SentenceOfDeath(2.0f + (0.05f * static_cast<float>(rnd)));

			// 物理挙動オン。
			static_cast<CRigidbody*>(m_CollisionObject.get())->OnDynamic();

			// 崩すためにチョコ壁との衝突判定も開始。
			m_CollisionObject->BitMask_On(Collision::FilterGroup::Gimmick);

			// 親子の縁を切る。
			this->RemoveChild();
			this->RemoveParent();

			m_IsThrow = false;

			m_eState = EnState::enState_Crumble;
		}
		float deltaTime = 1.0f / 60.0f;
		m_ThrowCounter += deltaTime;
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

	m_pBulletPhysics->ConvexSweepTest_Dynamic(static_cast<btConvexShape*>(m_CollisionObject->GetCollisionShape()),Start,End,callback);
	
	if (callback.isHit) {
		// 衝突。
		if (callback.hitCollisionType != Collision::Type::Player) {
			// プレイヤー以外無視。
			return;
		}

		// ブロックの淵にコリジョンが引っ掛かる挙動への対策。
		{
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
		}

		// IsIntersectクラスでdeltaTimeにより除算されるため、60掛ける。
		m_Player->SetRepulsion(m_Velocity * 60.0f);
	}
}

void CBlock::DeathCount() {
	if (m_DeathCounter >= m_DeathTime) {
		// 指定された時間が過ぎたので自分を殺す。
		SetAlive(false);
	}
	m_DeathCounter += 1.0f / 60.0f;
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

void CBlock::Throw(const D3DXVECTOR3& dir, float Power) {
	m_CollisionObject->SetCollisionType(Collision::Type::AttackWall);
	m_IsThrow = true;
	this->SetVelocity(dir * Power);
	m_ThrowTime = 3.0f;	// チョコ壁が飛ぶ時間を設定。
}

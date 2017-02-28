#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "Rigidbody.h"
#include "islntersect.h"
#include "ObjectManager.h"
#include "C3DObjectRender_S.h"


class CBlock : public CGameObject{
public:
	CBlock(){ 
		SetLife(true); 
		m_parent = NULL;
		m_child = NULL;
		m_isDead = false;
		m_eState = enState_Normal;
		m_fallPosY = 0.0f;
	}
	void OnDestroy()override;
	void Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot);
	void Update();
	void Draw();
	void BeginDraw();
	void EndDraw();

	void SetUpTechnique()override{
#ifdef NOT_EM
		m_pRender->SetUpTechnique("Boneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("Boneless_Tex_Fresnel");
#endif
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Lim");
	}

	void SetRenderState()override{
		m_RenderingState = RENDER_STATE::_3D_Simple;
	}

	void Setradius(float radius)
	{
		m_radius = radius;
	}
	void Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos);
	void SetLife(bool life){
		m_life = life;
	}
	bool GetLife(){
		return m_life;
	}
	//親が死んだときに呼ばれる処理。
	void OnDestroyParent();

	//親を設定。
	void SetParent(CBlock* par)
	{
		if (par){
			m_parent = par;
			par->SetChild(this);
		}
		else{
			m_parent = NULL;
		}
	}
	bool IsDead()
	{
		return m_isDead;
	}
private:
	//子供を設定。
	void SetChild(CBlock* child)
	{
		m_child = child;
	}
private:
	CIsIntersect	m_IsIntersect;
	enum EnState{
		enState_Normal,	//通常状態。
		enState_Broken,	//壊れた。
		enState_Fall,	//落下中。
	};
	CBlock*			m_parent;		//親ブロック
	CBlock*			m_child;		//子供
	D3DXVECTOR3		m_moveSpeed;	//落下速度
	float			m_radius;
	//ここからbulletPhysicsの剛体を使用するために必要な変数。
	btRigidBody*		m_rigidBody;		//剛体。
	btCollisionShape*	m_collisionShape;	//コリジョンの形状。
	btDefaultMotionState* m_myMotionState;
	bool m_life;
	EnState				m_eState;	//状態。
	float				m_fallPosY;	//落下位置。
	bool m_isDead;
	D3DXHANDLE m_hShaderTecnique;	//シェーダーテクニックのハンドル。
};
#pragma once
#include "stdafx.h"
#include "Rigidbody.h"
#include "islntersect.h"
#include "RenderContext.h"
#include "C3DObjectRender_S.h"

class CChocoBall : public CGameObject {
public:
	CChocoBall(){}
	~CChocoBall(){}

	void Initialize(D3DXVECTOR3 Spos, D3DXVECTOR3 Epos);
	void Update()override;
	void Draw()override;

	void SetRenderState()override{

	}

	void OnDestroy();

	//現時点と目的地を渡すとベクトルをセットします。
	//GetVector()で呼び出してください。
	void SetVector(D3DXVECTOR3 Spos, D3DXVECTOR3 Epos){
		m_Vector = Epos - Spos;
	}

	//進行方向のベクトルを返す
	D3DXVECTOR3 GetVector()
	{
		return m_Vector;
	}
private:
	//ここからbulletPhysicsの剛体を使用するために必要な変数。
	btCollisionShape*		m_collisionShape;	//コリジョンの形状。
	D3DXVECTOR3				m_EndPosition;		//目的地
	D3DXVECTOR3				m_Vector;			//進行方向を格納。
	D3DXVECTOR3				m_Vector2;
	btDefaultMotionState*	m_myMotionState;
	float					m_life;
	CRigidbody				m_Rigidbody;
	CIsIntersect			m_IsIntersect;
	float					m_radius;
	D3DXVECTOR3				m_moveSpeed;		//移動速度。
	
};
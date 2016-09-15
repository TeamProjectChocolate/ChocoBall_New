#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Audio.h"


//::落ちる床のクラス::
class FallingFloor : public CGameObject{
public:
	FallingFloor(){}
	~FallingFloor(){
		m_pAudio->StopCue("Lift", false,this);
	}
	void Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scale);
	void Update()override;
	void Draw()override;
	virtual void SetUpTechnique()override{
		m_pRender->SetUpTechnique("NotNormalMapNonAnimationFresnelShadowTec");
	}
	void Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos);
	void SetPos(D3DXVECTOR3 pos){
		position = pos;
	}
	const D3DXVECTOR3& GetPos()override{
		return position;
	}
	void SetMaxMove(float max){
		m_MaxMove = max;
	}
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
	//プレイヤーとの当たり判定
	bool IsHitPlayer(D3DXVECTOR3 pos, float radius);


private:
	D3DXVECTOR4			dimension;
	D3DXVECTOR3			position;
	D3DXVECTOR3			StartPos;
	D3DXVECTOR3			PlayerPos;
	D3DXMATRIX			m_WorldMatrix;
	D3DXMATRIX			m_InvWorld;
	btRigidBody*		m_rigidBody;		//剛体。
	btCollisionShape*	m_collisionShape;	//コリジョンの形状。
	btDefaultMotionState* m_myMotionState;
	CPlayer*			m_player;
	float m_MaxMove;
	CAudio* m_pAudio;
	bool m_IamFlgKeeper;
	static bool m_IsPlayCue;
};
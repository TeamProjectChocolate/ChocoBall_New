#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "Player.h"
#include "Audio.h"


class MoveFloor : public CGameObject{
public:
	MoveFloor();
	~MoveFloor();
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

	void SetMaxMove(float maxmove){
		MaxMove = maxmove;
	}

	//プレイヤーとの当たり判定
	bool IsHitPlayer(D3DXVECTOR3 pos, float radius);

	D3DXVECTOR3 GetMoveSpeed(){
		return m_MoveSpeed;
	}
	void SetDirection(D3DXVECTOR3 dir){
		D3DXVec3Normalize(&m_dir, &dir);
	}

	D3DXVECTOR3 GetDirection(){
		return m_dir;
	}
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
private:
	D3DXVECTOR4			dimension;
	D3DXVECTOR3			position;
	D3DXMATRIX			m_WorldMatrix;
	D3DXMATRIX			m_InvWorld;
	CPlayer*			m_player;
	btRigidBody*		m_rigidBody;
	btCollisionShape*	m_collisionShape;	//コリジョンの形状。
	btDefaultMotionState* m_myMotionState;
	D3DXVECTOR3			StartPos;
	D3DXVECTOR3			PlayerPos;
	D3DXVECTOR3			m_MoveSpeed;
	float				MaxMove;
	int m_state;
	enum state{
		move_flont,
		move_back,
		move_none
	};
	D3DXVECTOR3 m_dir;
	CAudio* m_pAudio;
	bool m_IamFlgKeeper;
	static bool m_IsPlayCue;
};
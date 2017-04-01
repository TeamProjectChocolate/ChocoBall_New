#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Audio.h"
#include "Infomation.h"

//::—Ž‚¿‚é°‚ÌƒNƒ‰ƒX::
class FallingFloor : public CGameObject{
public:
	FallingFloor(){}
	~FallingFloor(){
		m_pAudio->StopCue("Lift", false,this);
	}
	void Initialize(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scale);
	void Update()override;
	void Draw()override;

	void OnCollisionStay(CCollisionInterface* pCollision)override;

	void SetUpTechnique()override{
#ifdef NOT_VSM
		m_pRender->SetUpTechnique("Boneless_Tex_Shadow");
#else
		m_pRender->SetUpTechnique("Boneless_Tex_Shadow_VSM");
#endif
	}
	void EM_SetUpTechnique()override{
#ifdef NOT_VSM
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Shadow");
#else
		m_pEMSamplingRender->SetUpTechnique("Boneless_Tex_Shadow_VSM");
#endif
	}

	void Build(const D3DXVECTOR3& size, const D3DXVECTOR3& pos);
	void SetPos(D3DXVECTOR3 pos){
		position = pos;
	}
	const D3DXVECTOR3& GetPos()const override{
		return position;
	}
	void SetMaxMove(float max){
		m_MaxMove = max;
	}
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}


private:
	D3DXVECTOR4			dimension;
	D3DXVECTOR3			position;
	D3DXVECTOR3			StartPos;
	D3DXVECTOR3			PlayerPos;
	D3DXMATRIX			m_WorldMatrix;
	D3DXMATRIX			m_InvWorld;
	CPlayer*			m_player;
	float m_MaxMove;
	float m_MaxSpeed;
	CAudio* m_pAudio;
	bool m_IamFlgKeeper;
	static bool m_IsPlayCue;
	float m_moveSpeed;

	bool m_IsHitPlayer = false;
};
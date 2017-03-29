#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Audio.h"
#include "Infomation.h"


//::落ちる床のクラス::
class CUpFloor : public CGameObject{
public:
	CUpFloor(){}
	~CUpFloor(){};
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

	const D3DXVECTOR3& GetPos()override{
		return position;
	}

	//プレイヤーとの当たり判定
	bool IsHitPlayer(D3DXVECTOR3 pos, float radius);

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
	CPlayer*			m_player = nullptr;
	float m_MaxMove;			// 移動限界量
	CAudio* m_pAudio = nullptr;
	bool m_IamFlgKeeper;
	static bool m_IsPlayCue;

	bool m_IsHitPlayer = false;
};
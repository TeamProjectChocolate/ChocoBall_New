#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "ObjectManager.h"
#include "Chocoball.h"
#include "GameObject.h"
#include "Player.h"
#include "CourceDef.h"
#include "ShadowRender.h"
#include "Enemy_Boss.h"


#define CHOCO_NUM 200	//一回に流れるチョコの数

struct SInstancingVertex{
	FLOAT x, y, z, w;
	DWORD color;
	FLOAT u, v;
	int Index;
public:
	void SetIndex(const int& index){
		this->Index = index;
	}
};
//Chocoballを管理するクラス
class CCBManager : public CGameObject{
public:
	CCBManager(){
		m_CBManagerNum++;
		strcpy(m_pRenderName, "Choco");
	};
	~CCBManager(){
#ifdef NOT_INSTANCING
#else
		for (auto choco : m_Choco) {
			// 存在するチョコボールを削除。
			SINSTANCE(CObjectManager)->DeleteGameObject(choco);
		}
		m_Choco.clear();

		m_CBManagerNum--;
#endif
	};
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawShadow(CCamera*)override;
	void Draw_EM(CCamera*)override;
	void OnTriggerStay(CCollisionInterface* pCollision)override;

	void SetRenderState()override{
		m_RenderingState = RENDER::TYPE::Instancing;
	}
	void SetShadowRenderState()override{
		m_ShadowRenderingState = RENDER::TYPE::_3D_ShadowSample_I;
	}
	void EM_SetRenderState()override{
		m_EMRenderingState = RENDER::TYPE::EM_Sampling_I;
	}

	void ActivateShadowRender()override;

	void SetUpTechnique()override{
#ifdef NOT_EM
		m_pRender->SetUpTechnique("IBoneless_Tex_Lim");
#else
		m_pRender->SetUpTechnique("IBoneless_Tex_Fresnel");
#endif
	}
	void EM_SetUpTechnique()override{
		m_pEMSamplingRender->SetUpTechnique("IBoneless_Tex_Lim");
	}

	void FindCource();
	void NonActivate();

	//最初のポジションを取得できます。
	D3DXVECTOR3 GetStartPosition()
	{
		return m_pos;
	}


	//最初のポジションをセットします。
	void SetStartPosition(D3DXVECTOR3 pos)
	{
		m_pos = pos;
	}

	//終点のポジションをセットします。
	void SetEndPosition(D3DXVECTOR3 Epos)
	{
		m_posG = Epos;
	}

	//終点のポジションを取得できます。
	D3DXVECTOR3 GetEndPosition()
	{
		return m_posG;
	}
	//チョコボールとの衝突判定。
	void IsHit();
	
	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
	int GetCourceNo(){
		return m_InitPosOfCourceNo;
	}

	void SetPintoWorld(const D3DXMATRIX& mat)override{
#ifdef NOT_DOF
		for (short idx = 0; idx < CHOCO_NUM; idx++) {
			m_Choco[idx]->SetPintoWorld(mat);
		}
#else
		m_pModel->SetPintoWorld(mat);
#endif
	}
	void SetPintoPos(const D3DXVECTOR3& pos)override{
#ifdef NOT_DOF
		for (short idx = 0; idx < CHOCO_NUM; idx++) {
			m_Choco[idx]->SetPintoPos(pos);
		}
#else
		m_pModel->SetPintoPos(pos);
#endif
	}

	inline void SetIsBossDamage(bool flg) {
		m_IsBossDamage = flg;
	}

	// チョコボールを時間差で破裂させる。
	void OnBurst(float);
private:
	void CreateChocoBall();
private:
	int m_numCreate = 0;	// チョコボール生成数。
	D3DXVECTOR3			m_pos;			//生成される場所のポジション。
	D3DXVECTOR3			m_posG;			//流れていく先(ゴール)のポジション。
	vector<CChocoBall*>	m_Choco;
	float				m_interval;		//インターバル。
	float				m_timer;		//タイマー。
	int					m_InitPosOfCourceNo;// チョコボールが生成された場所のコースナンバー
	CCourceDef m_CourceDef;
	STAGE_ID m_StageID;
	bool m_IsFirst;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	static int m_CBManagerNum;

	CPlayer* m_pPlayer = nullptr;
	CEnemy_Boss* m_pBoss = nullptr;
	bool m_IsBossDamage = true;	// ボスにダメージを与えられるかのフラグ。

	bool m_IsBurst;	// チョコボールを爆散させるか。
	float m_CBDeathTime;	// チョコボールが死ぬ時間。
 };
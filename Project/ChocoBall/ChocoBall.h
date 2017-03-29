#pragma once
#include "stdafx.h"
#include "Rigidbody.h"
#include "islntersect.h"
#include "RenderContext.h"
#include "C3DObjectRender_S.h"
#include "ParticleEmitter.h"

class CChocoBall : public CGameObject {
public:
	CChocoBall(){}
	~CChocoBall(){
	}

	void Initialize(const D3DXVECTOR3& Spos, const D3DXVECTOR3& Epos);
	void Update()override;
	void Draw()override;

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
		m_RenderingState = RENDER::TYPE::_3D_Simple;
	}

	void BeginDraw();
	void EndDraw();
	void OnDestroy();

	//現時点と目的地を渡すとベクトルをセットします。
	//GetVector()で呼び出してください。
	inline void SetVector(const D3DXVECTOR3& Spos, const D3DXVECTOR3& Epos){
		m_Vector = Epos - Spos;
	}

	//進行方向のベクトルを返す
	inline const D3DXVECTOR3& GetVector()
	{
		return m_Vector;
	}
	inline void SetStageID(STAGE_ID id) {
		m_StageID = id;
	}
	inline void SetIsBossDamage(bool flg) {
		m_IsBossDamage = flg;
	}
	inline bool GetIsBossDamage()const {
		return m_IsBossDamage;
	}
	inline void SetDeathTime(float t) {
		m_DeathTime = t;
		m_TimeCounter = 0.0f;
	}
	inline void SetIsBurst(bool flg) {
		m_IsBurst = flg;
	}
private:
	//ここからbulletPhysicsの剛体を使用するために必要な変数。
	D3DXVECTOR3				m_EndPosition;		//目的地
	D3DXVECTOR3				m_Vector;			//進行方向を格納。
	D3DXVECTOR3				m_Vector2;
	float					m_life;
	float					m_radius;
	D3DXVECTOR3				m_moveSpeed;		//移動速度。

	bool m_IsBossDamage = true;	// ボスにダメージを与えられるか。

	// チョコボールを時間差で破裂。
	float m_DeathTime;
	float m_TimeCounter;
	bool m_IsBurst = false;
	STAGE_ID m_StageID;
	CParticleEmitter* m_pEmitter = nullptr;
};
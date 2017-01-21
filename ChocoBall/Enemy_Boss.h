#pragma once
#include "EnemyBase.h"
class CEnemy_Boss :
	public EnemyBase
{
public:
	CEnemy_Boss();
	~CEnemy_Boss();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitReaction()override;
	void ConfigLight()override;
	void SetUpTechnique()override {
		m_pRender->SetUpTechnique("Skin_Tex");
	}
	void EM_SetUpTechnique()override {
		m_pEMSamplingRender->SetUpTechnique("Skin_Tex_Lim");
	}

private:
	short m_HP;	// ヒットポイント。
	CIsIntersect m_IsIntersect;		//CIsIntersectのインスタンス
};


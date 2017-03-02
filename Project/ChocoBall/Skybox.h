#pragma once
#include "GameObject.h"
#include "Light.h"


class Skybox : public CGameObject{
public:
	Skybox();
	~Skybox();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Draw_EM(CCamera*)override;
	void SetUpTechnique()override
	{
		m_pRender->SetUpTechnique("Bonelsee_Tex_Bloom");
	}
	void EM_SetUpTechnique()override
	{
		m_pEMSamplingRender->SetUpTechnique("Bonelsee_Tex");
	}
private:

	CLight light;
};
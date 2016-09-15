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
	void SetUpTechnique()override
	{
		m_pRender->SetUpTechnique("NotNormalMapNonAnimationBloomTec");
	}
private:

	CLight light;
};
#pragma once
#include "Render.h"
class CShadowSamplingRender :
	public CRender
{
public:
	CShadowSamplingRender();
	~CShadowSamplingRender();
	void Initialize()override;
	void Draw()override;
	inline void SetShadowCamera(CCamera* camera)override{
		m_pShadowCamera = camera;
	}
private:
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer( LPD3DXMESHCONTAINER);
	void AnimationDraw(D3DXMESHCONTAINER_DERIVED*);
	void NonAnimationDraw();
private:
	CCamera* m_pShadowCamera;
};


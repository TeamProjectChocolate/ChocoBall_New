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
	void AnimationDraw(ANIMATION::D3DXMESHCONTAINER_DERIVED*);
	void NonAnimationDraw();
private:
	CCamera* m_pShadowCamera;
	float m_PlayerPos_Y;
	// 境界線使用。
	IDirect3DVertexDeclaration9* m_VertexDecl_Override = nullptr;
	LPDIRECT3DVERTEXBUFFER9 m_pHorizonBuffer = nullptr;	// 境界線格納用バッファー。
	bool m_IsFirst;
private:
	void DrawHorizon();
	void CopyBuffer();
	void ActivateHorizon();
};


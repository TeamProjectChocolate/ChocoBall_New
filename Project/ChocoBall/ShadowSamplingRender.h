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
	float m_PlayerPos_Y;
	// ���E���g�p�B
	IDirect3DVertexDeclaration9* m_VertexDecl_Override = nullptr;
	LPDIRECT3DVERTEXBUFFER9 m_pHorizonBuffer = nullptr;	// ���E���i�[�p�o�b�t�@�[�B
	bool m_IsFirst;
private:
	void DrawHorizon();
	void CopyBuffer();
	void ActivateHorizon();
};

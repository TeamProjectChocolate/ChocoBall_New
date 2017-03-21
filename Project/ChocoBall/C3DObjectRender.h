#pragma once
#include "Render.h"
#include "Infomation.h"

class C3DObjectRender :
	public CRender
{
public:
	C3DObjectRender();
	~C3DObjectRender();
	void Initialize()override;
	void Draw()override;
private:
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME);
	void AnimationDraw(ANIMATION::D3DXMESHCONTAINER_DERIVED*, ANIMATION::D3DXFRAME_DERIVED*);
	void NonAnimationDraw(ANIMATION::D3DXFRAME_DERIVED*);
private:
	bool m_UseBorn;
	// シェーダーに値をセットするためのハンドル群
	D3DXHANDLE m_hEyePosition;
	D3DXHANDLE m_hWorldMatrixArray;
	D3DXHANDLE m_hluminance;
	D3DXHANDLE m_hnumBone;
	D3DXHANDLE m_hAlpha;
	D3DXHANDLE m_hRota;
	D3DXHANDLE m_hWorld;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hShadowMap;
};


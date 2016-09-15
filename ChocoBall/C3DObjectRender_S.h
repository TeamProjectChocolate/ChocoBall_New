#pragma once
#include "Render.h"
#include "Infomation.h"

class C3DObjectRender_S :
	public CRender
{
public:
	C3DObjectRender_S();
	~C3DObjectRender_S();

	void Initialize()override;
	void Draw()override;
private:
	void DrawFrameSimple(LPD3DXFRAME pFrame);
	void DrawMeshContainerSimple(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void NonAnimationDrawSimple(D3DXFRAME_DERIVED*);

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


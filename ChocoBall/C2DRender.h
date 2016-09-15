#pragma once
#include "Render.h"
#include "Infomation.h"

#define FVF_CUSTOMVERTEX (D3DFVF_XYZW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct SVertex
{
	FLOAT x, y, z, w;
	DWORD color;
	FLOAT u, v;
};

class C2DRender :
	public CRender
{
public:
	C2DRender();
	~C2DRender();
	void Initialize()override;
	void Draw()override;
private:
	// シェーダーに値をセットするためのハンドル群
	D3DXHANDLE m_hWorld;
	D3DXHANDLE m_hSplit_X;
	D3DXHANDLE m_hSplit_Y;
	D3DXHANDLE m_hNowCol;
	D3DXHANDLE m_hNowRow;
	D3DXHANDLE m_hRatio_X;
	D3DXHANDLE m_hRatio_Y;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hAlpha;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
};


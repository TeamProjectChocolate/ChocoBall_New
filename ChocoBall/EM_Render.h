#pragma once
#include "Render.h"
#include "RenderTarget.h"
#include "Camera.h"

#define HEXA 6	// キューブマップは六面体

class CEM_Render :
	public CRender
{
public:
	CEM_Render();
	~CEM_Render();
	void Initialize()override;
	void Draw()override;
	inline void SetEnable(bool flg){
		m_isEnable = flg;
	}
	LPDIRECT3DCUBETEXTURE9 GetCubeTex(){
		return m_pCubeTex;
	}
private:
	CRenderTarget m_RenderTarget;
	LPDIRECT3DCUBETEXTURE9 m_pCubeTex;
	LPDIRECT3DSURFACE9 m_pZMap;
	LPDIRECT3DSURFACE9 m_pCubeSurfaces[HEXA];
	CCamera m_Cameras[HEXA];
	bool m_isEnable;
};

// 六面体それぞれの面を描画する際のカメラの向き。
static D3DXVECTOR3 lookAt[6] = {
	D3DXVECTOR3(1.0f, 0.0f, 0.0f), // +X
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), // -X
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Y
	D3DXVECTOR3(0.0f, -1.0f, 0.0f), // -Y
	D3DXVECTOR3(0.0f, 0.0f, 1.0f), // +Z
	D3DXVECTOR3(0.0f, 0.0f, -1.0f) // -Z
};
// 六面体のそれぞれの面を描画する際の上方向(上面と下面の描画は特に注意)。
static D3DXVECTOR3 up[6] = {
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +X(Up = +Y)
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -X(Up = +Y)
	D3DXVECTOR3(0.0f, 0.0f, -1.0f), // +Y(Up = -Z)
	D3DXVECTOR3(0.0f, 0.0f, 1.0f), // -X(Up = +Z)
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Z(Up = +Y)
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -Z(Up = +Y)
};

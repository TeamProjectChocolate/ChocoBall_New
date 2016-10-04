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
private:
	CRenderTarget m_RenderTarget;
	LPDIRECT3DCUBETEXTURE9 m_pCubeTex;
	LPDIRECT3DSURFACE9 m_pCubeSurfaces[HEXA];
	CCamera m_Cameras[HEXA];
	bool m_isEnable;
};


#pragma once
#include "Render.h"
class C3DRender_D :
	public CRender
{
public:
	C3DRender_D();
	~C3DRender_D();
	void Initialize()override;
	void Draw()override{};
	void Draw(const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&)override;
private:
	void DrawFrameDepth(LPD3DXFRAME, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void DrawMeshContainerDepth(LPD3DXMESHCONTAINER, LPD3DXFRAME, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void AnimationDrawDepth(D3DXMESHCONTAINER_DERIVED*, D3DXFRAME_DERIVED*, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void NonAnimationDrawDepth(D3DXFRAME_DERIVED*, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
};


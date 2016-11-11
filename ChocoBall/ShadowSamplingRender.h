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
	void CopyHorizon(const vector<float>& horizon) {
		for (float work : horizon) {
			m_Horizon.push_back(work);
		}
	}
	void SetIsHorizon(bool flg) {
		m_IsHorizon = flg;
	}
private:
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer( LPD3DXMESHCONTAINER);
	void AnimationDraw(D3DXMESHCONTAINER_DERIVED*);
	void NonAnimationDraw();
private:
	CCamera* m_pShadowCamera;
	float m_PlayerPos_Y;
	// ã´äEê¸égópÅB
	bool m_IsHorizon = false;
	vector<float> m_Horizon;
	IDirect3DVertexDeclaration9* m_VertexDecl_Override = nullptr;
	LPDIRECT3DVERTEXBUFFER9 m_pHorizonBuffer = nullptr;
	bool m_IsFirst;
private:
	void DrawHorizon();
	void CopyBuffer();
	void ActivateHorizon();
};


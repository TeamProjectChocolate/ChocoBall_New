#pragma once
#include "Render.h"
#include "Infomation.h"

class C3DObjectRender :
	public CRender
{
public:
	C3DObjectRender();
	~C3DObjectRender();
	void SetUpTechniqueAnimation(){
		m_pEffect->SetTechnique("NotNormalMapAnimationFresnelTec");
	}

	void Initialize()override;
	void Draw()override;
private:
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME);
	void AnimationDraw(D3DXMESHCONTAINER_DERIVED*, D3DXFRAME_DERIVED*);
	void NonAnimationDraw(D3DXFRAME_DERIVED*);
private:
	bool m_UseBorn;
	// �V�F�[�_�[�ɒl���Z�b�g���邽�߂̃n���h���Q
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


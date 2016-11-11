#pragma once
#include "Render.h"
#include "RenderTarget.h"
#include "Camera.h"

#define HEXA 6	// �L���[�u�}�b�v�͘Z�ʑ�

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

// �Z�ʑ̂��ꂼ��̖ʂ�`�悷��ۂ̃J�����̌����B
static D3DXVECTOR3 lookAt[6] = {
	D3DXVECTOR3(1.0f, 0.0f, 0.0f), // +X
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), // -X
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Y
	D3DXVECTOR3(0.0f, -1.0f, 0.0f), // -Y
	D3DXVECTOR3(0.0f, 0.0f, 1.0f), // +Z
	D3DXVECTOR3(0.0f, 0.0f, -1.0f) // -Z
};
// �Z�ʑ̂̂��ꂼ��̖ʂ�`�悷��ۂ̏����(��ʂƉ��ʂ̕`��͓��ɒ���)�B
static D3DXVECTOR3 up[6] = {
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +X(Up = +Y)
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -X(Up = +Y)
	D3DXVECTOR3(0.0f, 0.0f, -1.0f), // +Y(Up = -Z)
	D3DXVECTOR3(0.0f, 0.0f, 1.0f), // -X(Up = +Z)
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Z(Up = +Y)
	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -Z(Up = +Y)
};

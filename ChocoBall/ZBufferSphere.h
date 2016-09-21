#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Light.h"

class CGameCamera;

class CPlayer;

class CZBufferSphere : public CGameObject{
public:
	CZBufferSphere();
	~CZBufferSphere();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void SetUpTechnique()override{
		m_pRender->SetUpTechnique("ZMask");
	};
	LPDIRECT3DTEXTURE9 GetTexture(){
		return m_pZBufferSphereShadow;
	}
private:
	CGameCamera* GameCamera;
	CPlayer* Player;
	LPDIRECT3DSURFACE9 m_pMapZ = nullptr;			// �[�x�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pZBufferSphereShadow = nullptr;		// �e�𗎂Ƃ����߂̃e�N�X�`��
	LPDIRECT3DSURFACE9 m_pOriginalSurf = nullptr;	// �T�[�t�F�X
};
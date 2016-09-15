#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "C3DImage.h"
#include "Camera.h"

class CShadowRender
{
	SINGLETON_DECL(CShadowRender)
	enum RenderState{None = 0,Boneless,Bone};
public:
	// �e�𐶐�����I�u�W�F�N�g��o�^����֐�
	// �������F C3DImage* C3DImage���p�������N���X�̃|�C���^
	// �Ԃ�l�F �Ȃ�
	void Entry(CGameObject*);
	void Initialize();
	void Update();
	void Draw();
	void BeginDraw();
	void EndDraw();
	void SetObjectPos(D3DXVECTOR3 pos){
		m_target = pos;
	}
	void SetShadowCameraPos(D3DXVECTOR3 pos){
		m_ShadowLightPos = pos;
	}
	void SetShadowCameraDir(D3DXVECTOR3 dir){
		m_ShadowLightDirection = dir;
	}
	LPDIRECT3DTEXTURE9 GetTexture(){
		return m_pShadow;
	}
	void SetShadowCamera(LPD3DXEFFECT effect){
		effect->SetMatrix("LightViewProj", &(m_camera.GetView() * m_camera.GetProj()));
		m_camera.SetFarNear(effect);
	}
	void DeleteObject(CGameObject*);
	void DeleteObjectImidieit(CGameObject*);
	void CleanManager();
	void ExcuteDeleteObjects();
private:
	LPDIRECT3DSURFACE9 m_pMapZ = nullptr;			// �[�x�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pShadow = nullptr;		// �e�𗎂Ƃ����߂̃e�N�X�`��
	LPDIRECT3DSURFACE9 m_pOriginalSurf = nullptr;	// �T�[�t�F�X
	vector<CGameObject*> m_ShadowObjects;	// �e�𐶐��������I�u�W�F�N�g�̔z��
	vector<CGameObject*> m_DeleteObjects;
	D3DXVECTOR3 m_target;		// �e�𐶐��������I�u�W�F�N�g�̃|�W�V����
	D3DXVECTOR3 m_ShadowLightPos;		// �e��`�悷�邽�߂̃J�����̃|�W�V����
	D3DXVECTOR3 m_ShadowLightDirection;		// �e��`�悷�邽�߂̃J�����̌���
	CCamera m_camera;
	D3DXMATRIX m_View;					// ���C�g�̃r���[�s��
	D3DXMATRIX m_Proj;					// ���C�g�̃v���W�F�N�V�����s��
	void DeleteAll();
	RenderState m_RenderState;
};


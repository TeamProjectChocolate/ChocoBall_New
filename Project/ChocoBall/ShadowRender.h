#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "C3DImage.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "Infomation.h"

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
	void UpdateWeight(float);
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
#ifdef NOT_VSM
		return m_RenderTarget.GetTexture();
#else
		// �ŏI�I�ȉe�̃e�N�X�`����ԋp
		return m_BlurTarget[1].GetTexture();
#endif
	}
	void SetShadowCamera(LPD3DXEFFECT effect){
		effect->SetMatrix("LightViewProj", &(m_camera.GetView() * m_camera.GetProj()));
	}
	void DeleteObject(CGameObject*);
	void DeleteObjectImidieit(CGameObject*);
	void CleanManager();
	void ExcuteDeleteObjects();
private:
	CRenderTarget m_RenderTarget;
	CRenderTarget m_BlurTarget[2];
	D3DXVECTOR2 m_size[2];
	vector<CGameObject*> m_ShadowObjects;	// �e�𐶐��������I�u�W�F�N�g�̔z��
	vector<CGameObject*> m_DeleteObjects;
	D3DXVECTOR3 m_target;		// �e�𐶐��������I�u�W�F�N�g�̃|�W�V����
	D3DXVECTOR3 m_ShadowLightPos;		// �e��`�悷�邽�߂̃J�����̃|�W�V����
	D3DXVECTOR3 m_ShadowLightDirection;		// �e��`�悷�邽�߂̃J�����̌���
	CCamera m_camera;
	D3DXMATRIX m_View;					// ���C�g�̃r���[�s��
	D3DXMATRIX m_Proj;					// ���C�g�̃v���W�F�N�V�����s��
	void DeleteAll();
	static const int NUM_WEIGHTS = 8;
	float	m_weights[NUM_WEIGHTS];
	LPD3DXEFFECT m_pEffect;
	CPrimitive* m_Primitive;
};


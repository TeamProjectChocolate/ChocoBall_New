#include "stdafx.h"
#include "EM_Render.h"
#include "Effect.h"
#include "GraphicsDevice.h"
#include "RenderContext.h"
#include "ObjectManager.h"
#include "PixTag.h"

CEM_Render::CEM_Render()
{
}


CEM_Render::~CEM_Render()
{
	for (short idx = 0; idx < HEXA; idx++){
		SAFE_DELETE(m_pCubeSurfaces[idx]);
	}
	SAFE_DELETE(m_pCubeTex);
}

/*!
* @brief	�`��B
*/
void CEM_Render::Draw()
{
#ifdef NOT_EM
	m_isEnable = false;
#endif
	if (m_isEnable) {
		CPixTag tag;
		LPCWSTR name = L"EM";
		tag.Start(name);

		LPDIRECT3DSURFACE9 RenderingTarget;
		LPDIRECT3DSURFACE9 m_SavedMapZ;

		// ���Ƃ̃����_�����O�^�[�Q�b�g��ۑ�
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		vector<vector<OBJECT_DATA*>> Objects = SINSTANCE(CObjectManager)->GetObjectList();
		for (short hexa_idx = 0; hexa_idx < HEXA; hexa_idx++){
			(*graphicsDevice()).SetRenderTarget(0,m_pCubeSurfaces[hexa_idx]);
			(*graphicsDevice()).SetDepthStencilSurface(m_pZMap);
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			// 3D�̕`��
			{
				// ��{�`��
				for (short priorty = PRIORTY::PLAYER; priorty < PRIORTY::PARTICLE; priorty++){	// �D��x�̍������̂���X�V
					int size = Objects[priorty].size();
					for (int idx = 0; idx < size; idx++){
						if (Objects[priorty][idx]->object->GetAlive()) {	// �������Ă�����̂̂ݕ`��
							Objects[priorty][idx]->object->Draw_EM(&m_Cameras[hexa_idx]);
						}
					}
				}
				// �~�ς����f�[�^�ŃC���X�^���V���O�`��
				vector<RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER_STATE::EM_Sampling_I);
				for (auto data : datas){
					data->render->Draw();
				}
				// �p�[�e�B�N���`��
				for (short priorty = PRIORTY::PARTICLE; priorty < PRIORTY::OBJECT2D; priorty++){	// �D��x�̍������̂���X�V
					int size = Objects[priorty].size();
					for (int idx = 0; idx < size; idx++){
						if (Objects[priorty][idx]->object->GetAlive()){	// �������Ă�����̂̂ݕ`��
							Objects[priorty][idx]->object->Draw_EM(&m_Cameras[hexa_idx]);
						}
					}
				}
			}
			//// �`�悵�����̂Ƀu���[����������B
			//SINSTANCE(CRenderContext)->GetBloomRender()->Draw();
		}
		(*graphicsDevice()).SetRenderTarget(0, RenderingTarget);
		(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);

		tag.End();
		m_isEnable = false;	// �V�[���̍ŏ��̂ݕ`�悷��B
	}
};

/*!
* @brief	�쐬�B
*/
void CEM_Render::Initialize()
{
#ifdef NOT_EM
#else
	float size = 256.0f;
	// �L���[�u�}�b�v�����B
	(*graphicsDevice()).CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,&m_pCubeTex,nullptr);
	(*graphicsDevice()).CreateDepthStencilSurface(size, SINSTANCE(CRenderContext)->GetWindowWidth(), D3DFMT_D16, D3DMULTISAMPLE_NONE,0, true, &m_pZMap, NULL);
	// �T�[�t�F�C�X�擾�B
	for (short idx = 0; idx < HEXA; idx++){
		D3DCUBEMAP_FACES type = static_cast<D3DCUBEMAP_FACES>(idx);
		m_pCubeTex->GetCubeMapSurface(type, 0, &m_pCubeSurfaces[idx]);
	}
	// �J���������B
	//// �Z�ʑ̂��ꂼ��̖ʂ�`�悷��ۂ̃J�����̌����B
	//D3DXVECTOR3 lookAt[6] = {
	//	D3DXVECTOR3(1.0f, 0.0f, 0.0f), // +X
	//	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), // -X
	//	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Y
	//	D3DXVECTOR3(0.0f, -1.0f, 0.0f), // -Y
	//	D3DXVECTOR3(0.0f, 0.0f, 1.0f), // +Z
	//	D3DXVECTOR3(0.0f, 0.0f, -1.0f) // -Z
	//};
	//// �Z�ʑ̂̂��ꂼ��̖ʂ�`�悷��ۂ̏����(��ʂƉ��ʂ̕`��͓��ɒ���)�B
	//D3DXVECTOR3 up[6] = {
	//	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +X(Up = +Y)
	//	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -X(Up = +Y)
	//	D3DXVECTOR3(0.0f, 0.0f, -1.0f), // +Y(Up = -Z)
	//	D3DXVECTOR3(0.0f, 0.0f, 1.0f), // -X(Up = +Z)
	//	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Z(Up = +Y)
	//	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -Z(Up = +Y)
	//};

	for (short idx = 0; idx < HEXA; idx++){
		//float l_Far = 10000.0f;
		//float l_Near = 1.0f;
		//float l_ViewAngle = D3DXToRadian(90.0f);
		//D3DXVECTOR3 l_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//D3DXMatrixPerspectiveFovLH(
		//	&m_Proj,							// �ˉe�}�g���b�N�X
		//	l_ViewAngle,						// ��p
		//	1.0f,							// �A�X�y�N�g��
		//	l_Near,								// Near�v���[��
		//	l_Far);								// Far�v���[��
		m_Cameras[idx].Initialize();
		m_Cameras[idx].SetFar(10000.0f);
		m_Cameras[idx].SetNear(1.0f);
		m_Cameras[idx].SetAspect(1.0f);
		m_Cameras[idx].SetViewAngle(D3DXToRadian(90.0f));
		m_Cameras[idx].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_Cameras[idx].SetTarget(lookAt[idx]);
		m_Cameras[idx].SetUp(up[idx]);
		m_Cameras[idx].SetUpdateType(EUpdateType::enUpdateTypeTarget);
		m_Cameras[idx].SetNotWorkOutFlg(true);
		m_Cameras[idx].Update();
		D3DXMATRIX* view = m_Cameras[idx].GetViewPointer();
		CCamera* pCamera = SINSTANCE(CRenderContext)->GetCurrentCamera();
		if (pCamera){
			D3DXMatrixMultiply(view, view, &(pCamera->GetView()));
		}
		//D3DXMatrixLookAtLH(&m_View, &l_pos, &lookAt[idx], &up[idx]);			// �r���[�}�g���b�N�X�ݒ�
	}
	m_isEnable = true;
#endif
};

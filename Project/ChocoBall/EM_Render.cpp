#include "stdafx.h"
#include "EM_Render.h"
#include "Effect.h"
#include "GraphicsDevice.h"
#include "RenderContext.h"
#include "ObjectManager.h"
#include "PixTag.h"

CEM_Render::CEM_Render()
{
	m_CameraPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

		const vector<vector<OBJECT_DATA*>>& Objects = SINSTANCE(CObjectManager)->GetObjectList();
		for (short hexa_idx = 0; hexa_idx < HEXA; hexa_idx++){
			// �J�����̃|�W�V�����ݒ�B
			m_Cameras[hexa_idx].SetPos(m_CameraPos);
			// �J�����X�V�B
			m_Cameras[hexa_idx].Update();

			(*graphicsDevice()).SetRenderTarget(0,m_pCubeSurfaces[hexa_idx]);
			(*graphicsDevice()).SetDepthStencilSurface(m_pZMap);
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			// 3D�̕`��
			{
				// ��{�`��
				for (short priorty = OBJECT::PRIORTY::PLAYER; priorty < OBJECT::PRIORTY::PARTICLE; priorty++){	// �D��x�̍������̂���X�V
					int size = Objects[priorty].size();
					for (int idx = 0; idx < size; idx++){
						if (Objects[priorty][idx]->object->GetAlive()) {	// �������Ă�����̂̂ݕ`��
							Objects[priorty][idx]->object->Draw_EM(&m_Cameras[hexa_idx]);
						}
					}
				}
				// �~�ς����f�[�^�ŃC���X�^���V���O�`��
				const vector<CRenderContext::RENDER_DATA*>& datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER::TYPE::EM_Sampling_I);
				for (auto data : datas){
					data->render->Draw();
				}
				// �p�[�e�B�N���`��
				for (short priorty = OBJECT::PRIORTY::PARTICLE; priorty < OBJECT::PRIORTY::OBJECT2D; priorty++){	// �D��x�̍������̂���X�V
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
#ifdef TEST_EM
#else
		m_isEnable = false;	// �V�[���̍ŏ��̂ݕ`�悷��B
#endif
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

	D3DXMatrixIdentity(&m_CameraRota);
	m_CameraPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (short idx = 0; idx < HEXA; idx++){
		m_Cameras[idx].Initialize();
		m_Cameras[idx].SetFar(10000.0f);
		m_Cameras[idx].SetNear(1.0f);
		m_Cameras[idx].SetAspect(1.0f);
		m_Cameras[idx].SetViewAngle(D3DXToRadian(90.0f));
		m_Cameras[idx].SetPos(m_CameraPos);
		m_Cameras[idx].SetDirection(lookAt[idx]);
		m_Cameras[idx].SetUp(up[idx]);
		m_Cameras[idx].SetUpdateType(EUpdateType::enUpdateTypeDirection);
		m_Cameras[idx].SetNotWorkOutFlg(true);
	}
	m_isEnable = true;
#endif
};

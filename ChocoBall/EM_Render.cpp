#include "stdafx.h"
#include "EM_Render.h"
#include "Effect.h"
#include "GraphicsDevice.h"
#include "RenderContext.h"
#include "ObjectManager.h"

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
		LPDIRECT3DSURFACE9 RenderingTarget;
		LPDIRECT3DSURFACE9 m_SavedMapZ;

		// ���Ƃ̃����_�����O�^�[�Q�b�g��ۑ�
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		vector<OBJECT_DATA*> Objects = SINSTANCE(CObjectManager)->GetObjectList();
		for (short idx = 0; idx < HEXA; idx++){
			(*graphicsDevice()).SetRenderTarget(0,m_pCubeSurfaces[idx]);
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			int size = Objects.size();
			// 3D�̕`��
			{
				// ��{�`��
				for (short priorty = 0; priorty < PRIORTY::PARTICLE; priorty++){	// �D��x�̍������̂���X�V
					for (int idx = 0; idx < size; idx++){
						if (Objects[idx]->object->GetAlive()){	// �������Ă�����̂̂ݕ`��
							if (Objects[idx]->priority == priorty){	// ���݂̗D��x�ƈ�v������̂�`��
								Objects[idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
								Objects[idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
								Objects[idx]->object->Draw();
							}
						}
					}
				}
				// �~�ς����f�[�^�ŃC���X�^���V���O�`��
				vector<RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER_STATE::Instancing);
				for (auto data : datas){
					data->render->Draw();
				}
				// �p�[�e�B�N���`��
				for (short priorty = PRIORTY::PARTICLE; priorty < PRIORTY::OBJECT2D; priorty++){	// �D��x�̍������̂���X�V
					for (int idx = 0; idx < size; idx++){

						if (Objects[idx]->object->GetAlive()){	// �������Ă�����̂̂ݕ`��
							if (Objects[idx]->priority == priorty){	// ���݂̗D��x�ƈ�v������̂�`��
								Objects[idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
								Objects[idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
								Objects[idx]->object->Draw();
							}
						}
					}
				}
			}
		}
		(*graphicsDevice()).SetRenderTarget(0, RenderingTarget);
		(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);
		m_isEnable = false;	// �V�[���̍ŏ��̂ݕ`�悷��B
	}
};

/*!
* @brief	�쐬�B
*/
void CEM_Render::Initialize()
{
	// �L���[�u�}�b�v�����B
	D3DXCreateCubeTexture(&(*graphicsDevice()),SINSTANCE(CRenderContext)->GetWindowWidth(), 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,&m_pCubeTex);
	// �T�[�t�F�C�X�擾�B
	for (short idx = 0; idx < HEXA; idx++){
		D3DCUBEMAP_FACES type = static_cast<D3DCUBEMAP_FACES>(idx);
		m_pCubeTex->GetCubeMapSurface(type, 0, &m_pCubeSurfaces[idx]);
	}

	// �J���������B
	// �Z�ʑ̂��ꂼ��̖ʂ�`�悷��ۂ̃J�����̌����B
	D3DXVECTOR3 lookAt[6] = {
		D3DXVECTOR3(1.0f, 0.0f, 0.0f), // +X
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f), // -X
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Y
		D3DXVECTOR3(0.0f, -1.0f, 0.0f), // -Y
		D3DXVECTOR3(0.0f, 0.0f, 1.0f), // +Z
		D3DXVECTOR3(0.0f, 0.0f, -1.0f) // -Z
	};
	// �Z�ʑ̂̂��ꂼ��̖ʂ�`�悷��ۂ̏����(��ʂƉ��ʂ̕`��͓��ɒ���)�B
	D3DXVECTOR3 up[6] = {
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +X(Up = +Y)
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -X(Up = +Y)
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), // +Y(Up = -Z)
		D3DXVECTOR3(0.0f, 0.0f, 1.0f), // -X(Up = +Z)
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Z(Up = +Y)
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), // -Z(Up = +Y)
	};

	for (short idx = 0; idx < HEXA; idx++){
		m_Cameras[idx].Initialize();
		m_Cameras[idx].SetFar(10000.0f);
		m_Cameras[idx].SetNear(1.0f);
		m_Cameras[idx].SetViewAngle(D3DXToRadian(90.0f));
		m_Cameras[idx].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_Cameras[idx].SetTarget(lookAt[idx]);
		m_Cameras[idx].SetUp(up[idx]);
		m_Cameras[idx].SetUpdateType(EUpdateType::enUpdateTypeTarget);
		m_Cameras[idx].Update();
	}

	m_pEffect = SINSTANCE(CEffect)->LoadEffect(_T("Shader/ModelShader.hlsl"));
	m_isEnable = true;
};

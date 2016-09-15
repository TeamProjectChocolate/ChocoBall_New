#include "stdafx.h"
#include "DofRender.h"
#include "GraphicsDevice.h"
#include "RenderContext.h"
#include "ObjectManager.h"
#include "C3DImage.h"


CDofRender::CDofRender()
{
}


CDofRender::~CDofRender()
{
}

void CDofRender::UpdateWeight(float dispersion)
{
	float total = 0;
	for (int i = 0; i<NUM_WEIGHTS; i++) {
		m_weights[i] = expf(-0.5f*(float)(i*i) / dispersion);
		if (0 == i) {
			total += m_weights[i];
		}
		else {
			// ���S�ȊO�́A�Q�񓯂��W�����g���̂łQ�{
			total += 2.0f*m_weights[i];
		}
	}
	// �K�i��
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= total;
	}
};

/*!
* @brief	�`��B
*/
void CDofRender::Draw()
{

	if (m_isEnable) {
		UpdateWeight(200.0f);
		LPDIRECT3DSURFACE9 RenderingTarget;
		LPDIRECT3DSURFACE9 m_SavedMapZ;

		// ���Ƃ̃����_�����O�^�[�Q�b�g��ۑ�
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		m_Primitive = SINSTANCE(CRenderContext)->GetPrimitive();

		// �[�x��񒊏o
		{
			(*graphicsDevice()).SetRenderTarget(0, m_DepthSamplingTarget.GetSurface());
			(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

			D3DXVECTOR3 PintoPos;

			if (m_UsePintoObject){
				m_pPintoObject = SINSTANCE(CObjectManager)->FindGameObject<CGameObject>(_T("TEST3D"));

				if (m_pPintoObject != nullptr){
					CModel* model = m_pPintoObject->GetModel();
					PintoPos = m_pPintoObject->GetPos();
					// ���s�ړ������݂̂ɂ���
					D3DXMATRIX work = model->m_World;
					D3DXMatrixIdentity(&m_PintoWorld);
					memcpy(&m_PintoWorld.m[3][0], &work.m[3][0], sizeof(float)* 4);
				}
			}
			else{
				m_pPintoObject = nullptr;
				PintoPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXMatrixIdentity(&m_PintoWorld);
				D3DXMatrixTranslation(&m_PintoWorld, PintoPos.x, PintoPos.y, PintoPos.z);
			}

			// ��{�`��
			vector<OBJECT_DATA*> Objects = SINSTANCE(CObjectManager)->GetObjectList();
			int size = Objects.size();
			for (PRIORTY priorty = PRIORTY::PLAYER; priorty < PRIORTY::PARTICLE; priorty = static_cast<PRIORTY>(priorty + 1)){	// �D��x�̍������̂���X�V
				for (int idx = 0; idx < size; idx++){
					if (Objects[idx]->object->GetAlive()){
						if (Objects[idx]->priority == priorty){
							Objects[idx]->object->DrawDepth(m_FarNear,PintoPos,m_PintoWorld);
						}
					}
				}
			}
			// �~�ς����f�[�^�ŃC���X�^���V���O�`��
			vector<RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER_STATE::Instancing_Depth);
			for (auto data : datas){
				data->render->Draw(m_FarNear, PintoPos, m_PintoWorld);
			}
			// �p�[�e�B�N���`��
			for (PRIORTY priorty = PRIORTY::PARTICLE; priorty < PRIORTY::OBJECT2D; priorty = static_cast<PRIORTY>(priorty + 1)){	// �D��x�̍������̂���X�V
				for (int idx = 0; idx < size; idx++){
					if (Objects[idx]->object->GetAlive()){
						if (Objects[idx]->priority == priorty){
							Objects[idx]->object->DrawDepth(m_FarNear, PintoPos, m_PintoWorld);
						}
					}
				}
			}
		}
		int w = SINSTANCE(CRenderContext)->GetWindowWidth();
		int h = SINSTANCE(CRenderContext)->GetWindowHeight();

		//XBlur
		{
			(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[0].GetSurface());
			//(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			m_pEffect->SetTechnique("XBlur");
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_pEffect->BeginPass(0);
			float size[2] = {
				static_cast<float>(w),
				static_cast<float>(h)
			};
			float offset[] = {
				16.0f / static_cast<float>(w),
				0.0f
			};
			m_pEffect->SetValue("g_TexSize", size, sizeof(size));
			m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

			m_pEffect->SetTexture("g_blur", m_pSampleTexture);
			m_pEffect->CommitChanges();

			(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
			(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			m_pEffect->EndPass();
			m_pEffect->End();

		}
		//YBlur
		{
			(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[1].GetSurface());
			//(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			m_pEffect->SetTechnique("YBlur");
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_pEffect->BeginPass(0);
			float size[2] = {
				static_cast<float>(w),
				static_cast<float>(h)
			};
			float offset[] = {
				0.0f,
				16.0f / static_cast<float>(h)
			};
			m_pEffect->SetValue("g_TexSize", size, sizeof(size));
			m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

			m_pEffect->SetTexture("g_blur", m_BlurTarget[0].GetTexture());
			m_pEffect->CommitChanges();
			(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
			(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			m_pEffect->EndPass();
			m_pEffect->End();
		}

		{
			float offset[] = {
				0.5f / static_cast<float>(w),
				0.5f / static_cast<float>(h)
			};
			//�߂��B
			(*graphicsDevice()).SetRenderTarget(0, RenderingTarget);
			(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);

			//�[�x�����Q�Ƃ��č����B
			m_pEffect->SetTechnique("Final");
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_pEffect->BeginPass(0);
			m_pEffect->SetTexture("g_Texture", m_pSampleTexture);
			m_pEffect->SetTexture("g_DepthSample", m_DepthSamplingTarget.GetTexture());
			m_pEffect->SetTexture("g_blur", m_BlurTarget[1].GetTexture());
			m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			m_pEffect->CommitChanges();
			(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
			(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			m_pEffect->EndPass();
			m_pEffect->End();
		}
	}

};

/*!
* @brief	�쐬�B
*/
void CDofRender::Initialize()
{
	int w = SINSTANCE(CRenderContext)->GetWindowWidth();
	int h = SINSTANCE(CRenderContext)->GetWindowHeight();

	//�����_�����O�^�[�Q�b�g���쐬�B
	//�[�x��񒊏o�p
	m_DepthSamplingTarget.CreateRenderingTarget(w, h, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	//�u���[�p(���p�Əc�p)
	for (short i = 0; i < 2; i++){
		m_BlurTarget[i].CreateRenderingTarget(w, h, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	}

	m_pEffect = SINSTANCE(CEffect)->LoadEffect(_T("Shader/DepthOfField.hlsl"));

	m_FarNear = D3DXVECTOR2(100.0f, 1.0f);

	m_isEnable = true;
	m_UsePintoObject = true;
};

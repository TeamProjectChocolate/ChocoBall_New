#include "stdafx.h"
#include "BloomRender.h"
#include "GraphicsDevice.h"
#include "RenderContext.h"
#include "PixTag.h"


namespace{
#if defined(USE_LOW_QUALITY_BLOOM)
	const float BLUR_OFFFSET = 8.0f;
#else
	const float BLUR_OFFFSET = 16.0f;
#endif
}
CBloomRender::CBloomRender()
{
};


CBloomRender::~CBloomRender()
{
};


void CBloomRender::UpdateWeight(float dispersion)
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
	// ����͏d�݂����v�����1.0�ɂȂ�悤�ɋK�i�����Ă���B
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= total;
	}
};

/*!
* @brief	�`��B
*/
void CBloomRender::Draw()
{
#ifdef NOT_BLOOM
	m_isEnable = false;
#endif
	if (m_isEnable) {
		CPixTag tag;
		LPCWSTR name = L"Bloom";
		tag.Start(name);

		UpdateWeight(100.0f);
		LPDIRECT3DSURFACE9 RenderingTarget;
		LPDIRECT3DSURFACE9 m_SavedMapZ;

		// ���Ƃ̃����_�����O�^�[�Q�b�g��ۑ�
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		m_Primitive = SINSTANCE(CRenderContext)->GetPrimitive();
		//�P�x���o
		{
			(*graphicsDevice()).SetRenderTarget(0, m_luminanceRenderTarget.GetSurface());
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0, 1.0f, 0);
			m_pEffect->SetTechnique("SamplingLuminance");
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_pEffect->BeginPass(0);
			m_pEffect->SetTexture("g_scene",m_pSampleTexture);
			m_pEffect->CommitChanges();

			(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
			(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			m_pEffect->EndPass();
			m_pEffect->End();

		}

		CRenderTarget* RenderTarget = &m_luminanceRenderTarget;
		for (int idx = 0; idx < MGF_DOWN_SAMPLE_COUNT; idx++) {
			//XBlur
			{
				// �܂����u���[��������B
				(*graphicsDevice()).SetRenderTarget(0, m_DownSamplingRenderTarget[idx][0].GetSurface());
				//(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
				m_pEffect->SetTechnique("XBlur");
				m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
				m_pEffect->BeginPass(0);
				float size[2] = {
					m_size[idx][0].x,
					m_size[idx][0].y
				};
				float offset[] = {
					BLUR_OFFFSET / m_size[idx][0].x,
					0.0f
				};
				m_pEffect->SetValue("g_luminanceTexSize", size, sizeof(size));
				m_pEffect->SetValue("g_offset", offset, sizeof(offset));
				m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

				// �P�x�e�N�X�`�����Z�b�g�B
				m_pEffect->SetTexture("g_blur", RenderTarget->GetTexture());
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
				// ���u���[�����������̂ɂ���ɏc�u���[�������ă_�E���T���v�����O�����B
				(*graphicsDevice()).SetRenderTarget(0, m_DownSamplingRenderTarget[idx][1].GetSurface());
				//(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
				m_pEffect->SetTechnique("YBlur");
				m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
				m_pEffect->BeginPass(0);
				float size[2] = {
					m_size[idx][1].x,
					m_size[idx][1].y
				};
				float offset[] = {
					0.0f,
					BLUR_OFFFSET / m_size[idx][1].y
				};
				m_pEffect->SetValue("g_luminanceTexSize", size, sizeof(size));
				m_pEffect->SetValue("g_offset", offset, sizeof(offset));
				m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

				// ���u���[���������e�N�X�`�����Z�b�g�B
				m_pEffect->SetTexture("g_blur", m_DownSamplingRenderTarget[idx][0].GetTexture());
				m_pEffect->CommitChanges();
				(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
				(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
				(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

				(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				m_pEffect->EndPass();
				m_pEffect->End();
			}

			////XBlur2
			//{
			//	(*graphicsDevice()).SetRenderTarget(0, m_DownSamplingRenderTarget[2].GetSurface());
			//	//(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			//	m_pEffect->SetTechnique("XBlur");
			//	m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			//	m_pEffect->BeginPass(0);
			//	float size[2] = {
			//		static_cast<float>(m_size[1].w),
			//		static_cast<float>(m_size[1].h)
			//	};
			//	float offset[] = {
			//		BLUR_OFFFSET / static_cast<float>(m_size[1].w),
			//		0.0f
			//	};
			//	m_pEffect->SetValue("g_luminanceTexSize", size, sizeof(size));
			//	m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			//	m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

			//	m_pEffect->SetTexture("g_blur", m_DownSamplingRenderTarget[1].GetTexture());
			//	m_pEffect->CommitChanges();
			//	(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			//	(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
			//	(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			//	(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			//	m_pEffect->EndPass();
			//	m_pEffect->End();

			//}
			////YBlur2
			//{
			//	(*graphicsDevice()).SetRenderTarget(0, m_DownSamplingRenderTarget[3].GetSurface());
			//	//(*graphicsDevice()).Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			//	m_pEffect->SetTechnique("YBlur");
			//	m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			//	m_pEffect->BeginPass(0);
			//	float size[2] = {
			//		static_cast<float>(m_size[2].w)	,
			//		static_cast<float>(m_size[2].h)
			//	};
			//	float offset[] = {
			//		0.0f,
			//		BLUR_OFFFSET / static_cast<float>(m_size[2].h)
			//	};
			//	m_pEffect->SetValue("g_luminanceTexSize", size, sizeof(size));
			//	m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			//	m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

			//	m_pEffect->SetTexture("g_blur", m_DownSamplingRenderTarget[2].GetTexture());
			//	m_pEffect->CommitChanges();
			//	(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			//	(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
			//	(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			//	(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			//	m_pEffect->EndPass();
			//	m_pEffect->End();
			//}

			{
				float offset[] = {
					0.5f / m_size[idx][1].x,
					0.5f / m_size[idx][1].y
				};
				//�߂��B
				(*graphicsDevice()).SetRenderTarget(0, RenderingTarget);
				(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);

				//���Z�����B
				(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				m_pEffect->SetTechnique("Final");
				m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
				m_pEffect->BeginPass(0);
				// �c���u���[�����ς݂̃e�N�X�`�����Z�b�g�B
				m_pEffect->SetTexture("g_blur", m_DownSamplingRenderTarget[idx][1].GetTexture());
				m_pEffect->SetValue("g_offset", offset, sizeof(offset));
				m_pEffect->CommitChanges();
				(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
				(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
				(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

				(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				m_pEffect->EndPass();
				m_pEffect->End();

				(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			RenderTarget = &m_DownSamplingRenderTarget[idx][1];
		}
		tag.End();
	}
};

/*!
* @brief	�쐬�B
*/
void CBloomRender::Initialize()
{
#ifdef NOT_BLOOM
#else
	int w = SINSTANCE(CRenderContext)->GetWindowWidth();
	int h = SINSTANCE(CRenderContext)->GetWindowHeight();
	//�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬�B
	// �����_�����O�^�[�Q�b�g����
	m_luminanceRenderTarget.CreateRenderingTarget(w, h, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);

	//�_�E���T���v�����O�p(�u���[�p)�̃����_�����O�^�[�Q�b�g���쐬�B
	for (short i = 0,w_num = 0,h_num = 0; i < MGF_DOWN_SAMPLE_COUNT; i++){
		// ���u���[�p�̃T�C�Y�ۑ��B
		m_size[i][0].x = w >> ++w_num;
		m_size[i][0].y = h >> h_num;
		// ���u���[�p�̃����_�����O�^�[�Q�b�g�����B
		m_DownSamplingRenderTarget[i][0].CreateRenderingTarget(m_size[i][0].x, m_size[i][0].y, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
		// �c�u���[�p�̃T�C�Y�ۑ��B
		m_size[i][1].x = w >> w_num;
		m_size[i][1].y = h >> ++h_num;
		// �c�u���[�p�̃����_�����O�^�[�Q�b�g�����B
		m_DownSamplingRenderTarget[i][1].CreateRenderingTarget(m_size[i][1].x, m_size[i][1].y, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	}

	m_pEffect = SINSTANCE(CEffect)->LoadEffect(_T("Shader/Bloom.hlsl"));
	m_isEnable = true;
#endif
};

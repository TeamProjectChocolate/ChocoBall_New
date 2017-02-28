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
			// 中心以外は、２回同じ係数を使うので２倍
			total += 2.0f*m_weights[i];
		}
	}
	// 規格化
	// 今回は重みを合計すると1.0になるように規格化している。
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= total;
	}
};

/*!
* @brief	描画。
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

		// もとのレンダリングターゲットを保存
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		m_Primitive = SINSTANCE(CRenderContext)->GetPrimitive();
		//輝度抽出
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
				// まず横ブラーをかける。
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

				// 輝度テクスチャをセット。
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
				// 横ブラーをかけたものにさらに縦ブラーをかけてダウンサンプリング完了。
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

				// 横ブラーをかけたテクスチャをセット。
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
				//戻す。
				(*graphicsDevice()).SetRenderTarget(0, RenderingTarget);
				(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);

				//加算合成。
				(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				m_pEffect->SetTechnique("Final");
				m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
				m_pEffect->BeginPass(0);
				// 縦横ブラー完了済みのテクスチャをセット。
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
* @brief	作成。
*/
void CBloomRender::Initialize()
{
#ifdef NOT_BLOOM
#else
	int w = SINSTANCE(CRenderContext)->GetWindowWidth();
	int h = SINSTANCE(CRenderContext)->GetWindowHeight();
	//輝度抽出用のレンダリングターゲットを作成。
	// レンダリングターゲット生成
	m_luminanceRenderTarget.CreateRenderingTarget(w, h, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);

	//ダウンサンプリング用(ブラー用)のレンダリングターゲットを作成。
	for (short i = 0,w_num = 0,h_num = 0; i < MGF_DOWN_SAMPLE_COUNT; i++){
		// 横ブラー用のサイズ保存。
		m_size[i][0].x = w >> ++w_num;
		m_size[i][0].y = h >> h_num;
		// 横ブラー用のレンダリングターゲット生成。
		m_DownSamplingRenderTarget[i][0].CreateRenderingTarget(m_size[i][0].x, m_size[i][0].y, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
		// 縦ブラー用のサイズ保存。
		m_size[i][1].x = w >> w_num;
		m_size[i][1].y = h >> ++h_num;
		// 縦ブラー用のレンダリングターゲット生成。
		m_DownSamplingRenderTarget[i][1].CreateRenderingTarget(m_size[i][1].x, m_size[i][1].y, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	}

	m_pEffect = SINSTANCE(CEffect)->LoadEffect(_T("Shader/Bloom.hlsl"));
	m_isEnable = true;
#endif
};

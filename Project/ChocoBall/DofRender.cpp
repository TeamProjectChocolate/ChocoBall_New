#include "stdafx.h"
#include "DofRender.h"
#include "GraphicsDevice.h"
#include "RenderContext.h"
#include "ObjectManager.h"
#include "C3DImage.h"
#include "PixTag.h"


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
			// 中心以外は、２回同じ係数を使うので２倍
			total += 2.0f*m_weights[i];
		}
	}
	// 規格化
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= total;
	}
};

/*!
* @brief	描画。
*/
void CDofRender::Draw()
{
#ifdef NOT_DOF
	m_isEnable = false;
#endif
	if (m_isEnable) {
		CPixTag tag;
		LPCWSTR name = L"DofBlur";
		tag.Start(name);
		UpdateWeight(20.0f);
		LPDIRECT3DSURFACE9 RenderingTarget;
		LPDIRECT3DSURFACE9 m_SavedMapZ;

		// もとのレンダリングターゲットを保存
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		m_Primitive = SINSTANCE(CRenderContext)->GetPrimitive();

		int w = SINSTANCE(CRenderContext)->GetWindowWidth();
		int h = SINSTANCE(CRenderContext)->GetWindowHeight();

		//XBlur
		{
			(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[0].GetSurface());
			m_pEffect->SetTechnique("XBlur");
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_pEffect->BeginPass(0);
			float size[2] = {
				static_cast<float>(m_size[0].x),
				static_cast<float>(m_size[0].y)
			};
			float offset[] = {
				16.0f / static_cast<float>(m_size[0].x),
				0.0f
			};
			m_pEffect->SetValue("g_TexSize", size, sizeof(size));
			m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

			m_pEffect->SetTexture("g_blur", m_pSampleTexture);
			m_pEffect->CommitChanges();

			(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(PRIMITIVE::SShapeVertex_PT));
			(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			m_pEffect->EndPass();
			m_pEffect->End();

		}
		//YBlur
		{
			(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[1].GetSurface());
			m_pEffect->SetTechnique("YBlur");
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_pEffect->BeginPass(0);
			float size[2] = {
				static_cast<float>(m_size[1].x),
				static_cast<float>(m_size[1].y)
			};
			float offset[] = {
				0.0f,
				16.0f / static_cast<float>(m_size[1].y)
			};
			m_pEffect->SetValue("g_TexSize", size, sizeof(size));
			m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

			m_pEffect->SetTexture("g_blur", m_BlurTarget[0].GetTexture());
			m_pEffect->CommitChanges();
			(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(PRIMITIVE::SShapeVertex_PT));
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
			//戻す。
			(*graphicsDevice()).SetRenderTarget(0, RenderingTarget);
			(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);

			//深度情報を参照して合成。
			m_pEffect->SetTechnique("Final");
			m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_pEffect->BeginPass(0);
			m_pEffect->SetTexture("g_Texture", m_pSampleTexture);
			m_pEffect->SetTexture("g_DepthSample", m_DepthSamplingTarget.GetTexture());
			m_pEffect->SetTexture("g_blur", m_BlurTarget[1].GetTexture());
			m_pEffect->SetValue("g_offset", offset, sizeof(offset));
			m_pEffect->CommitChanges();
			(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
			(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(PRIMITIVE::SShapeVertex_PT));
			(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

			(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			m_pEffect->EndPass();
			m_pEffect->End();
		}
		tag.End();
	}
	
};

/*!
* @brief	作成。
*/
void CDofRender::Initialize()
{
#ifdef NOT_DOF
#else
	int w = SINSTANCE(CRenderContext)->GetWindowWidth();
	int h = SINSTANCE(CRenderContext)->GetWindowHeight();

	//レンダリングターゲットを作成。
	//深度情報抽出用
	m_DepthSamplingTarget.CreateRenderingTarget(w, h, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	//ブラー用(横用と縦用)
	for (short i = 0, w_num = 0, h_num = 0; i < 2; i++){
		if (i % 2 == 0){
			w_num++;
		}
		else{
			h_num++;
		}
		m_size[i] = D3DXVECTOR2(w >> w_num,h >> h_num);
		m_BlurTarget[i].CreateRenderingTarget(w >> w_num, h >> h_num, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	}

	m_pEffect = SINSTANCE(CEffect)->LoadEffect(_T("Shader/DepthOfField.hlsl"));

	m_DepthFarNear = D3DXVECTOR2(100.0f, 1.0f);

	m_isEnable = true;
	m_UsePintoObject = true;
#endif
};

void CDofRender::MathPinto(){
	D3DXVECTOR3 PintoPos;

	if (m_UsePintoObject){
		m_pPintoObject = SINSTANCE(CObjectManager)->FindGameObject<CGameObject>(_T("Player"));

		if (m_pPintoObject != nullptr){
			CModel* model = m_pPintoObject->GetModel();
			PintoPos = m_pPintoObject->GetPos();
			// 平行移動成分のみにする
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
}
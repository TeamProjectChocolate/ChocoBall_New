#include "stdafx.h"
#include "ShadowRender.h"
#include "Effect.h"
#include "GraphicsDevice.h"
#include "RenderContext.h"
#include "ShadowSamplingRender_I.h"

extern UINT                        g_NumBoneMatricesMax;
extern D3DXMATRIXA16*              g_pBoneMatrices;

CShadowRender* CShadowRender::m_instance = nullptr;

void CShadowRender::Entry(CGameObject* Obj){
	Obj->ActivateShadowRender();
	m_ShadowObjects.push_back(Obj);
}

void CShadowRender::Initialize(){
	// レンダリングターゲット生成
	m_RenderTarget.CreateRenderingTarget(WINDOW_WIDTH, WINDOW_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	m_BlurTarget[0].CreateRenderingTarget(WINDOW_WIDTH >> 1, WINDOW_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);
	m_BlurTarget[1].CreateRenderingTarget(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT);

	m_camera.Initialize();

	// 影描画用プロジェクション行列生成用の値をセット
	m_camera.SetUpdateType(EUpdateType::enUpdateTypeTarget);
	m_camera.SetNear(1.0f);
	m_camera.SetFar(20.0f);
	m_camera.SetAspect(WINDOW_WIDTH / WINDOW_HEIGHT);
	m_camera.SetViewAngle(D3DXToRadian(60));

	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ShadowTex.hlsl"));
}

void CShadowRender::Update(){
	m_camera.SetPos(m_ShadowLightPos);
	m_camera.SetTarget(m_target);
	m_camera.Update();
}

void CShadowRender::UpdateWeight(float dispersion)
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

void CShadowRender::Draw(){
	this->UpdateWeight(200.0f);

	// もとのレンダリングターゲットを保存
	IDirect3DSurface9* pOldBackBuffer;
	IDirect3DSurface9* pOldZBuffer;

	(*graphicsDevice()).GetRenderTarget(0, &pOldBackBuffer);
	(*graphicsDevice()).GetDepthStencilSurface(&pOldZBuffer);

	// レンダリングターゲットの変更
	(*graphicsDevice()).SetRenderTarget(0, m_RenderTarget.GetSurface());
	(*graphicsDevice()).SetDepthStencilSurface(m_RenderTarget.GetZMap());

	(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	for (int idx = 0, size = m_ShadowObjects.size(); idx < size; idx++){
		if (m_ShadowObjects[idx] != nullptr){
			if (m_ShadowObjects[idx]->GetAlive()){
				m_ShadowObjects[idx]->DrawShadow(&m_camera);
			}
		}
		else{
			DeleteObject(m_ShadowObjects[idx]);
		}
	}
	// インスタンシング
	vector<RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER_STATE::_3D_ShadowSample_I);
	for (auto data : datas){
			data->render->Draw();
	}

	// ブラーをかける
	m_Primitive = SINSTANCE(CRenderContext)->GetPrimitive();
	{
		(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[0].GetSurface());
		(*graphicsDevice()).SetDepthStencilSurface(m_BlurTarget[0].GetZMap());
		m_pEffect->SetTechnique("XBlur");
		m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		m_pEffect->BeginPass(0);

		float size[2] = {
			static_cast<float>(WINDOW_WIDTH),
			static_cast<float>(WINDOW_HEIGHT)
		};
		float offset[] = {
			16.0f / static_cast<float>(WINDOW_WIDTH),
			0.0f
		};
		m_pEffect->SetValue("g_TexSize", size, sizeof(size));
		m_pEffect->SetValue("g_offset", offset, sizeof(offset));
		m_pEffect->SetValue("g_weight", m_weights, sizeof(m_weights));

		m_pEffect->SetTexture("g_blur", m_RenderTarget.GetTexture());
		m_pEffect->CommitChanges();

		(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
		(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
		(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

		(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		m_pEffect->EndPass();
		m_pEffect->End();
	}
	{
		//YBlur
		(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[1].GetSurface());
		(*graphicsDevice()).SetDepthStencilSurface(m_BlurTarget[1].GetZMap());
		m_pEffect->SetTechnique("YBlur");
		m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		m_pEffect->BeginPass(0);
		float size[2] = {
			static_cast<float>(WINDOW_WIDTH),
			static_cast<float>(WINDOW_HEIGHT)
		};
		float offset[] = {
			0.0f,
			16.0f / static_cast<float>(WINDOW_HEIGHT)
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

	// レンダリングターゲットを元に戻す
	(*graphicsDevice()).SetRenderTarget(0, pOldBackBuffer);
	(*graphicsDevice()).SetDepthStencilSurface(pOldZBuffer);
}

void CShadowRender::DeleteObject(CGameObject* pObject){
	m_DeleteObjects.push_back(pObject);
}

void CShadowRender::DeleteObjectImidieit(CGameObject* pObject){
	vector<CGameObject*>::iterator itr;
	for (itr = m_ShadowObjects.begin(); itr != m_ShadowObjects.end();){
		if (pObject == (*itr)){
			itr = m_ShadowObjects.erase(itr);
			return;
		}
		else{
			itr++;
		}
	}
}

void CShadowRender::CleanManager(){
	int size = m_ShadowObjects.size();
	for (int idx = 0; idx < size; idx++){
		m_DeleteObjects.push_back(m_ShadowObjects[idx]);
	}
}

void CShadowRender::ExcuteDeleteObjects(){
	vector<CGameObject*>::iterator itr;
	int size = m_DeleteObjects.size();
	for (int idx = 0; idx < size; idx++){
		for (itr = m_ShadowObjects.begin(); itr != m_ShadowObjects.end();){
			if (m_DeleteObjects[idx] == (*itr)){
				itr = m_ShadowObjects.erase(itr);
				break;
			}
			else{
				itr++;
			}
		}
	}
	m_DeleteObjects.clear();
}

void CShadowRender::DeleteAll(){
	m_ShadowObjects.clear();
}
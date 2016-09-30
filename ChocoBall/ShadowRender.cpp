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
	int TexSize = 512;
	m_RenderTarget.CreateRenderingTarget(/*WINDOW_WIDTH*/TexSize, TexSize /*WINDOW_HEIGHT*/, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT);
	for (short i = 0, w_num = 0, h_num = 0; i < 2; i++){
		if (i % 2 == 0){
			w_num++;
		}
		else{
			h_num++;
		}
		m_size[i] = D3DXVECTOR2(WINDOW_WIDTH >> w_num, WINDOW_HEIGHT >> h_num);
		m_BlurTarget[i].CreateRenderingTarget(/*WINDOW_WIDTH*/TexSize >> w_num, /*WINDOW_HEIGHT*/TexSize >> h_num, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT);
	}
	m_camera.Initialize();

	// 影描画用プロジェクション行列生成用の値をセット
	m_camera.SetUpdateType(EUpdateType::enUpdateTypeTarget);
	m_camera.SetNear(1.0f);
	m_camera.SetFar(30.0f);
	m_camera.SetAspect(WINDOW_WIDTH / WINDOW_HEIGHT);
	m_camera.SetViewAngle(D3DXToRadian(60));
	m_camera.SetIsOrtho(true);	// 平行投影モード
	m_camera.SetViewVolume(D3DXVECTOR2(20.0f, 20.0f));
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/TransformedShader.hlsl"));
}

void CShadowRender::Update(){
	m_camera.SetPos(m_ShadowLightPos);
	m_camera.SetTarget(m_target);
	m_camera.Update();
}

void CShadowRender::Draw(){
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

	// ブラーをかける(5点ブラー)
	m_Primitive = SINSTANCE(CRenderContext)->GetPrimitive();
	{
		// Xブラー
		(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[0].GetSurface());
		(*graphicsDevice()).SetDepthStencilSurface(m_BlurTarget[0].GetZMap());
		(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

		m_pEffect->SetTechnique("TransformedPrimBlurX");
		m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		m_pEffect->BeginPass(0);

		m_pEffect->SetVector("g_TexSize", &static_cast<D3DXVECTOR4>(m_size[0]));
		m_pEffect->SetTexture("g_tex", m_RenderTarget.GetTexture());
		m_pEffect->CommitChanges();

		(*graphicsDevice()).SetVertexDeclaration(m_Primitive->GetVertexDecl());
		(*graphicsDevice()).SetStreamSource(0, m_Primitive->GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
		(*graphicsDevice()).SetIndices(m_Primitive->GetIndexBuffer());

		(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		m_pEffect->EndPass();
		m_pEffect->End();
	}
	{
		// Yブラー
		(*graphicsDevice()).SetRenderTarget(0, m_BlurTarget[1].GetSurface());
		(*graphicsDevice()).SetDepthStencilSurface(m_BlurTarget[1].GetZMap());
		(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

		m_pEffect->SetTechnique("TransformedPrimBlurY");
		m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		m_pEffect->BeginPass(0);
		float offset[] = {
			0.0f,
			16.0f / static_cast<float>(m_size[1].y)
		};
		m_pEffect->SetValue("g_TexSize", m_size[1], sizeof(m_size[1]));
		m_pEffect->SetTexture("g_tex", m_BlurTarget[0].GetTexture());
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
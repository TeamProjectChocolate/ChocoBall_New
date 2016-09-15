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
	m_pMapZ = nullptr;
	m_pShadow = nullptr;
	m_pOriginalSurf = nullptr;

	// レンダリングターゲット生成
	(*graphicsDevice()).CreateDepthStencilSurface(WINDOW_WIDTH, WINDOW_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pMapZ, NULL);
	(*graphicsDevice()).CreateTexture(WINDOW_WIDTH, WINDOW_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pShadow, NULL);
	m_pShadow->GetSurfaceLevel(0, &m_pOriginalSurf);

	m_camera.Initialize();

	// 影描画用プロジェクション行列生成用の値をセット
	m_camera.SetUpdateType(EUpdateType::enUpdateTypeTarget);
	m_camera.SetNear(1.0f);
	m_camera.SetFar(100.0f);
	m_camera.SetAspect(WINDOW_WIDTH / WINDOW_HEIGHT);
	m_camera.SetViewAngle(D3DXToRadian(60));
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
	(*graphicsDevice()).SetRenderTarget(0, m_pOriginalSurf);
	(*graphicsDevice()).SetDepthStencilSurface(m_pMapZ);

	(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	m_RenderState = RenderState::None;

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
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
* @brief	描画。
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

		// もとのレンダリングターゲットを保存
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		vector<vector<OBJECT_DATA*>> Objects = SINSTANCE(CObjectManager)->GetObjectList();
		for (short hexa_idx = 0; hexa_idx < HEXA; hexa_idx++){
			(*graphicsDevice()).SetRenderTarget(0,m_pCubeSurfaces[hexa_idx]);
			(*graphicsDevice()).SetDepthStencilSurface(m_pZMap);
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			// 3Dの描画
			{
				// 基本描画
				for (short priorty = PRIORTY::PLAYER; priorty < PRIORTY::PARTICLE; priorty++){	// 優先度の高いものから更新
					int size = Objects[priorty].size();
					for (int idx = 0; idx < size; idx++){
						if (Objects[priorty][idx]->object->GetAlive()) {	// 生存しているもののみ描画
							Objects[priorty][idx]->object->Draw_EM(&m_Cameras[hexa_idx]);
						}
					}
				}
				// 蓄積したデータでインスタンシング描画
				vector<RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER_STATE::EM_Sampling_I);
				for (auto data : datas){
					data->render->Draw();
				}
				// パーティクル描画
				for (short priorty = PRIORTY::PARTICLE; priorty < PRIORTY::OBJECT2D; priorty++){	// 優先度の高いものから更新
					int size = Objects[priorty].size();
					for (int idx = 0; idx < size; idx++){
						if (Objects[priorty][idx]->object->GetAlive()){	// 生存しているもののみ描画
							Objects[priorty][idx]->object->Draw_EM(&m_Cameras[hexa_idx]);
						}
					}
				}
			}
			//// 描画したものにブルームをかける。
			//SINSTANCE(CRenderContext)->GetBloomRender()->Draw();
		}
		(*graphicsDevice()).SetRenderTarget(0, RenderingTarget);
		(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);

		tag.End();
		m_isEnable = false;	// シーンの最初のみ描画する。
	}
};

/*!
* @brief	作成。
*/
void CEM_Render::Initialize()
{
#ifdef NOT_EM
#else
	float size = 256.0f;
	// キューブマップ生成。
	(*graphicsDevice()).CreateCubeTexture(size, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,&m_pCubeTex,nullptr);
	(*graphicsDevice()).CreateDepthStencilSurface(size, SINSTANCE(CRenderContext)->GetWindowWidth(), D3DFMT_D16, D3DMULTISAMPLE_NONE,0, true, &m_pZMap, NULL);
	// サーフェイス取得。
	for (short idx = 0; idx < HEXA; idx++){
		D3DCUBEMAP_FACES type = static_cast<D3DCUBEMAP_FACES>(idx);
		m_pCubeTex->GetCubeMapSurface(type, 0, &m_pCubeSurfaces[idx]);
	}
	// カメラ生成。
	//// 六面体それぞれの面を描画する際のカメラの向き。
	//D3DXVECTOR3 lookAt[6] = {
	//	D3DXVECTOR3(1.0f, 0.0f, 0.0f), // +X
	//	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), // -X
	//	D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Y
	//	D3DXVECTOR3(0.0f, -1.0f, 0.0f), // -Y
	//	D3DXVECTOR3(0.0f, 0.0f, 1.0f), // +Z
	//	D3DXVECTOR3(0.0f, 0.0f, -1.0f) // -Z
	//};
	//// 六面体のそれぞれの面を描画する際の上方向(上面と下面の描画は特に注意)。
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
		//	&m_Proj,							// 射影マトリックス
		//	l_ViewAngle,						// 画角
		//	1.0f,							// アスペクト比
		//	l_Near,								// Nearプレーン
		//	l_Far);								// Farプレーン
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
		//D3DXMatrixLookAtLH(&m_View, &l_pos, &lookAt[idx], &up[idx]);			// ビューマトリックス設定
	}
	m_isEnable = true;
#endif
};

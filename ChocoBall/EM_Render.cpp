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
* @brief	描画。
*/
void CEM_Render::Draw()
{
#ifdef NOT_EM
	m_isEnable = false;
#endif
	if (m_isEnable) {
		LPDIRECT3DSURFACE9 RenderingTarget;
		LPDIRECT3DSURFACE9 m_SavedMapZ;

		// もとのレンダリングターゲットを保存
		(*graphicsDevice()).GetRenderTarget(0, &RenderingTarget);
		(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

		vector<OBJECT_DATA*> Objects = SINSTANCE(CObjectManager)->GetObjectList();
		for (short idx = 0; idx < HEXA; idx++){
			(*graphicsDevice()).SetRenderTarget(0,m_pCubeSurfaces[idx]);
			(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
			int size = Objects.size();
			// 3Dの描画
			{
				// 基本描画
				for (short priorty = 0; priorty < PRIORTY::PARTICLE; priorty++){	// 優先度の高いものから更新
					for (int idx = 0; idx < size; idx++){
						if (Objects[idx]->object->GetAlive()){	// 生存しているもののみ描画
							if (Objects[idx]->priority == priorty){	// 現在の優先度と一致するものを描画
								Objects[idx]->object->SetPintoWorld(SINSTANCE(CRenderContext)->GetDofRender()->GetPintWorld());
								Objects[idx]->object->SetPintoPos(SINSTANCE(CRenderContext)->GetDofRender()->GetPintoObject()->GetPos());
								Objects[idx]->object->Draw();
							}
						}
					}
				}
				// 蓄積したデータでインスタンシング描画
				vector<RENDER_DATA*> datas = SINSTANCE(CRenderContext)->GetRenderArray(RENDER_STATE::Instancing);
				for (auto data : datas){
					data->render->Draw();
				}
				// パーティクル描画
				for (short priorty = PRIORTY::PARTICLE; priorty < PRIORTY::OBJECT2D; priorty++){	// 優先度の高いものから更新
					for (int idx = 0; idx < size; idx++){

						if (Objects[idx]->object->GetAlive()){	// 生存しているもののみ描画
							if (Objects[idx]->priority == priorty){	// 現在の優先度と一致するものを描画
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
		m_isEnable = false;	// シーンの最初のみ描画する。
	}
};

/*!
* @brief	作成。
*/
void CEM_Render::Initialize()
{
	// キューブマップ生成。
	D3DXCreateCubeTexture(&(*graphicsDevice()),SINSTANCE(CRenderContext)->GetWindowWidth(), 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,&m_pCubeTex);
	// サーフェイス取得。
	for (short idx = 0; idx < HEXA; idx++){
		D3DCUBEMAP_FACES type = static_cast<D3DCUBEMAP_FACES>(idx);
		m_pCubeTex->GetCubeMapSurface(type, 0, &m_pCubeSurfaces[idx]);
	}

	// カメラ生成。
	// 六面体それぞれの面を描画する際のカメラの向き。
	D3DXVECTOR3 lookAt[6] = {
		D3DXVECTOR3(1.0f, 0.0f, 0.0f), // +X
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f), // -X
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), // +Y
		D3DXVECTOR3(0.0f, -1.0f, 0.0f), // -Y
		D3DXVECTOR3(0.0f, 0.0f, 1.0f), // +Z
		D3DXVECTOR3(0.0f, 0.0f, -1.0f) // -Z
	};
	// 六面体のそれぞれの面を描画する際の上方向(上面と下面の描画は特に注意)。
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

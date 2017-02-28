#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "C3DImage.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "Infomation.h"

class CShadowRender
{
	SINGLETON_DECL(CShadowRender)
	enum RenderState{None = 0,Boneless,Bone};
public:
	// 影を生成するオブジェクトを登録する関数
	// 引き数： C3DImage* C3DImageを継承したクラスのポインタ
	// 返り値： なし
	void Entry(CGameObject*);
	void Initialize();
	void Update();
	void UpdateWeight(float);
	void Draw();
	void BeginDraw();
	void EndDraw();
	void SetObjectPos(D3DXVECTOR3 pos){
		m_target = pos;
	}
	void SetShadowCameraPos(D3DXVECTOR3 pos){
		m_ShadowLightPos = pos;
	}
	void SetShadowCameraDir(D3DXVECTOR3 dir){
		m_ShadowLightDirection = dir;
	}
	LPDIRECT3DTEXTURE9 GetTexture(){
#ifdef NOT_VSM
		return m_RenderTarget.GetTexture();
#else
		// 最終的な影のテクスチャを返却
		return m_BlurTarget[1].GetTexture();
#endif
	}
	void SetShadowCamera(LPD3DXEFFECT effect){
		effect->SetMatrix("LightViewProj", &(m_camera.GetView() * m_camera.GetProj()));
	}
	void DeleteObject(CGameObject*);
	void DeleteObjectImidieit(CGameObject*);
	void CleanManager();
	void ExcuteDeleteObjects();
private:
	CRenderTarget m_RenderTarget;
	CRenderTarget m_BlurTarget[2];
	D3DXVECTOR2 m_size[2];
	vector<CGameObject*> m_ShadowObjects;	// 影を生成したいオブジェクトの配列
	vector<CGameObject*> m_DeleteObjects;
	D3DXVECTOR3 m_target;		// 影を生成したいオブジェクトのポジション
	D3DXVECTOR3 m_ShadowLightPos;		// 影を描画するためのカメラのポジション
	D3DXVECTOR3 m_ShadowLightDirection;		// 影を描画するためのカメラの向き
	CCamera m_camera;
	D3DXMATRIX m_View;					// ライトのビュー行列
	D3DXMATRIX m_Proj;					// ライトのプロジェクション行列
	void DeleteAll();
	static const int NUM_WEIGHTS = 8;
	float	m_weights[NUM_WEIGHTS];
	LPD3DXEFFECT m_pEffect;
	CPrimitive* m_Primitive;
};

